#pragma once

#include "funcs.hpp"
#include "naming_conventions.hpp"
#include "../ios/ExecutionEngine.h"
#include <memory>
#include <map>

// i chose this way of organization as i think its a bit more readable
// but of course its not the best one, find a better way
// all thanks to android studio not letting me use .cpp files

static int loadstring(lua_State* ls);
static int executeWithOptions(lua_State* ls);
static int getScriptMemoryUsage(lua_State* ls);
static int collectGarbageWrapper(lua_State* ls);
static int optimizeScript(lua_State* ls);

// Register our enhanced implementation functions for use in Lua
void regImpls(lua_State* thread){
    // Original loadstring implementation
    lua_pushcclosure(thread,loadstring,"loadstring",0);
    lua_setfield(thread,-10002,"loadstring");
    
    // Add new enhanced execution functions
    lua_pushcclosure(thread,executeWithOptions,"executeWithOptions",0);
    lua_setfield(thread,-10002,"executeWithOptions");
    
    lua_pushcclosure(thread,getScriptMemoryUsage,"getScriptMemoryUsage",0);
    lua_setfield(thread,-10002,"getScriptMemoryUsage");
    
    lua_pushcclosure(thread,collectGarbageWrapper,"collectGarbage",0);
    lua_setfield(thread,-10002,"collectGarbage");
    
    lua_pushcclosure(thread,optimizeScript,"optimizeScript",0);
    lua_setfield(thread,-10002,"optimizeScript");
    
    // Register all naming conventions to support different executor APIs
    RegisterAllNamingConventions(thread);
}

// Original loadstring implementation
int loadstring(lua_State* ls){
    const char* s = lua_tostring(ls,1);

    // Define bytecode_encoder_t if not already defined
    #ifndef BYTECODE_ENCODER_DEFINED
    #define BYTECODE_ENCODER_DEFINED
    typedef struct {
        int reserved;
    } bytecode_encoder_t;
    #endif

    bytecode_encoder_t encoder;
    
    // For iOS build, provide a simplified compile function that just returns the input string
    // as we don't have the actual Luau compiler infrastructure
    #if defined(IOS_TARGET) || defined(__APPLE__)
    std::string bc = s; // Just use the input string directly
    #else
    auto bc = Luau::compile(s,{},{},&encoder);
    #endif

    const char* chunkname{};
    if (lua_gettop(ls) == 2) chunkname = lua_tostring(ls, 2);
    else chunkname = "insertrandomgeneratedstring";

    #if defined(IOS_TARGET) || defined(__APPLE__)
    // For iOS, we'll use the standard luau_load function
    if (luau_load(ls, chunkname, bc.c_str(), bc.size(), 0))
    #else
    if (rluau_load(ls, chunkname, bc.c_str(), bc.size(), 0))
    #endif
    {
        lua_pushnil(ls);
        lua_pushstring(ls, lua_tostring(ls, -2));
        return 2;
    }
    return 1;
}

// New implementation: execute script with options
// Usage in Lua: executeWithOptions(script, {obfuscate=true, timeout=1000, ...})
int executeWithOptions(lua_State* ls) {
    if (lua_gettop(ls) < 1 || !lua_isstring(ls, 1)) {
        lua_pushboolean(ls, 0);
        lua_pushstring(ls, "First argument must be a string (script)");
        return 2;
    }

    const char* script = lua_tostring(ls, 1);
    
    // Create default options
    ExecutionOptions options;
    
    // Parse options table if provided
    if (lua_gettop(ls) >= 2 && lua_istable(ls, 2)) {
        // Get obfuscation option
        lua_getfield(ls, 2, "obfuscate");
        if (!lua_isnil(ls, -1)) {
            options.enableObfuscation = lua_toboolean(ls, -1);
        }
        lua_pop(ls, 1);
        
        // Get anti-detection option
        lua_getfield(ls, 2, "antiDetection");
        if (!lua_isnil(ls, -1)) {
            options.enableAntiDetection = lua_toboolean(ls, -1);
        }
        lua_pop(ls, 1);
        
        // Get timeout option
        lua_getfield(ls, 2, "timeout");
        if (lua_isnumber(ls, -1)) {
            options.timeout = (int)lua_tonumber(ls, -1);
        }
        lua_pop(ls, 1);
        
        // Get capture output option
        lua_getfield(ls, 2, "captureOutput");
        if (!lua_isnil(ls, -1)) {
            options.captureOutput = lua_toboolean(ls, -1);
        }
        lua_pop(ls, 1);
        
        // Get auto retry option
        lua_getfield(ls, 2, "autoRetry");
        if (!lua_isnil(ls, -1)) {
            options.autoRetry = lua_toboolean(ls, -1);
        }
        lua_pop(ls, 1);
        
        // Get environment variables
        lua_getfield(ls, 2, "env");
        if (lua_istable(ls, -1)) {
            // Iterate over the env table
            lua_pushnil(ls);  // First key
            while (lua_next(ls, -2) != 0) {
                // Key at -2, value at -1
                if (lua_isstring(ls, -2) && lua_isstring(ls, -1)) {
                    options.environment[lua_tostring(ls, -2)] = lua_tostring(ls, -1);
                }
                lua_pop(ls, 1);  // Remove value, keep key for next iteration
            }
        }
        lua_pop(ls, 1);  // Pop the env table
    }
    
    // Execute the script
    ExecutionStatus status = executescript(ls, script, options);
    
    // Return results
    lua_pushboolean(ls, status.success);
    if (status.success) {
        // Success: return true and output if available
        if (!status.output.empty()) {
            lua_pushstring(ls, status.output.c_str());
        } else {
            lua_pushnil(ls);
        }
        
        // Return memory usage as a third return value
        lua_pushnumber(ls, status.memoryUsed);
        return 3;
    } else {
        // Error: return false and error message
        lua_pushstring(ls, status.error.c_str());
        return 2;
    }
}

