// PatternScanner.mm - Production-grade implementation
#include "PatternScanner.h"
#include "MemoryAccess.h"
#include <iostream>
#include <vector>
#include <unordered_map>
#include <mutex>
#include <algorithm>
#include <iomanip>
#include <mach/mach.h>
#include <mach-o/dyld.h>
#include <dlfcn.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sstream>

namespace iOS {
    // Static cache variables for performance
    static std::unordered_map<std::string, uintptr_t> moduleBaseCache;
    static std::unordered_map<std::string, size_t> moduleSizeCache;
    static std::mutex cacheMutex;

    // Convert a hex signature string to a byte pattern and mask
    static std::pair<std::vector<uint8_t>, std::string> ParseSignature(const std::string& signature) {
        std::vector<uint8_t> pattern;
        std::string mask;
        
        std::istringstream stream(signature);
        std::string byteStr;
        
        while (stream >> byteStr) {
            if (byteStr == "?" || byteStr == "??") {
                // Wildcard byte
                pattern.push_back(0);
                mask.push_back('?');
            } else {
                // Convert hex string to byte
                try {
                    uint8_t byte = static_cast<uint8_t>(std::stoul(byteStr, nullptr, 16));
                    pattern.push_back(byte);
                    mask.push_back('x');
                } catch (const std::exception& e) {
                    std::cerr << "PatternScanner: Invalid byte in signature: " << byteStr << std::endl;
                    // Handle invalid hex by treating as wildcard
                    pattern.push_back(0);
                    mask.push_back('?');
                }
            }
        }
        
        return {pattern, mask};
    }
    
    // Implements Boyer-Moore-Horspool pattern matching algorithm for efficient scanning
    PatternScanner::ScanResult PatternScanner::ScanForPattern(const char* pattern, const char* mask, void* startAddress, void* endAddress) {
        if (!pattern || !mask) {
            std::cerr << "PatternScanner: Null pattern or mask provided" << std::endl;
            return ScanResult(0, 0);
        }
        
        size_t patternLength = strlen(mask);
        if (patternLength == 0) {
            std::cerr << "PatternScanner: Empty pattern" << std::endl;
            return ScanResult(0, 0);
        }
        
        // Get process memory bounds if not specified
        if (!startAddress || !endAddress) {
            task_t task = mach_task_self();
            vm_address_t address = 0;
            vm_size_t size = 0;
            uint32_t depth = 1;
            
            startAddress = reinterpret_cast<void*>(GetBaseAddress());
            if (!startAddress) {
                std::cerr << "PatternScanner: Failed to get base address" << std::endl;
                return ScanResult(0, 0);
            }
            
            // Use a fixed large size if we can't get actual module size
            endAddress = reinterpret_cast<void*>(reinterpret_cast<uintptr_t>(startAddress) + 0x10000000); // 256 MB search space
        }
        
        // Ensure addresses are valid
        uintptr_t start = reinterpret_cast<uintptr_t>(startAddress);
        uintptr_t end = reinterpret_cast<uintptr_t>(endAddress);
        
        if (start >= end) {
            std::cerr << "PatternScanner: Invalid address range" << std::endl;
            return ScanResult(0, 0);
        }
        
        // Create bad character table for Boyer-Moore-Horspool algorithm
        size_t badCharTable[256];
        for (size_t i = 0; i < 256; i++) {
            badCharTable[i] = patternLength;
        }
        
        for (size_t i = 0; i < patternLength - 1; i++) {
            if (mask[i] == 'x') {
                badCharTable[static_cast<uint8_t>(pattern[i])] = patternLength - i - 1;
            }
        }
        
        // Scan memory for pattern
        size_t scanSize = end - start;
        const size_t bufferSize = 4096; // Read memory in chunks to improve performance
        uint8_t buffer[bufferSize];
        
        for (size_t offset = 0; offset < scanSize; ) {
            // Calculate how much to read
            size_t bytesToRead = std::min(bufferSize, scanSize - offset);
            if (bytesToRead < patternLength) {
                break; // Not enough memory left to match pattern
            }
            
            // Read memory chunk
            if (!MemoryAccess::ReadMemory(reinterpret_cast<void*>(start + offset), buffer, bytesToRead)) {
                // Skip unreadable memory regions
                offset += bytesToRead;
                continue;
            }
            
            // Scan this memory chunk
            size_t chunkPos = 0;
            while (chunkPos <= bytesToRead - patternLength) {
                size_t j = patternLength - 1;
                
                // Check pattern backward
                while (j != static_cast<size_t>(-1) && (mask[j] == '?' || buffer[chunkPos + j] == pattern[j])) {
                    j--;
                }
                
                if (j == static_cast<size_t>(-1)) {
                    // Pattern found
                    return ScanResult(start + offset + chunkPos, patternLength);
                }
                
                // Skip using bad character rule
                size_t skip = badCharTable[buffer[chunkPos + patternLength - 1]];
                if (skip == 0) skip = 1; // Ensure progress
                
                chunkPos += skip;
            }
            
            // Move to next chunk, overlapping a bit to handle patterns that cross chunk boundaries
            offset += (bytesToRead - patternLength + 1);
        }
        
        // Pattern not found
        return ScanResult(0, 0);
    }
    
