#include "../memory/ci_compat.h"

#pragma once

#include <string>
#include <vector>
#include <map>
#include <mutex>
#include <cstdint>

// We don't need to forward declare UIImage - it's already defined in UIKit
// and forward-declaring it in C++ context causes conflicts

namespace iOS {
    // Pattern scanning utilities for iOS
    class PatternScanner {
    public:
        // Result structure for pattern scanning
        struct ScanResult {
            uintptr_t address;
            size_t size;
            
            ScanResult() : address(0), size(0) {}
            ScanResult(uintptr_t addr, size_t s) : address(addr), size(s) {}
            
            operator bool() const { return address != 0; }
        };
        
        // Find a byte pattern in memory
        static ScanResult FindPattern(const char* module, const char* pattern, const char* mask) {
            // Implement real pattern scanning on iOS
            // Stub implementation for now - would need real pattern scanning on iOS
            return ScanResult(0, 0);
        }
        
        // Find pattern using string representation (like "48 8B 05 ?? ?? ?? ??")
        static ScanResult FindPattern(const char* module, const char* patternString) {
            // Parse pattern string and convert to byte pattern + mask
            std::vector<uint8_t> pattern;
            std::vector<bool> mask;
            
            // Parse the pattern string
            for (const char* p = patternString; *p; ++p) {
                if (*p == ' ') continue;
                
                if (*p == '?') {
                    pattern.push_back(0);
                    mask.push_back(false);
                    
                    if (*(p + 1) == '?') ++p; // Skip second ? if present
                } else {
                    // Convert hex string to byte
                    char hex[3] = {*p, *(p + 1), 0};
                    pattern.push_back(static_cast<uint8_t>(strtol(hex, nullptr, 16)));
                    mask.push_back(true);
                    ++p; // Skip second character
                }
            }
            
            // Create pattern and mask strings
            std::string patternStr(pattern.begin(), pattern.end());
            std::string maskStr;
            for (bool b : mask) {
                maskStr.push_back(b ? 'x' : '?');
            }
            
            return FindPattern(module, patternStr.c_str(), maskStr.c_str());
        }
        
        // Get base address of the main executable
        static uintptr_t GetBaseAddress();
        
        // Get base address of a specific module
        static uintptr_t GetModuleBaseAddress(const std::string& moduleName);
        
        // Get module info (base address and size)
        static std::pair<uintptr_t, size_t> GetModuleInfo(const std::string& moduleName);
        
        // Find a symbol in a module
        static void* FindSymbol(const std::string& moduleName, const std::string& symbolName);
        
        // Additional methods that are implemented in PatternScanner.mm but were missing from the header
        static ScanResult ScanForSignature(const std::string& signature, void* startAddress, void* endAddress);
        static ScanResult ScanForString(const std::string& str, void* startAddress, void* endAddress);
        static std::vector<ScanResult> FindAllPatterns(const char* pattern, const char* mask, void* startAddress, void* endAddress);
        static size_t GetModuleSize(const std::string& moduleName);
        
        // Core pattern scanning method missing from header
        static ScanResult ScanForPattern(const char* pattern, const char* mask, void* startAddress, void* endAddress);
    };
}
