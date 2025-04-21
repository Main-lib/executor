#pragma once

#include <cstdint>
#include <string>
#include <functional>
#include <vector>
#include <unordered_map>

namespace Hooks {

// Function pointer type for hooks
using HookFunction = void*;

// Hook callback type
using HookCallback = std::function<void(void*)>;

// Hook information structure
struct HookInfo {
    uintptr_t target;
    HookFunction detour;
    HookFunction original;
    std::string name;
    bool enabled;
};

// Hook engine class
class HookEngine {
public:
    // Initialize the hook engine
    static bool Initialize();
    
    // Clean up and remove all hooks
    static void Shutdown();
    
    // Create a new hook
    static bool CreateHook(uintptr_t target, HookFunction detour, HookFunction* original, const std::string& name = "");
    
    // Remove a hook
    static bool RemoveHook(uintptr_t target);
    
    // Enable/disable a hook
    static bool EnableHook(uintptr_t target, bool enable);
    
    // Get all registered hooks
    static std::vector<HookInfo> GetAllHooks();
    
    // Clear all hooks
    static void ClearAllHooks();
    
private:
    // Map of target addresses to hook information
    static std::unordered_map<uintptr_t, HookInfo> s_hooks;
    
    // Whether the hook engine is initialized
    static bool s_initialized;
};

} // namespace Hooks
