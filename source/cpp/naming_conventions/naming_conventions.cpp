#include "naming_conventions.h"
#include <iostream>
#include <algorithm>

namespace RobloxExecutor {
namespace NamingConventions {

// Singleton instance implementation
NamingConventionManager& NamingConventionManager::GetInstance() {
    static NamingConventionManager instance;
    return instance;
}

// Constructor
NamingConventionManager::NamingConventionManager()
    : m_enableUNC(true),
      m_enableSNC(true),
      m_enableCustom(true),
      m_initialized(false) {
}

// Initialize the naming convention manager
bool NamingConventionManager::Initialize(bool enableUNC, bool enableSNC) {
    if (m_initialized) {
        std::cout << "NamingConventionManager: Already initialized" << std::endl;
        return true;
    }
    
    m_enableUNC = enableUNC;
    m_enableSNC = enableSNC;
    
    // Initialize naming conventions
    if (m_enableUNC) {
        InitializeUNC();
    }
    
    if (m_enableSNC) {
        InitializeSNC();
    }
    
    m_initialized = true;
    std::cout << "NamingConventionManager: Initialized with UNC=" 
              << (m_enableUNC ? "enabled" : "disabled")
              << ", SNC=" << (m_enableSNC ? "enabled" : "disabled") << std::endl;
    
    return true;
}

// Register a function alias
bool NamingConventionManager::RegisterAlias(const std::string& originalName, 
                                          const std::string& aliasName,
                                          ConventionType convention, 
                                          const std::string& description) {
    // Check if the alias already exists
    auto it = m_aliasMap.find(aliasName);
    if (it != m_aliasMap.end()) {
        // Alias already exists, check if it points to the same original function
        if (it->second != originalName) {
            std::cerr << "NamingConventionManager: Alias '" << aliasName 
                      << "' already registered for function '" << it->second 
                      << "', cannot register for '" << originalName << "'" << std::endl;
            return false;
        }
        
        // Alias already registered for this function, nothing to do
        return true;
    }
    
    // Register the alias
    m_aliasMap[aliasName] = originalName;
    
    // Add to original-to-aliases map
    FunctionAlias alias(originalName, aliasName, convention, description);
    m_originalToAliases[originalName].push_back(alias);
    
    return true;
}

// Register multiple aliases for a function
bool NamingConventionManager::RegisterAliases(const std::string& originalName, 
                                            const std::vector<std::string>& aliases,
                                            ConventionType convention,
                                            const std::string& description) {
    bool success = true;
    
    for (const auto& alias : aliases) {
        if (!RegisterAlias(originalName, alias, convention, description)) {
            success = false;
        }
    }
    
    return success;
}

// Resolve a function name to its original name
std::string NamingConventionManager::ResolveFunction(const std::string& functionName) const {
    // Check if the function name is an alias
    auto it = m_aliasMap.find(functionName);
    if (it != m_aliasMap.end()) {
        return it->second;
    }
    
    // Not an alias, return the original name
    return functionName;
}

// Check if a function name is an alias
bool NamingConventionManager::IsAlias(const std::string& functionName) const {
    return m_aliasMap.find(functionName) != m_aliasMap.end();
}

// Get all aliases for a function
std::vector<FunctionAlias> NamingConventionManager::GetAliases(const std::string& originalName) const {
    auto it = m_originalToAliases.find(originalName);
    if (it != m_originalToAliases.end()) {
        return it->second;
    }
    
    return std::vector<FunctionAlias>();
}

// Get all function aliases
std::vector<FunctionAlias> NamingConventionManager::GetAllAliases() const {
    std::vector<FunctionAlias> allAliases;
    
    for (const auto& pair : m_originalToAliases) {
        allAliases.insert(allAliases.end(), pair.second.begin(), pair.second.end());
    }
    
    return allAliases;
}

// Enable or disable a naming convention
void NamingConventionManager::EnableConvention(ConventionType convention, bool enable) {
    switch (convention) {
        case ConventionType::UNC:
            m_enableUNC = enable;
            break;
        case ConventionType::SNC:
            m_enableSNC = enable;
            break;
        case ConventionType::Custom:
            m_enableCustom = enable;
            break;
    }
}

// Check if a naming convention is enabled
bool NamingConventionManager::IsConventionEnabled(ConventionType convention) const {
    switch (convention) {
        case ConventionType::UNC:
            return m_enableUNC;
        case ConventionType::SNC:
            return m_enableSNC;
        case ConventionType::Custom:
            return m_enableCustom;
        default:
            return false;
    }
}

// Initialize UNC naming convention
void NamingConventionManager::InitializeUNC() {
    std::cout << "NamingConventionManager: Initializing UNC naming convention" << std::endl;
    
    // Cache functions
    RegisterAlias("cloneref", "cache.replace", ConventionType::UNC, "Replace an instance reference with another");
    RegisterAlias("cache.invalidate", "cache.invalidate", ConventionType::UNC, "Invalidate an instance in the cache");
    RegisterAlias("cache.iscached", "cache.iscached", ConventionType::UNC, "Check if an instance is cached");
    RegisterAlias("cloneref", "cloneref", ConventionType::UNC, "Clone an instance reference");
    RegisterAlias("compareinstances", "compareinstances", ConventionType::UNC, "Compare two instances for equality");
    
    // Closure functions
    RegisterAlias("checkcaller", "checkcaller", ConventionType::UNC, "Check if the caller is from the executor");
    RegisterAlias("clonefunction", "clonefunction", ConventionType::UNC, "Clone a function");
    RegisterAlias("getcallingscript", "getcallingscript", ConventionType::UNC, "Get the script that called the current function");
    RegisterAlias("getscriptclosure", "getscriptclosure", ConventionType::UNC, "Get the closure of a script");
    RegisterAliases("getscriptclosure", {"getscriptfunction"}, ConventionType::UNC, "Get the closure of a script");
    RegisterAlias("hookfunction", "hookfunction", ConventionType::UNC, "Hook a function");
    RegisterAliases("hookfunction", {"replaceclosure"}, ConventionType::UNC, "Hook a function");
    RegisterAlias("closuretype", "closuretype", ConventionType::UNC, "Get the type of a closure");
    RegisterAlias("iscclosure", "iscclosure", ConventionType::UNC, "Check if a function is a C closure");
    RegisterAlias("islclosure", "islclosure", ConventionType::UNC, "Check if a function is a Lua closure");
    RegisterAlias("isexecutorclosure", "isexecutorclosure", ConventionType::UNC, "Check if a function is an executor closure");
    RegisterAliases("isexecutorclosure", {"checkclosure", "isourclosure"}, ConventionType::UNC, "Check if a function is an executor closure");
    RegisterAlias("loadstring", "loadstring", ConventionType::UNC, "Load a string as a function");
    
    // Metatable functions
    RegisterAlias("getrawmetatable", "getrawmetatable", ConventionType::UNC, "Get the raw metatable of an object");
    RegisterAlias("hookmetamethod", "hookmetamethod", ConventionType::UNC, "Hook a metamethod");
    RegisterAlias("getnamecallmethod", "getnamecallmethod", ConventionType::UNC, "Get the name of the method being called");
    RegisterAlias("isreadonly", "isreadonly", ConventionType::UNC, "Check if a table is read-only");
    RegisterAlias("setrawmetatable", "setrawmetatable", ConventionType::UNC, "Set the raw metatable of an object");
    RegisterAlias("setreadonly", "setreadonly", ConventionType::UNC, "Set whether a table is read-only");
    
    // Miscellaneous functions
    RegisterAlias("identifyexecutor", "identifyexecutor", ConventionType::UNC, "Identify the executor");
    RegisterAliases("identifyexecutor", {"getexecutorname"}, ConventionType::UNC, "Identify the executor");
    RegisterAlias("lz4compress", "lz4compress", ConventionType::UNC, "Compress data using LZ4");
    RegisterAlias("lz4decompress", "lz4decompress", ConventionType::UNC, "Decompress data using LZ4");
    RegisterAlias("messagebox", "messagebox", ConventionType::UNC, "Display a message box");
    RegisterAlias("queue_on_teleport", "queue_on_teleport", ConventionType::UNC, "Queue a script to run after teleporting");
    RegisterAliases("queue_on_teleport", {"queueonteleport"}, ConventionType::UNC, "Queue a script to run after teleporting");
    RegisterAlias("request", "request", ConventionType::UNC, "Send an HTTP request");
    RegisterAliases("request", {"http.request", "http_request"}, ConventionType::UNC, "Send an HTTP request");
    RegisterAlias("setclipboard", "setclipboard", ConventionType::UNC, "Set the clipboard content");
    RegisterAliases("setclipboard", {"toclipboard"}, ConventionType::UNC, "Set the clipboard content");
    RegisterAlias("setfpscap", "setfpscap", ConventionType::UNC, "Set the FPS cap");
    RegisterAlias("join", "join", ConventionType::UNC, "Join a game");
    RegisterAliases("join", {"joingame", "joinplace", "joinserver"}, ConventionType::UNC, "Join a game");
    RegisterAlias("gethwid", "gethwid", ConventionType::UNC, "Get the hardware ID");
    
    // Script functions
    RegisterAlias("getgc", "getgc", ConventionType::UNC, "Get the garbage collector");
    RegisterAlias("getgenv", "getgenv", ConventionType::UNC, "Get the global environment");
    RegisterAlias("getloadedmodules", "getloadedmodules", ConventionType::UNC, "Get loaded modules");
    RegisterAlias("getrenv", "getrenv", ConventionType::UNC, "Get the Roblox environment");
    RegisterAlias("getrunningscripts", "getrunningscripts", ConventionType::UNC, "Get running scripts");
    RegisterAlias("getscriptbytecode", "getscriptbytecode", ConventionType::UNC, "Get the bytecode of a script");
    RegisterAliases("getscriptbytecode", {"dumpstring"}, ConventionType::UNC, "Get the bytecode of a script");
    RegisterAlias("getscripthash", "getscripthash", ConventionType::UNC, "Get the hash of a script");
    RegisterAlias("getscripts", "getscripts", ConventionType::UNC, "Get all scripts");
    RegisterAlias("getsenv", "getsenv", ConventionType::UNC, "Get the environment of a script");
    RegisterAlias("getthreadidentity", "getthreadidentity", ConventionType::UNC, "Get the identity of the current thread");
    RegisterAliases("getthreadidentity", {"getidentity", "getthreadcontext"}, ConventionType::UNC, "Get the identity of the current thread");
    RegisterAlias("setthreadidentity", "setthreadidentity", ConventionType::UNC, "Set the identity of the current thread");
    RegisterAliases("setthreadidentity", {"setidentity", "setthreadcontext"}, ConventionType::UNC, "Set the identity of the current thread");
    
    // Drawing functions
    RegisterAlias("Drawing", "Drawing", ConventionType::UNC, "Drawing library");
    RegisterAlias("Drawing.new", "Drawing.new", ConventionType::UNC, "Create a new drawing object");
    RegisterAlias("Drawing.Fonts", "Drawing.Fonts", ConventionType::UNC, "Drawing fonts");
    RegisterAlias("isrenderobj", "isrenderobj", ConventionType::UNC, "Check if an object is a render object");
    RegisterAlias("cleardrawcache", "cleardrawcache", ConventionType::UNC, "Clear the drawing cache");
    
    // WebSocket functions
    RegisterAlias("WebSocket", "WebSocket", ConventionType::UNC, "WebSocket library");
    RegisterAlias("WebSocket.connect", "WebSocket.connect", ConventionType::UNC, "Connect to a WebSocket server");
    
    // Cryptography functions
    RegisterAlias("crypt.base64decode", "crypt.base64decode", ConventionType::UNC, "Decode base64 data");
    RegisterAliases("crypt.base64decode", {"crypt.base64.decode", "crypt.base64_decode", "base64.decode", "base64_decode"}, ConventionType::UNC, "Decode base64 data");
    RegisterAlias("crypt.base64encode", "crypt.base64encode", ConventionType::UNC, "Encode data as base64");
    RegisterAliases("crypt.base64encode", {"crypt.base64.encode", "crypt.base64_encode", "base64.encode", "base64_encode"}, ConventionType::UNC, "Encode data as base64");
    RegisterAlias("crypt.decrypt", "crypt.decrypt", ConventionType::UNC, "Decrypt data");
    RegisterAlias("crypt.encrypt", "crypt.encrypt", ConventionType::UNC, "Encrypt data");
    RegisterAlias("crypt.generatebytes", "crypt.generatebytes", ConventionType::UNC, "Generate random bytes");
    RegisterAlias("crypt.generatekey", "crypt.generatekey", ConventionType::UNC, "Generate a cryptographic key");
    RegisterAlias("crypt.hash", "crypt.hash", ConventionType::UNC, "Hash data");

    // Debug functions
    RegisterAlias("debug.getconstant", "debug.getconstant", ConventionType::UNC, "Get a constant from a function");
    RegisterAlias("debug.getconstants", "debug.getconstants", ConventionType::UNC, "Get all constants from a function");
    RegisterAlias("debug.getinfo", "debug.getinfo", ConventionType::UNC, "Get information about a function");
    RegisterAlias("debug.getproto", "debug.getproto", ConventionType::UNC, "Get a proto from a function");
    RegisterAlias("debug.getprotos", "debug.getprotos", ConventionType::UNC, "Get all protos from a function");
    RegisterAlias("debug.getstack", "debug.getstack", ConventionType::UNC, "Get the stack of a thread");
    RegisterAlias("debug.getupvalue", "debug.getupvalue", ConventionType::UNC, "Get an upvalue from a function");
    RegisterAlias("debug.getupvalues", "debug.getupvalues", ConventionType::UNC, "Get all upvalues from a function");
    RegisterAlias("debug.print", "debug.print", ConventionType::UNC, "Print debug information");
    RegisterAlias("debug.setconstant", "debug.setconstant", ConventionType::UNC, "Set a constant in a function");
    RegisterAlias("debug.setstack", "debug.setstack", ConventionType::UNC, "Set a value in the stack");
    RegisterAlias("debug.setupvalue", "debug.setupvalue", ConventionType::UNC, "Set an upvalue in a function");

    // File system functions
    RegisterAlias("appendfile", "appendfile", ConventionType::UNC, "Append to a file");
    RegisterAlias("delfile", "delfile", ConventionType::UNC, "Delete a file");
    RegisterAlias("delfolder", "delfolder", ConventionType::UNC, "Delete a folder");
    RegisterAlias("dofile", "dofile", ConventionType::UNC, "Execute a file");
    RegisterAlias("isfile", "isfile", ConventionType::UNC, "Check if a file exists");
    RegisterAlias("isfolder", "isfolder", ConventionType::UNC, "Check if a folder exists");
    RegisterAlias("listfiles", "listfiles", ConventionType::UNC, "List files in a folder");
    RegisterAlias("loadfile", "loadfile", ConventionType::UNC, "Load a file as a function");
    RegisterAlias("makefolder", "makefolder", ConventionType::UNC, "Create a folder");
    RegisterAlias("readfile", "readfile", ConventionType::UNC, "Read a file");
    RegisterAlias("writefile", "writefile", ConventionType::UNC, "Write to a file");

    // Instance interaction functions
    RegisterAlias("fireclickdetector", "fireclickdetector", ConventionType::UNC, "Fire a click detector");
    RegisterAlias("fireproximityprompt", "fireproximityprompt", ConventionType::UNC, "Fire a proximity prompt");
    RegisterAlias("firesignal", "firesignal", ConventionType::UNC, "Fire a signal");
    RegisterAlias("firetouchinterest", "firetouchinterest", ConventionType::UNC, "Fire a touch interest");
    RegisterAlias("getcallbackvalue", "getcallbackvalue", ConventionType::UNC, "Get a callback value");
    RegisterAlias("getconnections", "getconnections", ConventionType::UNC, "Get connections from a signal");
    RegisterAlias("getcustomasset", "getcustomasset", ConventionType::UNC, "Get a custom asset");
    RegisterAlias("gethiddenproperty", "gethiddenproperty", ConventionType::UNC, "Get a hidden property");
    RegisterAlias("gethui", "gethui", ConventionType::UNC, "Get the hidden UI");
    RegisterAlias("getinstances", "getinstances", ConventionType::UNC, "Get all instances");
    RegisterAlias("getnilinstances", "getnilinstances", ConventionType::UNC, "Get nil instances");
    RegisterAlias("isrbxactive", "isrbxactive", ConventionType::UNC, "Check if Roblox is active");
    RegisterAlias("sethiddenproperty", "sethiddenproperty", ConventionType::UNC, "Set a hidden property");

    // Mouse input functions
    RegisterAlias("mouse1click", "mouse1click", ConventionType::UNC, "Simulate a left mouse click");
    RegisterAlias("mouse1press", "mouse1press", ConventionType::UNC, "Simulate a left mouse press");
    RegisterAlias("mouse1release", "mouse1release", ConventionType::UNC, "Simulate a left mouse release");
    RegisterAlias("mouse2click", "mouse2click", ConventionType::UNC, "Simulate a right mouse click");
    RegisterAlias("mouse2press", "mouse2press", ConventionType::UNC, "Simulate a right mouse press");
    RegisterAlias("mouse2release", "mouse2release", ConventionType::UNC, "Simulate a right mouse release");
    RegisterAlias("mousemoveabs", "mousemoveabs", ConventionType::UNC, "Move the mouse to absolute coordinates");
    RegisterAlias("mousemoverel", "mousemoverel", ConventionType::UNC, "Move the mouse by relative coordinates");
    RegisterAlias("mousescroll", "mousescroll", ConventionType::UNC, "Simulate mouse scrolling");
    
    // Player functions
    RegisterAlias("getplayer", "getplayer", ConventionType::UNC, "Get a player");
    RegisterAlias("getlocalplayer", "getlocalplayer", ConventionType::UNC, "Get the local player");
    RegisterAlias("getplayers", "getplayers", ConventionType::UNC, "Get all players");
    RegisterAlias("runanimation", "runanimation", ConventionType::UNC, "Run an animation");
    RegisterAliases("runanimation", {"playanimation"}, ConventionType::UNC, "Run an animation");
    
    // SNC-specific aliases
    RegisterAlias("is_salad_closure", "isexecutorclosure", ConventionType::UNC, "Check if a function is a Salad closure");
    RegisterAlias("is_essence_closure", "isexecutorclosure", ConventionType::UNC, "Check if a function is an Essence closure");
    RegisterAlias("is_ronix_closure", "isexecutorclosure", ConventionType::UNC, "Check if a function is a Ronix closure");
    RegisterAlias("is_awp_closure", "isexecutorclosure", ConventionType::UNC, "Check if a function is an AWP closure");
    RegisterAlias("is_wave_closure", "isexecutorclosure", ConventionType::UNC, "Check if a function is a Wave closure");
}

// Initialize SNC naming convention
void NamingConventionManager::InitializeSNC() {
    std::cout << "NamingConventionManager: Initializing SNC naming convention" << std::endl;
    
    // Cache functions
    RegisterAlias("cloneref", "cache.replace", ConventionType::SNC, "Replace an instance reference with another");
    RegisterAlias("cache.invalidate", "cache.invalidate", ConventionType::SNC, "Invalidate an instance in the cache");
    RegisterAlias("cache.iscached", "cache.iscached", ConventionType::SNC, "Check if an instance is cached");
    RegisterAlias("cloneref", "cloneref", ConventionType::SNC, "Clone an instance reference");
    RegisterAlias("compareinstances", "compareinstances", ConventionType::SNC, "Compare two instances for equality");
    
    // Closure functions
    RegisterAlias("checkcaller", "checkcaller", ConventionType::SNC, "Check if the caller is from the executor");
    RegisterAlias("clonefunction", "clonefunction", ConventionType::SNC, "Clone a function");
    RegisterAlias("getcallingscript", "getcallingscript", ConventionType::SNC, "Get the script that called the current function");
    RegisterAlias("getscriptclosure", "getscriptclosure", ConventionType::SNC, "Get the closure of a script");
    RegisterAliases("getscriptclosure", {"getscriptfunction"}, ConventionType::SNC, "Get the closure of a script");
    RegisterAlias("hookfunction", "hookfunction", ConventionType::SNC, "Hook a function");
    RegisterAliases("hookfunction", {"replaceclosure"}, ConventionType::SNC, "Hook a function");
    RegisterAlias("closuretype", "closuretype", ConventionType::SNC, "Get the type of a closure");
    RegisterAlias("iscclosure", "iscclosure", ConventionType::SNC, "Check if a function is a C closure");
    RegisterAlias("islclosure", "islclosure", ConventionType::SNC, "Check if a function is a Lua closure");
    RegisterAlias("isexecutorclosure", "isexecutorclosure", ConventionType::SNC, "Check if a function is an executor closure");
    RegisterAliases("isexecutorclosure", {"checkclosure", "isourclosure"}, ConventionType::SNC, "Check if a function is an executor closure");
    RegisterAlias("loadstring", "loadstring", ConventionType::SNC, "Load a string as a function");
    
    // Metatable functions
    RegisterAlias("getrawmetatable", "getrawmetatable", ConventionType::SNC, "Get the raw metatable of an object");
    RegisterAlias("hookmetamethod", "hookmetamethod", ConventionType::SNC, "Hook a metamethod");
    RegisterAlias("getnamecallmethod", "getnamecallmethod", ConventionType::SNC, "Get the name of the method being called");
    RegisterAlias("isreadonly", "isreadonly", ConventionType::SNC, "Check if a table is read-only");
    RegisterAlias("setrawmetatable", "setrawmetatable", ConventionType::SNC, "Set the raw metatable of an object");
    RegisterAlias("setreadonly", "setreadonly", ConventionType::SNC, "Set whether a table is read-only");
    
    // Miscellaneous functions
    RegisterAlias("identifyexecutor", "identifyexecutor", ConventionType::SNC, "Identify the executor");
    RegisterAliases("identifyexecutor", {"getexecutorname"}, ConventionType::SNC, "Identify the executor");
    RegisterAlias("lz4compress", "lz4compress", ConventionType::SNC, "Compress data using LZ4");
    RegisterAlias("lz4decompress", "lz4decompress", ConventionType::SNC, "Decompress data using LZ4");
    RegisterAlias("messagebox", "messagebox", ConventionType::SNC, "Display a message box");
    RegisterAlias("queue_on_teleport", "queue_on_teleport", ConventionType::SNC, "Queue a script to run after teleporting");
    RegisterAliases("queue_on_teleport", {"queueonteleport"}, ConventionType::SNC, "Queue a script to run after teleporting");
    RegisterAlias("request", "request", ConventionType::SNC, "Send an HTTP request");
    RegisterAliases("request", {"http.request", "http_request"}, ConventionType::SNC, "Send an HTTP request");
    RegisterAlias("setclipboard", "setclipboard", ConventionType::SNC, "Set the clipboard content");
    RegisterAliases("setclipboard", {"toclipboard"}, ConventionType::SNC, "Set the clipboard content");
    RegisterAlias("setfpscap", "setfpscap", ConventionType::SNC, "Set the FPS cap");
    RegisterAlias("join", "join", ConventionType::SNC, "Join a game");
    RegisterAliases("join", {"joingame", "joinplace", "joinserver"}, ConventionType::SNC, "Join a game");
    RegisterAlias("gethwid", "gethwid", ConventionType::SNC, "Get the hardware ID");
    
    // Script functions
    RegisterAlias("getgc", "getgc", ConventionType::SNC, "Get the garbage collector");
    RegisterAlias("getgenv", "getgenv", ConventionType::SNC, "Get the global environment");
    RegisterAlias("getloadedmodules", "getloadedmodules", ConventionType::SNC, "Get loaded modules");
    RegisterAlias("getrenv", "getrenv", ConventionType::SNC, "Get the Roblox environment");
    RegisterAlias("getrunningscripts", "getrunningscripts", ConventionType::SNC, "Get running scripts");
    RegisterAlias("getscriptbytecode", "getscriptbytecode", ConventionType::SNC, "Get the bytecode of a script");
    RegisterAliases("getscriptbytecode", {"dumpstring"}, ConventionType::SNC, "Get the bytecode of a script");
    RegisterAlias("getscripthash", "getscripthash", ConventionType::SNC, "Get the hash of a script");
    RegisterAlias("getscripts", "getscripts", ConventionType::SNC, "Get all scripts");
    RegisterAlias("getsenv", "getsenv", ConventionType::SNC, "Get the environment of a script");
    RegisterAlias("getthreadidentity", "getthreadidentity", ConventionType::SNC, "Get the identity of the current thread");
    RegisterAliases("getthreadidentity", {"getidentity", "getthreadcontext"}, ConventionType::SNC, "Get the identity of the current thread");
    RegisterAlias("setthreadidentity", "setthreadidentity", ConventionType::SNC, "Set the identity of the current thread");
    RegisterAliases("setthreadidentity", {"setidentity", "setthreadcontext"}, ConventionType::SNC, "Set the identity of the current thread");
    
    // Drawing functions
    RegisterAlias("Drawing", "Drawing", ConventionType::SNC, "Drawing library");
    RegisterAlias("Drawing.new", "Drawing.new", ConventionType::SNC, "Create a new drawing object");
    RegisterAlias("Drawing.Fonts", "Drawing.Fonts", ConventionType::SNC, "Drawing fonts");
    RegisterAlias("isrenderobj", "isrenderobj", ConventionType::SNC, "Check if an object is a render object");
    RegisterAlias("cleardrawcache", "cleardrawcache", ConventionType::SNC, "Clear the drawing cache");
    
    // WebSocket functions
    RegisterAlias("WebSocket", "WebSocket", ConventionType::SNC, "WebSocket library");
    RegisterAlias("WebSocket.connect", "WebSocket.connect", ConventionType::SNC, "Connect to a WebSocket server");
    
    // Cryptography functions
    RegisterAlias("crypt.base64decode", "crypt.base64decode", ConventionType::SNC, "Decode base64 data");
    RegisterAliases("crypt.base64decode", {"crypt.base64.decode", "crypt.base64_decode", "base64.decode", "base64_decode"}, ConventionType::SNC, "Decode base64 data");
    RegisterAlias("crypt.base64encode", "crypt.base64encode", ConventionType::SNC, "Encode data as base64");
    RegisterAliases("crypt.base64encode", {"crypt.base64.encode", "crypt.base64_encode", "base64.encode", "base64_encode"}, ConventionType::SNC, "Encode data as base64");
    RegisterAlias("crypt.decrypt", "crypt.decrypt", ConventionType::SNC, "Decrypt data");
    RegisterAlias("crypt.encrypt", "crypt.encrypt", ConventionType::SNC, "Encrypt data");
    RegisterAlias("crypt.generatebytes", "crypt.generatebytes", ConventionType::SNC, "Generate random bytes");
    RegisterAlias("crypt.generatekey", "crypt.generatekey", ConventionType::SNC, "Generate a cryptographic key");
    RegisterAlias("crypt.hash", "crypt.hash", ConventionType::SNC, "Hash data");

    // Debug functions
    RegisterAlias("debug.getconstant", "debug.getconstant", ConventionType::SNC, "Get a constant from a function");
    RegisterAlias("debug.getconstants", "debug.getconstants", ConventionType::SNC, "Get all constants from a function");
    RegisterAlias("debug.getinfo", "debug.getinfo", ConventionType::SNC, "Get information about a function");
    RegisterAlias("debug.getproto", "debug.getproto", ConventionType::SNC, "Get a proto from a function");
    RegisterAlias("debug.getprotos", "debug.getprotos", ConventionType::SNC, "Get all protos from a function");
    RegisterAlias("debug.getstack", "debug.getstack", ConventionType::SNC, "Get the stack of a thread");
    RegisterAlias("debug.getupvalue", "debug.getupvalue", ConventionType::SNC, "Get an upvalue from a function");
    RegisterAlias("debug.getupvalues", "debug.getupvalues", ConventionType::SNC, "Get all upvalues from a function");
    RegisterAlias("debug.print", "debug.print", ConventionType::SNC, "Print debug information");
    RegisterAlias("debug.setconstant", "debug.setconstant", ConventionType::SNC, "Set a constant in a function");
    RegisterAlias("debug.setstack", "debug.setstack", ConventionType::SNC, "Set a value in the stack");
    RegisterAlias("debug.setupvalue", "debug.setupvalue", ConventionType::SNC, "Set an upvalue in a function");

    // File system functions
    RegisterAlias("appendfile", "appendfile", ConventionType::SNC, "Append to a file");
    RegisterAlias("delfile", "delfile", ConventionType::SNC, "Delete a file");
    RegisterAlias("delfolder", "delfolder", ConventionType::SNC, "Delete a folder");
    RegisterAlias("dofile", "dofile", ConventionType::SNC, "Execute a file");
    RegisterAlias("isfile", "isfile", ConventionType::SNC, "Check if a file exists");
    RegisterAlias("isfolder", "isfolder", ConventionType::SNC, "Check if a folder exists");
    RegisterAlias("listfiles", "listfiles", ConventionType::SNC, "List files in a folder");
    RegisterAlias("loadfile", "loadfile", ConventionType::SNC, "Load a file as a function");
    RegisterAlias("makefolder", "makefolder", ConventionType::SNC, "Create a folder");
    RegisterAlias("readfile", "readfile", ConventionType::SNC, "Read a file");
    RegisterAlias("writefile", "writefile", ConventionType::SNC, "Write to a file");

    // Instance interaction functions
    RegisterAlias("fireclickdetector", "fireclickdetector", ConventionType::SNC, "Fire a click detector");
    RegisterAlias("fireproximityprompt", "fireproximityprompt", ConventionType::SNC, "Fire a proximity prompt");
    RegisterAlias("firesignal", "firesignal", ConventionType::SNC, "Fire a signal");
    RegisterAlias("firetouchinterest", "firetouchinterest", ConventionType::SNC, "Fire a touch interest");
    RegisterAlias("getcallbackvalue", "getcallbackvalue", ConventionType::SNC, "Get a callback value");
    RegisterAlias("getconnections", "getconnections", ConventionType::SNC, "Get connections from a signal");
    RegisterAlias("getcustomasset", "getcustomasset", ConventionType::SNC, "Get a custom asset");
    RegisterAlias("gethiddenproperty", "gethiddenproperty", ConventionType::SNC, "Get a hidden property");
    RegisterAlias("gethui", "gethui", ConventionType::SNC, "Get the hidden UI");
    RegisterAlias("getinstances", "getinstances", ConventionType::SNC, "Get all instances");
    RegisterAlias("getnilinstances", "getnilinstances", ConventionType::SNC, "Get nil instances");
    RegisterAlias("isrbxactive", "isrbxactive", ConventionType::SNC, "Check if Roblox is active");
    RegisterAlias("sethiddenproperty", "sethiddenproperty", ConventionType::SNC, "Set a hidden property");

    // Mouse input functions
    RegisterAlias("mouse1click", "mouse1click", ConventionType::SNC, "Simulate a left mouse click");
    RegisterAlias("mouse1press", "mouse1press", ConventionType::SNC, "Simulate a left mouse press");
    RegisterAlias("mouse1release", "mouse1release", ConventionType::SNC, "Simulate a left mouse release");
    RegisterAlias("mouse2click", "mouse2click", ConventionType::SNC, "Simulate a right mouse click");
    RegisterAlias("mouse2press", "mouse2press", ConventionType::SNC, "Simulate a right mouse press");
    RegisterAlias("mouse2release", "mouse2release", ConventionType::SNC, "Simulate a right mouse release");
    RegisterAlias("mousemoveabs", "mousemoveabs", ConventionType::SNC, "Move the mouse to absolute coordinates");
    RegisterAlias("mousemoverel", "mousemoverel", ConventionType::SNC, "Move the mouse by relative coordinates");
    RegisterAlias("mousescroll", "mousescroll", ConventionType::SNC, "Simulate mouse scrolling");
    
    // Player functions
    RegisterAlias("getplayer", "getplayer", ConventionType::SNC, "Get a player");
    RegisterAlias("getlocalplayer", "getlocalplayer", ConventionType::SNC, "Get the local player");
    RegisterAlias("getplayers", "getplayers", ConventionType::SNC, "Get all players");
    RegisterAlias("runanimation", "runanimation", ConventionType::SNC, "Run an animation");
    RegisterAliases("runanimation", {"playanimation"}, ConventionType::SNC, "Run an animation");
    
    // SNC-specific aliases
    RegisterAlias("is_salad_closure", "isexecutorclosure", ConventionType::SNC, "Check if a function is a Salad closure");
    RegisterAlias("is_essence_closure", "isexecutorclosure", ConventionType::SNC, "Check if a function is an Essence closure");
    RegisterAlias("is_ronix_closure", "isexecutorclosure", ConventionType::SNC, "Check if a function is a Ronix closure");
    RegisterAlias("is_awp_closure", "isexecutorclosure", ConventionType::SNC, "Check if a function is an AWP closure");
    RegisterAlias("is_wave_closure", "isexecutorclosure", ConventionType::SNC, "Check if a function is a Wave closure");
}

} // namespace NamingConventions
} // namespace RobloxExecutor
