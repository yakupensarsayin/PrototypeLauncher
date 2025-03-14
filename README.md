Overview
--------
PrototypeLauncher is a lightweight Windows application designed to launch the Prototype game executable. It allows users to select the game's .exe file via a file dialog and starts the process with a predefined (4) CPU affinity mask.

Functionality
-------------
- File Selection: Opens a file dialog to locate and select the Prototype game's executable (.exe).
- Process Launching: Executes the selected .exe with a CPU affinity mask set to utilize specific cores.
- Default Path Check: Automatically suggests the Steam installation path for Prototype if detected.

Compilation Instructions
------------------------

Prerequisites
- Windows OS
- Resource file (resources.rc) and icon (app.ico) in the project directory

a. Using cl (Microsoft C++ Compiler)
1. Ensure Visual Studio or Windows SDK is installed with `cl.exe` and `rc.exe`.
2. Open a command prompt in the project directory.
3. Compile the resource file: `rc resources.rc`
4. Compile and link the application:
   ```bash
    cl /EHsc /MT /O2 /DUNICODE /D_UNICODE Main.cpp Window.cpp ProcessHandler.cpp resources.res /link comdlg32.lib user32.lib gdi32.lib
    ```
5. Output: `Main.exe`

b. Using g++ (MinGW)
1. Install MinGW-w64 (e.g., via MSYS2) with `g++` and `windres`.
2. Open a terminal in the project directory.
3. Compile the resource file:
   `windres resources.rc -o resources.o`
4. Compile and link the application:
   ```bash
    g++ -municode -O2 Main.cpp Window.cpp ProcessHandler.cpp resources.o -o PrototypeLauncher.exe -lcomdlg32 -luser32 -lgdi32 -static
    ```
5. Output: `PrototypeLauncher.exe`

Notes
-----
- The application is statically linked (/MT or -static) to ensure no runtime dependencies.
- Antivirus software may flag the executable as suspicious due to its unsigned status. This is a known false positive.
