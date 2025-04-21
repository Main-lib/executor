#pragma once

#include <string>
#include <vector>
#include <map>
#include <functional>
#include <memory>
#include <unordered_map>

#include "../globals.hpp"
#include "../luau/lua.h"

/**
 * @file naming_conventions.hpp
 * @brief Implements naming conventions from both UNC and SNC repositories
 * 
 * This file contains the implementation of naming conventions from both
 * the UNC (Unified Naming Conventions) and SNC repositories for Roblox.
 * It provides a unified interface for registering and using these conventions
 * in the executor.
 */

namespace NamingConventions {

// Function type for naming convention implementations
using ConventionFunction = std::function<int(lua_State*)>;

// Structure to hold information about a naming convention
struct Convention {
    std::string name;                  // Primary name of the convention
    std::vector<std::string> aliases;  // Alternative names (aliases)
    ConventionFunction function;       // Implementation function
    std::string description;           // Description of what the convention does
    std::string source;                // Source repository (UNC, SNC, or BOTH)
    
    Convention() {}
    
    Convention(const std::string& name, 
               const std::vector<std::string>& aliases,
               ConventionFunction function,
               const std::string& description,
               const std::string& source)
        : name(name), aliases(aliases), function(function), 
          description(description), source(source) {}
};

// Registry for all naming conventions
class ConventionRegistry {
private:
    std::unordered_map<std::string, Convention> m_conventions;
    
public:
    // Register a convention
    void RegisterConvention(const Convention& convention) {
        m_conventions[convention.name] = convention;
        
        // Also register aliases
        for (const auto& alias : convention.aliases) {
            m_conventions[alias] = convention;
        }
    }
    
    // Get a convention by name
    Convention* GetConvention(const std::string& name) {
        auto it = m_conventions.find(name);
        if (it != m_conventions.end()) {
            return &it->second;
        }
        return nullptr;
    }
    
    // Get all conventions
    std::vector<Convention> GetAllConventions() {
        std::vector<Convention> result;
        std::unordered_map<std::string, bool> added;
        
        for (const auto& pair : m_conventions) {
            // Skip aliases (only include primary names)
            if (added.find(pair.second.name) == added.end()) {
                result.push_back(pair.second);
                added[pair.second.name] = true;
            }
        }
        
        return result;
    }
    
    // Register all conventions with a Lua state
    void RegisterWithLuaState(lua_State* L) {
        for (const auto& pair : m_conventions) {
            const auto& convention = pair.second;
            
            // Only register primary names (not aliases)
            if (pair.first == convention.name) {
                lua_pushcclosure(L, [](lua_State* L) -> int {
                    // Get the convention from registry
                    Convention* convention = static_cast<Convention*>(lua_touserdata(L, lua_upvalueindex(1)));
                    if (convention && convention->function) {
                        return convention->function(L);
                    }
                    return 0;
                }, convention.name.c_str(), 1);
                
                // Store the convention as userdata in the closure
                Convention* conventionPtr = static_cast<Convention*>(lua_newuserdata(L, sizeof(Convention)));
                *conventionPtr = convention;
                lua_setupvalue(L, -2, 1);
                
                // Set the function in the global table
                lua_setglobal(L, convention.name.c_str());
                
                // Register aliases
                for (const auto& alias : convention.aliases) {
                    lua_getglobal(L, convention.name.c_str());
                    lua_setglobal(L, alias.c_str());
                }
            }
        }
    }
};

// Global registry instance
static ConventionRegistry g_registry;

// Initialize all naming conventions
void InitializeConventions();

// Cache-related conventions
namespace Cache {
    // Implementation of cache.invalidate
    int invalidate(lua_State* L);
    
    // Implementation of cache.iscached
    int iscached(lua_State* L);
    
    // Implementation of cache.replace
    int replace(lua_State* L);
    
    // Implementation of cloneref
    int cloneref(lua_State* L);
    
    // Implementation of compareinstances
    int compareinstances(lua_State* L);
}

// Closure-related conventions
namespace Closures {
    // Implementation of checkcaller
    int checkcaller(lua_State* L);
    
    // Implementation of clonefunction
    int clonefunction(lua_State* L);
    
    // Implementation of getcallingscript
    int getcallingscript(lua_State* L);
    
    // Implementation of getscriptclosure
    int getscriptclosure(lua_State* L);
    
    // Implementation of hookfunction
    int hookfunction(lua_State* L);
    
    // Implementation of closuretype
    int closuretype(lua_State* L);
    
    // Implementation of iscclosure
    int iscclosure(lua_State* L);
    
    // Implementation of islclosure
    int islclosure(lua_State* L);
    
    // Implementation of isexecutorclosure
    int isexecutorclosure(lua_State* L);
    
    // Implementation of loadstring
    int loadstring(lua_State* L);
}

// Instance-related conventions
namespace Instances {
    // Implementation of fireclickdetector
    int fireclickdetector(lua_State* L);
    
