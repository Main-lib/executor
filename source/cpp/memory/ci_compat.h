#pragma once

// Memory and platform compatibility header
// This file provides platform-specific definitions for memory operations

// Platform detection
#if defined(__APPLE__)
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define PLATFORM_IOS
    #elif TARGET_OS_MAC
        #define PLATFORM_MACOS
    #endif
#elif defined(_WIN32) || defined(_WIN64)
    #define PLATFORM_WINDOWS
#elif defined(__ANDROID__)
    #define PLATFORM_ANDROID
#elif defined(__linux__)
    #define PLATFORM_LINUX
#endif

// Memory protection utilities
// Real implementations will be provided elsewhere
// These are just forward declarations
void* MEMORY_PROTECT(void* addr, size_t size, int prot);
bool MEMORY_UNPROTECT(void* addr, size_t size);
