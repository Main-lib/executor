#include "hooks/hooks.hpp"
#include <iostream>

#ifdef USE_DOBBY
// If Dobby is available, include its header
#include "dobby.h"
#endif

namespace Hooks {

// Static member initialization
std::unordered_map<uintptr_t, HookInfo> HookEngine::s_hooks;
bool HookEngine::s_initialized = false;

bool HookEngine::Initialize() {
    if (s_initialized) {
        return true;
    }
    
    std::cout << "Hook engine initialized" << std::endl;
    s_initialized = true;
    return true;
}

void HookEngine::Shutdown() {
    if (!s_initialized) {
        return;
    }
    
    // Remove all hooks
    ClearAllHooks();
    
    std::cout << "Hook engine shutdown" << std::endl;
    s_initialized = false;
}

bool HookEngine::CreateHook(uintptr_t target, HookFunction detour, HookFunction* original, const std::string& name) {
    if (!s_initialized) {
        std::cerr << "Hook engine not initialized" << std::endl;
        return false;
    }
    
    // Check if hook already exists
    if (s_hooks.find(target) != s_hooks.end()) {
        std::cerr << "Hook already exists at target address: " << std::hex << target << std::endl;
        return false;
    }
    
#ifdef USE_DOBBY
    // Use Dobby to create the hook
    if (DobbyHook((void*)target, detour, original) != kDobbySuccess) {
        std::cerr << "Failed to create hook with Dobby" << std::endl;
        return false;
    }
#else
    // Fallback implementation without Dobby
    *original = (void*)target;
    std::cout << "Warning: Dobby not available, hook will not be applied" << std::endl;
#endif
    
    // Store hook information
    HookInfo info;
    info.target = target;
    info.detour = detour;
    info.original = *original;
    info.name = name;
    info.enabled = true;
    
    s_hooks[target] = info;
    
    std::cout << "Hook created at " << std::hex << target << " with name: " << name << std::endl;
    return true;
}

bool HookEngine::RemoveHook(uintptr_t target) {
    if (!s_initialized) {
        std::cerr << "Hook engine not initialized" << std::endl;
        return false;
    }
    
    // Check if hook exists
    auto it = s_hooks.find(target);
    if (it == s_hooks.end()) {
        std::cerr << "No hook exists at target address: " << std::hex << target << std::endl;
        return false;
    }
    
#ifdef USE_DOBBY
    // Use Dobby to remove the hook
    if (DobbyDestroy((void*)target) != kDobbySuccess) {
        std::cerr << "Failed to remove hook with Dobby" << std::endl;
        return false;
    }
#endif
    
    // Remove hook information
    s_hooks.erase(it);
    
    std::cout << "Hook removed at " << std::hex << target << std::endl;
    return true;
}

bool HookEngine::EnableHook(uintptr_t target, bool enable) {
    if (!s_initialized) {
        std::cerr << "Hook engine not initialized" << std::endl;
        return false;
    }
    
    // Check if hook exists
    auto it = s_hooks.find(target);
    if (it == s_hooks.end()) {
        std::cerr << "No hook exists at target address: " << std::hex << target << std::endl;
        return false;
    }
    
    // Update hook status
    it->second.enabled = enable;
    
    std::cout << "Hook " << (enable ? "enabled" : "disabled") << " at " << std::hex << target << std::endl;
    return true;
}

std::vector<HookInfo> HookEngine::GetAllHooks() {
    std::vector<HookInfo> hooks;
    
    for (const auto& pair : s_hooks) {
        hooks.push_back(pair.second);
    }
    
    return hooks;
}

void HookEngine::ClearAllHooks() {
    if (!s_initialized) {
        return;
    }
    
    // Remove all hooks
    for (const auto& pair : s_hooks) {
#ifdef USE_DOBBY
        DobbyDestroy((void*)pair.first);
#endif
    }
    
    s_hooks.clear();
    
    std::cout << "All hooks cleared" << std::endl;
}

} // namespace Hooks