    // Implementation of firetouchinterest
    int firetouchinterest(lua_State* L);
    
    // Implementation of fireproximityprompt
    int fireproximityprompt(lua_State* L);
    
    // Implementation of firesignal
    int firesignal(lua_State* L);
    
    // Implementation of getcallbackvalue
    int getcallbackvalue(lua_State* L);
    
    // Implementation of getconnections
    int getconnections(lua_State* L);
    
    // Implementation of getcustomasset
    int getcustomasset(lua_State* L);
    
    // Implementation of gethiddenproperty
    int gethiddenproperty(lua_State* L);
    
    // Implementation of gethui
    int gethui(lua_State* L);
    
    // Implementation of getinstances
    int getinstances(lua_State* L);
    
    // Implementation of getnilinstances
    int getnilinstances(lua_State* L);
    
    // Implementation of isscriptable
    int isscriptable(lua_State* L);
    
    // Implementation of sethiddenproperty
    int sethiddenproperty(lua_State* L);
    
    // Implementation of setrbxclipboard
    int setrbxclipboard(lua_State* L);
    
    // Implementation of setscriptable
    int setscriptable(lua_State* L);
    
    // Implementation of getplayer
    int getplayer(lua_State* L);
    
    // Implementation of getlocalplayer
    int getlocalplayer(lua_State* L);
    
    // Implementation of getplayers
    int getplayers(lua_State* L);
    
    // Implementation of runanimation
    int runanimation(lua_State* L);
}

// Metatable-related conventions
namespace Metatables {
    // Implementation of getrawmetatable
    int getrawmetatable(lua_State* L);
    
    // Implementation of hookmetamethod
    int hookmetamethod(lua_State* L);
    
    // Implementation of getnamecallmethod
    int getnamecallmethod(lua_State* L);
    
    // Implementation of isreadonly
    int isreadonly(lua_State* L);
    
    // Implementation of setrawmetatable
    int setrawmetatable(lua_State* L);
    
    // Implementation of setreadonly
    int setreadonly(lua_State* L);
}

// Miscellaneous conventions
namespace Misc {
    // Implementation of identifyexecutor
    int identifyexecutor(lua_State* L);
    
    // Implementation of lz4compress
    int lz4compress(lua_State* L);
    
    // Implementation of lz4decompress
    int lz4decompress(lua_State* L);
    
    // Implementation of messagebox
    int messagebox(lua_State* L);
    
    // Implementation of queue_on_teleport
    int queue_on_teleport(lua_State* L);
    
    // Implementation of request
    int request(lua_State* L);
    
    // Implementation of setclipboard
    int setclipboard(lua_State* L);
    
    // Implementation of setfpscap
    int setfpscap(lua_State* L);
    
    // Implementation of join
    int join(lua_State* L);
    
    // Implementation of gethwid
    int gethwid(lua_State* L);
}

// Script-related conventions
namespace Scripts {
    // Implementation of getgc
    int getgc(lua_State* L);
    
    // Implementation of getgenv
    int getgenv(lua_State* L);
    
    // Implementation of getloadedmodules
    int getloadedmodules(lua_State* L);
    
    // Implementation of getrenv
    int getrenv(lua_State* L);
    
    // Implementation of getrunningscripts
    int getrunningscripts(lua_State* L);
    
    // Implementation of getscriptbytecode
    int getscriptbytecode(lua_State* L);
    
    // Implementation of getscripthash
    int getscripthash(lua_State* L);
    
    // Implementation of getscripts
    int getscripts(lua_State* L);
    
    // Implementation of getsenv
    int getsenv(lua_State* L);
    
    // Implementation of getthreadidentity
    int getthreadidentity(lua_State* L);
    
    // Implementation of setthreadidentity
    int setthreadidentity(lua_State* L);
}

// Drawing-related conventions
namespace Drawing {
    // Implementation of Drawing.new
    int new_drawing(lua_State* L);
    
    // Implementation of isrenderobj
    int isrenderobj(lua_State* L);
    
    // Implementation of cleardrawcache
    int cleardrawcache(lua_State* L);
}

// WebSocket-related conventions
namespace WebSocket {
    // Implementation of WebSocket.connect
    int connect(lua_State* L);
}

// Debug-related conventions
namespace Debug {
    // Implementation of debug functions
    int debug_functions(lua_State* L);
}

// Crypt-related conventions
namespace Crypt {
    // Implementation of crypt functions
    int crypt_functions(lua_State* L);
}

// Filesystem-related conventions
namespace Filesystem {
    // Implementation of filesystem functions
    int filesystem_functions(lua_State* L);
}

// Input-related conventions
namespace Input {
    // Implementation of input functions
    int input_functions(lua_State* L);
}

} // namespace NamingConventions
