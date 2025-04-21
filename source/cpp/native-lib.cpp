#include <iostream>
#include <string>
#include <memory>

// Include our pure C++ utility macros without Objective-C dependencies
#include "utility.h"

// Include platform-specific headers with proper guards
#ifdef __APPLE__
    #include "hooks/hooks.hpp"
    #include "memory/mem.hpp"
    #include "ios/ExecutionEngine.h"
    #include "ios/ScriptManager.h"
    #include "ios/UIController.h"
    #include "ios/ai_features/AIIntegration.h"
    #include "ios/ai_features/AIIntegrationManager.h"
#endif

// Forward declarations for RobloxExecutor namespace
namespace RobloxExecutor {
    struct InitOptions {
        bool enableLogging;
        bool enableErrorReporting;
        bool enablePerformanceMonitoring;
        bool enableSecurity;
        bool enableJailbreakBypass;
        bool enableUI;
    };
    
    bool Initialize(const InitOptions& options);
    void Shutdown();
    
    namespace SystemState {
        // Platform-specific declarations inside platform-specific guards
        #ifdef __APPLE__
            // iOS types
            std::shared_ptr<iOS::ExecutionEngine> GetExecutionEngine();
            std::shared_ptr<iOS::UIController> GetUIController();
        #endif
    }
}

// C interface
extern "C" {
    // Initialization function that runs when library is loaded
    __attribute__((constructor))
    void dylib_initializer() {
        std::cout << "Roblox Executor dylib initializing" << std::endl;
        
        #ifdef __APPLE__
        // Initialize the hook engine
        Hooks::HookEngine::Initialize();
        
        // Initialize memory system
        Memory::Initialize();
        
        // iOS-specific initialization
        std::cout << "Initializing iOS integration" << std::endl;
        #endif
    }
    
    __attribute__((destructor))
    void dylib_finalizer() {
        std::cout << "Roblox Executor dylib shutting down" << std::endl;
        
        #ifdef __APPLE__
        // Clean up hooks
        Hooks::HookEngine::ClearAllHooks();
        #endif
    }
    
    // Lua module entry point
    int luaopen_mylibrary(void* L) {
        // Cast to void to remove the unused parameter warning
        (void)(L);
        
        std::cout << "Lua module loaded: mylibrary" << std::endl;
        
        // This will be called when the Lua state loads our library
        // Perform any Lua-specific initialization here
        
        return 1; // Return 1 to indicate success
    }
}
