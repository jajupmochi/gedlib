# Project Setup Tutorial

This is a tutorial to setup settings to run the test code.

(It can be very struggling for me to do it, since I may drop this project for months and years, and forget all about C++. Then time to time I need to update this library, and even worse, setup everything on a brand new machine! So this tutorial helps me to remember and setup things quickly.)

# Setups for VS Code

The basic setups can be found in `.vscode`, including `launch.json` and `task.json`.

I use Clang++ and CodeLLDB tool chain, which is claimed to be "modern".

## Step 1: Install VS Code.

For Ubuntu, I recommend to download .deb file from the website and install. **Do not use snap version comes with the Ubuntu App store**. It runs in a sandbox, which can cause a super amout of permission problem!

## Step 2: Install necessary packages.

```bash
sudo apt update
sudo apt install clang lldb # Compiler and debugger
clang++ --version
lldb --version

sudo apt install ninja-build doxygen # These are required in the library's `CMakeLists.txt` file.
```

## Step 3: Install extensions.

You will need the following extensions:

- C/C++ by Microsoft
- CodeLLDB (A tool to use LLDB. Otherwise you will need cppdbg and install lldb-mi)
- CMake Tools
- (optional) clangd (Syntax analysis)
- (optional) clang-format (formatting)

## Step 4: Writing config files.

This includes `launch.json`, `task.json`, and (optional) `c_cpp_properties.json` under `.vscode` folder. I suggest you just use the ones comes with the repo, otherwise it can be super frustrating to setup for beginners.

Here are some demos, which should work directly:

`launch.json`:

```json
{
    // Use IntelliSense to learn about possible attributes.
    // Hover to view descriptions of existing attributes.
    // For more information, visit: https://go.microsoft.com/fwlink/?linkid=830387
    "version": "0.2.0",
    "configurations": [
    {
      "name": "Debug CMake active file for unitest",
      "type": "lldb",
      "request": "launch",
      "program": "${workspaceFolder}/tests/unit_tests/bin/${fileBasenameNoExtension}",
      "args": [],
      "cwd": "${workspaceFolder}",
      "stopOnEntry": false,
      "preLaunchTask": "cmake build active file",
      "console": "integratedTerminal"
    },
    ]
}
```

`task.json`:

```json
{
  "version": "2.0.0",
  "tasks": [
    {
      "label": "cmake configure",
      "type": "shell",
      "command": "cmake",
      "args": [
        "-S",
        "${workspaceFolder}",
        "-B",
        "${workspaceFolder}/build",
        "-DCMAKE_BUILD_TYPE=Debug"
      ],
      "problemMatcher": []
    },
    {
      "label": "cmake build active file",
      "dependsOn": ["cmake configure"],
      "type": "shell",
      "command": "cmake",
      "args": [
        "--build",
        "${workspaceFolder}/build",
        "--target",
        "${fileBasenameNoExtension}"
      ],
      "group": {
        "kind": "build",
        "isDefault": true
      },
      "problemMatcher": ["$gcc"]
    },
  ]
}
```

`c_cpp_properties.json`:

```json
{
  "configurations": [
    {
      "name": "Linux-Clang",
      "compilerPath": "/usr/bin/clang++",
      "cStandard": "c11",
      "cppStandard": "c++17",
      "intelliSenseMode": "linux-clang-x64",
      "includePath": [
        "${workspaceFolder}/**"
      ]
    }
  ],
  "version": 4
}
```

These settings will build and run the debugging automatically for the `tests/unit_tests/src/ged_computation_tests.cpp`. I did not test any others.

## Step 5: Build the library.

Go to the root dir of the project, run

```python
python3 setup.py
```

This will make use of multiple `CMakeLists.txt` files, install multiple external libraries and link them. A `lib/libgxlgedlib.so` file will be generated. You will face a lot of linking errors without this step.

## Step 6: Run cpp code.

Open `tests/unit_tests/src/ged_computation_tests.cpp`, and run it. Use F5 as a shortkey.