    // Scan for a signature in hex format (e.g., "48 8B 05 ?? ?? ?? ??")
    PatternScanner::ScanResult PatternScanner::ScanForSignature(const std::string& signature, void* startAddress, void* endAddress) {
        auto [pattern, mask] = ParseSignature(signature);
        
        if (pattern.empty()) {
            std::cerr << "PatternScanner: Failed to parse signature: " << signature << std::endl;
            return ScanResult(0, 0);
        }
        
        return ScanForPattern(reinterpret_cast<const char*>(pattern.data()), mask.c_str(), startAddress, endAddress);
    }
    
    // Scan for a string in memory
    PatternScanner::ScanResult PatternScanner::ScanForString(const std::string& str, void* startAddress, void* endAddress) {
        if (str.empty()) {
            std::cerr << "PatternScanner: Empty string to scan for" << std::endl;
            return ScanResult(0, 0);
        }
        
        // Create pattern and mask from string
        std::vector<char> pattern(str.begin(), str.end());
        std::string mask(str.length(), 'x');
        
        return ScanForPattern(pattern.data(), mask.c_str(), startAddress, endAddress);
    }
    
    // Find all occurrences of a pattern
    std::vector<PatternScanner::ScanResult> PatternScanner::FindAllPatterns(const char* pattern, const char* mask, void* startAddress, void* endAddress) {
        std::vector<ScanResult> results;
        size_t patternLength = strlen(mask);
        
        if (!pattern || !mask || patternLength == 0) {
            std::cerr << "PatternScanner: Invalid pattern for FindAllPatterns" << std::endl;
            return results;
        }
        
        // Get initial result
        ScanResult result = ScanForPattern(pattern, mask, startAddress, endAddress);
        if (result.address == 0) {
            return results; // No matches
        }
        
        results.push_back(result);
        
        // Find additional matches
        uintptr_t lastAddress = result.address + patternLength;
        while (true) {
            // Get next match
            result = ScanForPattern(pattern, mask, 
                                   reinterpret_cast<void*>(lastAddress), 
                                   endAddress);
            
            if (result.address == 0) {
                break; // No more matches
            }
            
            results.push_back(result);
            lastAddress = result.address + patternLength;
        }
        
        return results;
    }
    
    // Get base address of the current process
    uintptr_t PatternScanner::GetBaseAddress() {
        return GetModuleBaseAddress(""); // Empty string = main executable
    }
    
    // Get base address of a module
    uintptr_t PatternScanner::GetModuleBaseAddress(const std::string& moduleName) {
        // Lock cache
        std::lock_guard<std::mutex> lock(cacheMutex);
        
        // Check cache first
        std::string lookupName = moduleName.empty() ? "_main" : moduleName;
        auto it = moduleBaseCache.find(lookupName);
        if (it != moduleBaseCache.end()) {
            return it->second;
        }
        
        uintptr_t baseAddress = 0;
        
        if (moduleName.empty()) {
            // Get main executable base address
            for (uint32_t i = 0; i < _dyld_image_count(); i++) {
                const char* imageName = _dyld_get_image_name(i);
                const mach_header* header = _dyld_get_image_header(i);
                
                if (imageName && strstr(imageName, "/Roblox") != nullptr) {
                    baseAddress = reinterpret_cast<uintptr_t>(header);
                    break;
                }
            }
            
            // If we couldn't find Roblox, fall back to the main executable
            if (baseAddress == 0) {
                baseAddress = reinterpret_cast<uintptr_t>(_dyld_get_image_header(0));
            }
        } else {
            // Find a specific module
            for (uint32_t i = 0; i < _dyld_image_count(); i++) {
                const char* imageName = _dyld_get_image_name(i);
                if (imageName && (strstr(imageName, moduleName.c_str()) != nullptr)) {
                    baseAddress = reinterpret_cast<uintptr_t>(_dyld_get_image_header(i));
                    break;
                }
            }
            
            // Try using dlopen as a backup
            if (baseAddress == 0) {
                void* handle = dlopen(moduleName.c_str(), RTLD_NOLOAD);
                if (!handle) {
                    // Try with various extensions
                    std::vector<std::string> attempts = {
                        moduleName + ".dylib",
                        moduleName + ".framework/" + moduleName,
                        "/usr/lib/" + moduleName,
                        "/System/Library/Frameworks/" + moduleName + ".framework/" + moduleName
                    };
                    
                    for (const auto& attempt : attempts) {
                        handle = dlopen(attempt.c_str(), RTLD_NOLOAD);
                        if (handle) {
                            break;
                        }
                    }
                }
                
                if (handle) {
                    Dl_info info;
                    if (dladdr(handle, &info) != 0) {
                        baseAddress = reinterpret_cast<uintptr_t>(info.dli_fbase);
                    }
                    dlclose(handle);
                }
            }
        }
        
        // Cache the result
        if (baseAddress != 0) {
            moduleBaseCache[lookupName] = baseAddress;
        } else {
            std::cerr << "PatternScanner: Failed to find module: " << 
                (moduleName.empty() ? "main executable" : moduleName) << std::endl;
        }
        
        return baseAddress;
    }
    
