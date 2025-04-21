#pragma once

#include <string>
#include <unordered_map>
#include <vector>
#include <functional>
#include "../globals.hpp"
#include "../luau/lua.h"

// Class to handle different naming conventions for Roblox executor functions
class NamingConventionMapper {
private:
    // Map of function aliases to their standard names
    std::unordered_map<std::string, std::string> m_aliasMap;
    
    // Singleton instance
    static NamingConventionMapper* s_instance;
    
    // Private constructor for singleton pattern
    NamingConventionMapper() {
        InitializeAliasMap();
    }
    
    // Initialize the alias map with all known naming conventions
    void InitializeAliasMap() {
        // UNC Standard Naming Conventions
        
        // Cache functions
        AddAlias("cache.invalidate", "cache_invalidate");
        AddAlias("cache.iscached", "cache_iscached");
        AddAlias("cache.replace", "cache_replace");
        
        // Closure functions
        AddAlias("checkcaller", "checkcaller");
        AddAlias("clonefunction", "clonefunction");
        AddAlias("getcallingscript", "getcallingscript");
        AddAlias("getscriptclosure", "getscriptclosure");
        AddAlias("getscriptfunction", "getscriptclosure"); // Alias
        AddAlias("hookfunction", "hookfunction");
        AddAlias("replaceclosure", "hookfunction"); // Alias
        AddAlias("closuretype", "closuretype");
        AddAlias("iscclosure", "iscclosure");
        AddAlias("islclosure", "islclosure");
        AddAlias("isexecutorclosure", "isexecutorclosure");
        AddAlias("checkclosure", "isexecutorclosure"); // Alias
        AddAlias("isourclosure", "isexecutorclosure"); // Alias
        AddAlias("loadstring", "loadstring");
        AddAlias("newcclosure", "newcclosure");
        AddAlias("newlclosure", "newlclosure");
        
        // Debug functions
        AddAlias("debug.getconstant", "debug_getconstant");
        AddAlias("debug.getconstants", "debug_getconstants");
        AddAlias("debug.getinfo", "debug_getinfo");
        AddAlias("debug.getproto", "debug_getproto");
        AddAlias("debug.getprotos", "debug_getprotos");
        AddAlias("debug.getstack", "debug_getstack");
        AddAlias("debug.getupvalue", "debug_getupvalue");
        AddAlias("debug.getupvalues", "debug_getupvalues");
        AddAlias("debug.print", "debug_print");
        AddAlias("debug.setconstant", "debug_setconstant");
        AddAlias("debug.setstack", "debug_setstack");
        AddAlias("debug.setupvalue", "debug_setupvalue");
        
        // Drawing functions
        AddAlias("Drawing", "Drawing");
        AddAlias("Drawing.new", "Drawing_new");
        AddAlias("Drawing.Fonts", "Drawing_Fonts");
        AddAlias("isrenderobj", "isrenderobj");
        AddAlias("cleardrawcache", "cleardrawcache");
        
        // Filesystem functions
        AddAlias("readfile", "readfile");
        AddAlias("listfiles", "listfiles");
        AddAlias("writefile", "writefile");
        AddAlias("makefolder", "makefolder");
        AddAlias("appendfile", "appendfile");
        AddAlias("isfile", "isfile");
        AddAlias("isfolder", "isfolder");
        AddAlias("delfile", "delfile");
        AddAlias("delfolder", "delfolder");
        AddAlias("loadfile", "loadfile");
        AddAlias("dofile", "dofile");
        
        // Input functions
        AddAlias("mouse1click", "mouse1click");
        AddAlias("mouse1press", "mouse1press");
        AddAlias("mouse1release", "mouse1release");
        AddAlias("mouse2click", "mouse2click");
        AddAlias("mouse2press", "mouse2press");
        AddAlias("mouse2release", "mouse2release");
        AddAlias("mousemoveabs", "mousemoveabs");
        AddAlias("mousemoverel", "mousemoverel");
        AddAlias("mousescroll", "mousescroll");
        
        // Instance functions
        AddAlias("fireclickdetector", "fireclickdetector");
        AddAlias("fireproximityprompt", "fireproximityprompt");
        AddAlias("firesignal", "firesignal");
        AddAlias("firetouchinterest", "firetouchinterest");
        AddAlias("getcallbackvalue", "getcallbackvalue");
        AddAlias("getconnections", "getconnections");
        AddAlias("getcustomasset", "getcustomasset");
        AddAlias("gethiddenproperty", "gethiddenproperty");
        AddAlias("gethui", "gethui");
        AddAlias("getinstances", "getinstances");
        AddAlias("getnilinstances", "getnilinstances");
        AddAlias("isrbxactive", "isrbxactive");
        AddAlias("isscriptable", "isscriptable");
        AddAlias("sethiddenproperty", "sethiddenproperty");
        AddAlias("setscriptable", "setscriptable");
        
        // Metatable functions
        AddAlias("getrawmetatable", "getrawmetatable");
        AddAlias("hookmetamethod", "hookmetamethod");
        AddAlias("getnamecallmethod", "getnamecallmethod");
        AddAlias("isreadonly", "isreadonly");
        AddAlias("setrawmetatable", "setrawmetatable");
        AddAlias("setreadonly", "setreadonly");
        
        // Miscellaneous functions
        AddAlias("identifyexecutor", "identifyexecutor");
        AddAlias("getexecutorname", "identifyexecutor"); // Alias
        AddAlias("lz4compress", "lz4compress");
        AddAlias("lz4decompress", "lz4decompress");
        AddAlias("messagebox", "messagebox");
        AddAlias("queue_on_teleport", "queue_on_teleport");
        AddAlias("queueonteleport", "queue_on_teleport"); // Alias
        AddAlias("request", "request");
        AddAlias("http.request", "request"); // Alias
        AddAlias("http_request", "request"); // Alias
        AddAlias("setclipboard", "setclipboard");
        AddAlias("toclipboard", "setclipboard"); // Alias
        AddAlias("setfpscap", "setfpscap");
        AddAlias("join", "join");
        AddAlias("joingame", "join"); // Alias
        AddAlias("joinplace", "join"); // Alias
        AddAlias("joinserver", "join"); // Alias
        AddAlias("gethwid", "gethwid");
        
        // Script functions
        AddAlias("getgc", "getgc");
        AddAlias("getgenv", "getgenv");
        AddAlias("getloadedmodules", "getloadedmodules");
        AddAlias("getrenv", "getrenv");
        AddAlias("getrunningscripts", "getrunningscripts");
        AddAlias("getscriptbytecode", "getscriptbytecode");
        AddAlias("dumpstring", "getscriptbytecode"); // Alias
        AddAlias("getscripthash", "getscripthash");
        AddAlias("getscripts", "getscripts");
        AddAlias("getsenv", "getsenv");
        AddAlias("getthreadidentity", "getthreadidentity");
        AddAlias("getidentity", "getthreadidentity"); // Alias
        AddAlias("getthreadcontext", "getthreadidentity"); // Alias
        AddAlias("setthreadidentity", "setthreadidentity");
        AddAlias("setidentity", "setthreadidentity"); // Alias
        AddAlias("setthreadcontext", "setthreadidentity"); // Alias
        
        // WebSocket functions
        AddAlias("WebSocket", "WebSocket");
        AddAlias("WebSocket.connect", "WebSocket_connect");
        
        // Cryptography functions
        AddAlias("crypt.base64decode", "crypt_base64decode");
        AddAlias("crypt.base64encode", "crypt_base64encode");
        AddAlias("crypt.decrypt", "crypt_decrypt");
        AddAlias("crypt.encrypt", "crypt_encrypt");
        AddAlias("crypt.generatebytes", "crypt_generatebytes");
        AddAlias("crypt.generatekey", "crypt_generatekey");
        AddAlias("crypt.hash", "crypt_hash");
        
        // Player functions
        AddAlias("getplayer", "getplayer");
        AddAlias("getlocalplayer", "getlocalplayer");
        AddAlias("getplayers", "getplayers");
        AddAlias("runanimation", "runanimation");
        AddAlias("playanimation", "runanimation"); // Alias
        
        // Other executor-specific naming conventions
        
        // Synapse X naming conventions
        AddAlias("syn.cache_invalidate", "cache_invalidate");
        AddAlias("syn.cache_replace", "cache_replace");
        AddAlias("syn.is_cached", "cache_iscached");
        AddAlias("syn.set_thread_identity", "setthreadidentity");
        AddAlias("syn.get_thread_identity", "getthreadidentity");
        AddAlias("syn.write_clipboard", "setclipboard");
        AddAlias("syn.queue_on_teleport", "queue_on_teleport");
        AddAlias("syn.protect_gui", "gethui");
        AddAlias("syn.unprotect_gui", "gethui");
        AddAlias("syn.secure_call", "checkcaller");
        AddAlias("syn.is_beta", "identifyexecutor");
        AddAlias("syn.request", "request");
        
        // KRNL naming conventions
        AddAlias("krnl_loadstring", "loadstring");
        AddAlias("krnl_setclipboard", "setclipboard");
        AddAlias("krnl_request", "request");
        AddAlias("krnl_isrbxactive", "isrbxactive");
        AddAlias("krnl_sethiddenproperty", "sethiddenproperty");
        AddAlias("krnl_gethiddenproperty", "gethiddenproperty");
        
        // Script-Ware naming conventions
        AddAlias("getexecutor", "identifyexecutor");
        AddAlias("getscriptfromthread", "getcallingscript");
        AddAlias("getscriptfunction", "getscriptclosure");
        AddAlias("getthreadscript", "getcallingscript");
        AddAlias("isluau", "identifyexecutor");
        AddAlias("iswindowactive", "isrbxactive");
        AddAlias("setfpscap", "setfpscap");
        
        // Sentinel naming conventions
        AddAlias("sentinel_isrbxactive", "isrbxactive");
        AddAlias("sentinel_newcclosure", "newcclosure");
        AddAlias("sentinel_checkcaller", "checkcaller");
        AddAlias("sentinel_getthreadidentity", "getthreadidentity");
        AddAlias("sentinel_setthreadidentity", "setthreadidentity");
        
        // Fluxus naming conventions
        AddAlias("fluxus_request", "request");
        AddAlias("fluxus_loadstring", "loadstring");
        AddAlias("fluxus_setclipboard", "setclipboard");
        
        // Electron naming conventions
        AddAlias("electron_loadstring", "loadstring");
        AddAlias("electron_request", "request");
        AddAlias("electron_setclipboard", "setclipboard");
        
        // Calamari naming conventions
        AddAlias("calamari_request", "request");
        AddAlias("calamari_loadstring", "loadstring");
    }
    
