# C++ Compile and setting problems

## (2025.05.27) file takes too long to build

```bash
-- Building GEDLIB with build type 'Debug'.
-- Configuring done (6.5s)
-- Generating done (46.5s)
-- Build files have been written to: /mnt/F0CA2D75CA2D38EC/research-repo/codes/gedlib/build
```

If this is the reason, a workaround is to comment `"dependsOn":["cmake configure"],` in `tasks.json`->`"tasks"`.

Maybe it can also be done by adding `CMakePresets.json` or modify `CMakeLists.txt`.

## (2025.05.27) error: variable length array declaration not allowed at file scope

```bash
error: variable length array declaration not allowed at file scope
char FatalConditionHandler::altStackMem[SIGSTKSZ] = {};
```

This error can be solved better, but a workaround is to add `"--target", "${fileBasenameNoExtension}"` to `tasks.json`->`"tasks"."args"`, so that one the active cpp file will be built.
