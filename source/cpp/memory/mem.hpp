#pragma once

#include <cstdint>
#include <vector>
#include <string>
#include <memory>
#include <functional>

/**
 * Memory management namespace for the Roblox Executor
 * Provides utilities for memory manipulation, scanning, and patching
 */
namespace Memory {
    // Initialize the memory subsystem
    bool Initialize();
    
    // Clean up memory resources
    void Shutdown();
    
    // Memory protection utilities
    bool ProtectMemory(uintptr_t address, size_t size, bool executable, bool writable);
    
    // Memory patching utilities
    bool PatchMemory(uintptr_t address, const std::vector<uint8_t>& bytes);
    bool PatchMemoryNOP(uintptr_t address, size_t count);
    
    // Memory scanning utilities
    uintptr_t FindPattern(const std::string& module, const std::string& pattern, const std::string& mask);
    std::vector<uintptr_t> FindPatterns(const std::string& module, const std::string& pattern, const std::string& mask, size_t maxResults = 10);
    
    // Memory reading utilities
    template<typename T>
    T Read(uintptr_t address) {
        return *reinterpret_cast<T*>(address);
    }
    
    // Memory writing utilities
    template<typename T>
    bool Write(uintptr_t address, const T& value) {
        *reinterpret_cast<T*>(address) = value;
        return true;
    }
    
    // Memory allocation utilities
    void* AllocateMemory(size_t size, bool executable = false);
    void FreeMemory(void* memory);
    
    // Hook utilities
    using HookCallback = std::function<void(void*)>;
    bool CreateHook(uintptr_t target, void* detour, void** original);
    bool RemoveHook(uintptr_t target);
}
