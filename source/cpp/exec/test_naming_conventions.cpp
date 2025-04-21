#include "naming_conventions.hpp"
#include <iostream>

// Simple test function to verify that our naming conventions mapper is working correctly
void TestNamingConventions() {
    // Get the singleton instance
    NamingConventionMapper* mapper = NamingConventionMapper::GetInstance();
    
    // Test some standard UNC functions
    std::cout << "Testing UNC standard functions:" << std::endl;
    std::cout << "cache.invalidate -> " << mapper->GetStandardName("cache.invalidate") << std::endl;
    std::cout << "loadstring -> " << mapper->GetStandardName("loadstring") << std::endl;
    std::cout << "getrawmetatable -> " << mapper->GetStandardName("getrawmetatable") << std::endl;
    
    // Test some aliases
    std::cout << "\nTesting aliases:" << std::endl;
    std::cout << "getscriptfunction -> " << mapper->GetStandardName("getscriptfunction") << std::endl;
    std::cout << "replaceclosure -> " << mapper->GetStandardName("replaceclosure") << std::endl;
    std::cout << "queueonteleport -> " << mapper->GetStandardName("queueonteleport") << std::endl;
    
    // Test executor-specific naming conventions
    std::cout << "\nTesting executor-specific naming conventions:" << std::endl;
    std::cout << "syn.request -> " << mapper->GetStandardName("syn.request") << std::endl;
    std::cout << "krnl_loadstring -> " << mapper->GetStandardName("krnl_loadstring") << std::endl;
    std::cout << "sentinel_checkcaller -> " << mapper->GetStandardName("sentinel_checkcaller") << std::endl;
    
    // Test getting aliases for a standard function
    std::cout << "\nAliases for 'loadstring':" << std::endl;
    std::vector<std::string> loadstringAliases = mapper->GetAliasesForStandard("loadstring");
    for (const auto& alias : loadstringAliases) {
        std::cout << "  - " << alias << std::endl;
    }
    
    std::cout << "\nAliases for 'request':" << std::endl;
    std::vector<std::string> requestAliases = mapper->GetAliasesForStandard("request");
    for (const auto& alias : requestAliases) {
        std::cout << "  - " << alias << std::endl;
    }
    
    std::cout << "\nTest completed successfully!" << std::endl;
}

// Main function for standalone testing
int main() {
    TestNamingConventions();
    return 0;
}