    // Helper to add an alias to the map
    void AddAlias(const std::string& alias, const std::string& standardName) {
        m_aliasMap[alias] = standardName;
    }
    
public:
    // Get singleton instance
    static NamingConventionMapper* GetInstance() {
        if (!s_instance) {
            s_instance = new NamingConventionMapper();
        }
        return s_instance;
    }
    
    // Get the standard name for a given alias
    std::string GetStandardName(const std::string& alias) {
        auto it = m_aliasMap.find(alias);
        if (it != m_aliasMap.end()) {
            return it->second;
        }
        return alias; // Return the original name if no mapping exists
    }
    
    // Check if a name is an alias
    bool IsAlias(const std::string& name) {
        return m_aliasMap.find(name) != m_aliasMap.end();
    }
    
    // Get all aliases for a standard name
    std::vector<std::string> GetAliasesForStandard(const std::string& standardName) {
        std::vector<std::string> aliases;
        for (const auto& pair : m_aliasMap) {
            if (pair.second == standardName) {
                aliases.push_back(pair.first);
            }
        }
        return aliases;
    }
    
    // Register all naming conventions in the Lua state
    void RegisterAllNamingConventions(lua_State* L) {
        // Group aliases by their standard function
        std::unordered_map<std::string, std::vector<std::string>> standardToAliases;
        
        for (const auto& pair : m_aliasMap) {
            standardToAliases[pair.second].push_back(pair.first);
        }
        
        // For each standard function, create aliases that point to it
        for (const auto& pair : standardToAliases) {
            const std::string& standardName = pair.first;
            const std::vector<std::string>& aliases = pair.second;
            
            // Get the standard function from the global table
            lua_getglobal(L, standardName.c_str());
            
            // If the standard function exists, create aliases for it
            if (!lua_isnil(L, -1)) {
                for (const auto& alias : aliases) {
                    // Skip if the alias is the same as the standard name
                    if (alias == standardName) continue;
                    
                    // Create the alias
                    lua_pushvalue(L, -1); // Push a copy of the standard function
                    lua_setglobal(L, alias.c_str()); // Set it as a global with the alias name
                }
            }
            
            // Pop the standard function
            lua_pop(L, 1);
        }
    }
    
    // Clean up
    ~NamingConventionMapper() {
        // Nothing to clean up
    }
};

// Initialize static member
NamingConventionMapper* NamingConventionMapper::s_instance = nullptr;

// Function to register all naming conventions
void RegisterAllNamingConventions(lua_State* L) {
    NamingConventionMapper::GetInstance()->RegisterAllNamingConventions(L);
}