    // Get module size
    size_t PatternScanner::GetModuleSize(const std::string& moduleName) {
        // Lock cache
        std::lock_guard<std::mutex> lock(cacheMutex);
        
        // Check cache first
        std::string lookupName = moduleName.empty() ? "_main" : moduleName;
        auto it = moduleSizeCache.find(lookupName);
        if (it != moduleSizeCache.end()) {
            return it->second;
        }
        
        // Get the module base address first
        uintptr_t baseAddress = GetModuleBaseAddress(moduleName);
        if (baseAddress == 0) {
            return 0;
        }
        
        // Use memory mapping to determine module size
        task_t task = mach_task_self();
        vm_address_t address = static_cast<vm_address_t>(baseAddress);
        vm_size_t size = 0;
        
        // Find the memory region containing this address
        vm_region_basic_info_data_64_t info;
        mach_msg_type_number_t infoCount = VM_REGION_BASIC_INFO_COUNT_64;
        mach_port_t objectName = MACH_PORT_NULL;
        
        // First get the region containing the base address
        kern_return_t kr = vm_region_64(task, 
                                      &address, 
                                      &size, 
                                      VM_REGION_BASIC_INFO_64, 
                                      (vm_region_info_t)&info, 
                                      &infoCount, 
                                      &objectName);
        
        if (kr != KERN_SUCCESS) {
            // Fallback to a conservative estimate
            size_t fallbackSize = 0x1000000; // 16 MB
            moduleSizeCache[lookupName] = fallbackSize;
            return fallbackSize;
        }
        
        // Now we need to find all consecutive regions
        uintptr_t moduleEnd = baseAddress;
        vm_address_t currentAddress = address + size;
        
        // Scan for consecutive memory regions
        bool foundEnd = false;
        const size_t maxRegions = 100; // Safety limit
        size_t regionCount = 0;
        
        while (!foundEnd && regionCount < maxRegions) {
            vm_address_t regionAddress = currentAddress;
            vm_size_t regionSize = 0;
            
            kr = vm_region_64(task, 
                           &regionAddress, 
                           &regionSize, 
                           VM_REGION_BASIC_INFO_64, 
                           (vm_region_info_t)&info, 
                           &infoCount, 
                           &objectName);
            
            if (kr != KERN_SUCCESS || regionAddress > currentAddress) {
                // Gap in memory or end of regions
                foundEnd = true;
            } else {
                // Check if this is still the same module
                // For iOS, we can't always rely on shared segment names
                // Instead we check if the memory is the expected protection
                bool isExecutable = (info.protection & VM_PROT_EXECUTE) != 0;
                bool isPartOfModule = isExecutable || 
                                     (info.protection & VM_PROT_READ) != 0;
                
                if (isPartOfModule && (regionAddress == currentAddress)) {
                    // Still part of the module
                    moduleEnd = regionAddress + regionSize;
                    currentAddress = moduleEnd;
                } else {
                    // Different module/end of module
                    foundEnd = true;
                }
            }
            
            regionCount++;
        }
        
        // Calculate final size
        size_t moduleSize = moduleEnd - baseAddress;
        
        // Validate size is reasonable
        if (moduleSize > 0x10000000) { // > 256 MB is suspicious
            moduleSize = 0x1000000; // Fallback to 16 MB
        }
        
        // Cache and return the size
        moduleSizeCache[lookupName] = moduleSize;
        return moduleSize;
    }
}