// Get current script memory usage
int getScriptMemoryUsage(lua_State* ls) {
    size_t memoryUsage = GetMemoryUsage();
    lua_pushnumber(ls, memoryUsage);
    return 1;
}

// Wrapper for collectGarbage
int collectGarbageWrapper(lua_State* ls) {
    bool full = false;
    if (lua_gettop(ls) >= 1) {
        full = lua_toboolean(ls, 1);
    }
    
    size_t freedBytes = CollectGarbage(full);
    lua_pushnumber(ls, freedBytes);
    return 1;
}

// Optimize script using AI (just a placeholder in this implementation)
int optimizeScript(lua_State* ls) {
    if (lua_gettop(ls) < 1 || !lua_isstring(ls, 1)) {
        lua_pushnil(ls);
        lua_pushstring(ls, "Argument must be a string (script)");
        return 2;
    }
    
    const char* script = lua_tostring(ls, 1);
    std::string optimized = OptimizeScript(script);
    
    lua_pushstring(ls, optimized.c_str());
    return 1;
}

// Implementation of Execution namespace functions that integrate with iOS::ExecutionEngine

namespace Execution {
    // Convert our ExecutionOptions to iOS::ExecutionEngine::ExecutionContext
    iOS::ExecutionEngine::ExecutionContext ConvertToExecutionContext(const ExecutionOptions& options) {
        iOS::ExecutionEngine::ExecutionContext context;
        
        context.m_enableObfuscation = options.enableObfuscation;
        context.m_enableAntiDetection = options.enableAntiDetection;
        context.m_autoRetry = options.autoRetry;
        context.m_maxRetries = options.maxRetries;
        context.m_timeout = options.timeout;
        context.m_environment = options.environment;
        
        return context;
    }
    
    // Execute script with options using the iOS execution engine
    ScriptResult ExecuteScriptWithOptions(const std::string& script, const ScriptOptions& options) {
        ScriptResult result;
        
        try {
            // Create iOS execution engine
            std::shared_ptr<iOS::ExecutionEngine> engine = std::make_shared<iOS::ExecutionEngine>();
            
            // Initialize the engine
            if (!engine->Initialize()) {
                result.success = false;
                result.error = "Failed to initialize execution engine";
                return result;
            }
            
            // Create execution context
            iOS::ExecutionEngine::ExecutionContext context;
            context.m_enableObfuscation = options.useObfuscation;
            context.m_enableAntiDetection = options.useAntiDetection;
            context.m_timeout = options.timeout;
            context.m_environment = options.environment;
            
            // Set output callback if capturing output
            if (options.captureOutput) {
                engine->SetOutputCallback([&result](const std::string& output) {
                    result.output += output;
                    
                    // Forward to global output callback if set
                    if (ExecutionState::outputCallback) {
                        ExecutionState::outputCallback(output);
                    }
                });
            }
            
            // Execute script
            iOS::ExecutionEngine::ExecutionResult execResult = engine->Execute(script, context);
            
            // Fill result
            result.success = execResult.m_success;
            result.error = execResult.m_error;
            result.executionTime = execResult.m_executionTime;
            result.output = execResult.m_output;
            
            // Track memory usage
            ExecutionState::memoryUsage += execResult.m_executionTime * 10; // Rough estimate
        }
        catch (const std::exception& e) {
            result.success = false;
            result.error = std::string("Exception: ") + e.what();
        }
        
        return result;
    }
    
    // Basic script execution with iOS execution engine
    bool ExecuteScript(const std::string& script, std::string& error) {
        try {
            iOS::ExecutionEngine engine;
            if (!engine.Initialize()) {
                error = "Failed to initialize execution engine";
                return false;
            }
            
            iOS::ExecutionEngine::ExecutionResult result = engine.Execute(script);
            
            if (!result.m_success) {
                error = result.m_error;
            }
            
            return result.m_success;
        }
        catch (const std::exception& e) {
            error = std::string("Exception: ") + e.what();
            return false;
        }
    }
}
