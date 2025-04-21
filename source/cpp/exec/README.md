# Roblox Executor Naming Conventions Support

This module provides support for multiple naming conventions used by different Roblox executors. It allows scripts written for one executor to work seamlessly with our executor by mapping function names to their standard implementations.

## Features

- Support for UNC (Unified Naming Convention) standard
- Support for SNC (Salad Naming Convention)
- Support for executor-specific naming conventions:
  - Synapse X
  - KRNL
  - Script-Ware
  - Sentinel
  - Fluxus
  - Electron
  - Calamari
  - And more

## Implementation

The implementation consists of two main components:

1. `naming_conventions.hpp` - Defines the `NamingConventionMapper` class that maps function aliases to their standard names.
2. Integration with `impls.hpp` - Registers all naming conventions in the Lua environment.

## Usage

The naming conventions are automatically registered when the executor initializes. When a script calls a function using any supported naming convention, it will be automatically mapped to the standard implementation.

For example, if a script calls `syn.request()`, it will be mapped to the standard `request()` function.

## Testing

You can test the naming conventions mapper using the provided test program:

```bash
cd source/cpp/exec
make test
```

## Adding New Naming Conventions

To add support for a new naming convention:

1. Open `naming_conventions.hpp`
2. Add new aliases in the `InitializeAliasMap()` method using the `AddAlias()` function
3. Format: `AddAlias("alias_name", "standard_name");`

## Example

```cpp
// Add support for a new executor's naming conventions
AddAlias("newexecutor_loadstring", "loadstring");
AddAlias("newexecutor_request", "request");
AddAlias("newexecutor_setclipboard", "setclipboard");
```

## License

This code is part of the executor project and is subject to the same license terms.
