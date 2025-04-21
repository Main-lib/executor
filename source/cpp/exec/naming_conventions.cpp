#include "naming_conventions.hpp"
#include <iostream>

namespace NamingConventions {

// Initialize all naming conventions from both UNC and SNC repositories
void InitializeConventions() {
    std::cout << "Initializing naming conventions from UNC and SNC repositories..." << std::endl;
    
    // Register Cache-related conventions
    g_registry.RegisterConvention(Convention(
        "cache.invalidate",
        {},
        Cache::invalidate,
        "Invalidates a cached instance reference",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "cache.iscached",
        {},
        Cache::iscached,
        "Checks if an instance is cached",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "cache.replace",
        {},
        Cache::replace,
        "Replaces a cached instance with another",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "cloneref",
        {},
        Cache::cloneref,
        "Creates a clone of an instance reference",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "compareinstances",
        {},
        Cache::compareinstances,
        "Compares two instance references",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Closure-related conventions
    g_registry.RegisterConvention(Convention(
        "checkcaller",
        {},
        Closures::checkcaller,
        "Checks if the current function was called by the executor",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "clonefunction",
        {},
        Closures::clonefunction,
        "Creates a clone of a function",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getcallingscript",
        {},
        Closures::getcallingscript,
        "Gets the script that called the current function",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getscriptclosure",
        {"getscriptfunction"},
        Closures::getscriptclosure,
        "Gets the closure of a script",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "hookfunction",
        {"replaceclosure"},
        Closures::hookfunction,
        "Hooks a function to call a custom function",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "closuretype",
        {},
        Closures::closuretype,
        "Gets the type of a closure",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "iscclosure",
        {},
        Closures::iscclosure,
        "Checks if a function is a C closure",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "islclosure",
        {},
        Closures::islclosure,
        "Checks if a function is a Lua closure",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "isexecutorclosure",
        {"checkclosure", "isourclosure"},
        Closures::isexecutorclosure,
        "Checks if a function is an executor closure",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "loadstring",
        {},
        Closures::loadstring,
        "Loads a string as Lua code",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Instance-related conventions
    g_registry.RegisterConvention(Convention(
        "fireclickdetector",
        {},
        Instances::fireclickdetector,
        "Fires a click detector",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "firetouchinterest",
        {"firetouchtransmitter"},
        Instances::firetouchinterest,
        "Fires a touch interest",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "fireproximityprompt",
        {},
        Instances::fireproximityprompt,
        "Fires a proximity prompt",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "firesignal",
        {},
        Instances::firesignal,
        "Fires a signal",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getcallbackvalue",
        {},
        Instances::getcallbackvalue,
        "Gets the value of a callback property",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getconnections",
        {},
        Instances::getconnections,
        "Gets the connections of a signal",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getcustomasset",
        {},
        Instances::getcustomasset,
        "Gets a custom asset",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "gethiddenproperty",
        {},
        Instances::gethiddenproperty,
        "Gets a hidden property of an instance",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "gethui",
        {},
        Instances::gethui,
        "Gets the hidden UI container",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getinstances",
        {},
        Instances::getinstances,
        "Gets all instances",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getnilinstances",
        {},
        Instances::getnilinstances,
        "Gets all nil instances",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "isscriptable",
        {},
        Instances::isscriptable,
        "Checks if a property is scriptable",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "sethiddenproperty",
        {},
        Instances::sethiddenproperty,
        "Sets a hidden property of an instance",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setrbxclipboard",
        {},
        Instances::setrbxclipboard,
        "Sets the Roblox clipboard",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setscriptable",
        {},
        Instances::setscriptable,
        "Sets a property as scriptable",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getplayer",
        {},
        Instances::getplayer,
        "Gets a player by name or the local player",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getlocalplayer",
        {},
        Instances::getlocalplayer,
        "Gets the local player",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getplayers",
        {},
        Instances::getplayers,
        "Gets all players",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "runanimation",
        {"playanimation"},
        Instances::runanimation,
        "Runs an animation on a player",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Metatable-related conventions
    g_registry.RegisterConvention(Convention(
        "getrawmetatable",
        {},
        Metatables::getrawmetatable,
        "Gets the raw metatable of an object",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "hookmetamethod",
        {},
        Metatables::hookmetamethod,
        "Hooks a metamethod",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getnamecallmethod",
        {},
        Metatables::getnamecallmethod,
        "Gets the name of the method being called",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "isreadonly",
        {},
        Metatables::isreadonly,
        "Checks if a table is read-only",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setrawmetatable",
        {},
        Metatables::setrawmetatable,
        "Sets the raw metatable of an object",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setreadonly",
        {},
        Metatables::setreadonly,
        "Sets a table as read-only",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Miscellaneous conventions
    g_registry.RegisterConvention(Convention(
        "identifyexecutor",
        {"getexecutorname"},
        Misc::identifyexecutor,
        "Identifies the executor",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "lz4compress",
        {},
        Misc::lz4compress,
        "Compresses data using LZ4",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "lz4decompress",
        {},
        Misc::lz4decompress,
        "Decompresses data using LZ4",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "messagebox",
        {},
        Misc::messagebox,
        "Shows a message box",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "queue_on_teleport",
        {"queueonteleport"},
        Misc::queue_on_teleport,
        "Queues code to run after teleporting",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "request",
        {"http.request", "http_request"},
        Misc::request,
        "Sends an HTTP request",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setclipboard",
        {"toclipboard"},
        Misc::setclipboard,
        "Sets the clipboard",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setfpscap",
        {},
        Misc::setfpscap,
        "Sets the FPS cap",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "join",
        {"joingame", "joinplace", "joinserver"},
        Misc::join,
        "Joins a game",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "gethwid",
        {},
        Misc::gethwid,
        "Gets the hardware ID",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Script-related conventions
    g_registry.RegisterConvention(Convention(
        "getgc",
        {},
        Scripts::getgc,
        "Gets the garbage collector",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getgenv",
        {},
        Scripts::getgenv,
        "Gets the global environment",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getloadedmodules",
        {},
        Scripts::getloadedmodules,
        "Gets all loaded modules",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getrenv",
        {},
        Scripts::getrenv,
        "Gets the Roblox environment",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getrunningscripts",
        {},
        Scripts::getrunningscripts,
        "Gets all running scripts",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getscriptbytecode",
        {"dumpstring"},
        Scripts::getscriptbytecode,
        "Gets the bytecode of a script",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getscripthash",
        {},
        Scripts::getscripthash,
        "Gets the hash of a script",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getscripts",
        {},
        Scripts::getscripts,
        "Gets all scripts",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getsenv",
        {},
        Scripts::getsenv,
        "Gets the environment of a script",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "getthreadidentity",
        {"getidentity", "getthreadcontext"},
        Scripts::getthreadidentity,
        "Gets the identity of the current thread",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "setthreadidentity",
        {"setidentity", "setthreadcontext"},
        Scripts::setthreadidentity,
        "Sets the identity of the current thread",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register Drawing-related conventions
    g_registry.RegisterConvention(Convention(
        "Drawing.new",
        {},
        Drawing::new_drawing,
        "Creates a new drawing object",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "isrenderobj",
        {},
        Drawing::isrenderobj,
        "Checks if an object is a render object",
        "BOTH" // Found in both UNC and SNC
    ));
    
    g_registry.RegisterConvention(Convention(
        "cleardrawcache",
        {},
        Drawing::cleardrawcache,
        "Clears the drawing cache",
        "BOTH" // Found in both UNC and SNC
    ));
    
    // Register WebSocket-related conventions
    g_registry.RegisterConvention(Convention(
        "WebSocket.connect",
        {},
        WebSocket::connect,
        "Connects to a WebSocket server",
        "BOTH" // Found in both UNC and SNC
    ));
    
    std::cout << "Naming conventions initialized successfully." << std::endl;
}

// Placeholder implementations for all functions
// These will be replaced with actual implementations

// Cache-related conventions
namespace Cache {
    int invalidate(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int iscached(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int replace(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int cloneref(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int compareinstances(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Closure-related conventions
namespace Closures {
    int checkcaller(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int clonefunction(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getcallingscript(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getscriptclosure(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int hookfunction(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int closuretype(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int iscclosure(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int islclosure(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int isexecutorclosure(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int loadstring(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Instance-related conventions
namespace Instances {
    int fireclickdetector(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int firetouchinterest(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int fireproximityprompt(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int firesignal(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getcallbackvalue(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getconnections(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getcustomasset(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int gethiddenproperty(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int gethui(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getinstances(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getnilinstances(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int isscriptable(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int sethiddenproperty(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setrbxclipboard(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setscriptable(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getplayer(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getlocalplayer(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getplayers(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int runanimation(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Metatable-related conventions
namespace Metatables {
    int getrawmetatable(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int hookmetamethod(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getnamecallmethod(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int isreadonly(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setrawmetatable(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setreadonly(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Miscellaneous conventions
namespace Misc {
    int identifyexecutor(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int lz4compress(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int lz4decompress(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int messagebox(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int queue_on_teleport(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int request(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setclipboard(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setfpscap(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int join(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int gethwid(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Script-related conventions
namespace Scripts {
    int getgc(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getgenv(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getloadedmodules(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getrenv(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getrunningscripts(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getscriptbytecode(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getscripthash(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getscripts(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getsenv(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int getthreadidentity(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int setthreadidentity(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Drawing-related conventions
namespace Drawing {
    int new_drawing(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int isrenderobj(lua_State* L) {
        // Implementation will be added
        return 0;
    }
    
    int cleardrawcache(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// WebSocket-related conventions
namespace WebSocket {
    int connect(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Debug-related conventions
namespace Debug {
    int debug_functions(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Crypt-related conventions
namespace Crypt {
    int crypt_functions(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Filesystem-related conventions
namespace Filesystem {
    int filesystem_functions(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

// Input-related conventions
namespace Input {
    int input_functions(lua_State* L) {
        // Implementation will be added
        return 0;
    }
}

} // namespace NamingConventions
