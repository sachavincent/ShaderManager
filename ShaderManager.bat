@echo off
setlocal enabledelayedexpansion

:: Set project root directory (modify this to match your project's location)
set "PROJECT_ROOT=%~dp0"

set "LIBS_DIR=%PROJECT_ROOT%\libs"
set "TESTS_ASSETS_DIR=%PROJECT_ROOT%\ShaderManagerTests\assets"

:: Define include directories
set "INCLUDE_DIRS=%LIBS_DIR%\efsw\include;%LIBS_DIR%\glfw\include;%LIBS_DIR%\glm;%LIBS_DIR%\glslang\install\include;%LIBS_DIR%\glaze\include;%LIBS_DIR%\magic_enum\include;%LIBS_DIR%\fmt\include;%LIBS_DIR%\glad"

set "LIB_FILES_DEBUG=%LIBS_DIR%\glad\lib\glad.lib;%LIBS_DIR%\fmt\out\build\x64-Debug\fmtd.lib;%GLSLANG_LIBS_DEBUG%;%LIBS_DIR%\glfw\out\install\x64-Debug\lib\glfw3.lib"
set "LIB_FILES_RELEASE=%LIBS_DIR%\glad\lib\glad.lib;%LIBS_DIR%\fmt\out\build\x64-Release\fmt.lib;%GLSLANG_LIBS_RELEASE%;%LIBS_DIR%\glfw\out\install\x64-Release\lib\glfw3.lib"


for %%L in (
    "%LIBS_DIR%\glslang\install\lib\glslangd.lib",
    "%LIBS_DIR%\glslang\install\lib\OSDependentd.lib",
    "%LIBS_DIR%\glslang\install\lib\MachineIndependentd.lib",
    "%LIBS_DIR%\glslang\install\lib\glslang-default-resource-limitsd.lib",
    "%LIBS_DIR%\glslang\install\lib\GenericCodeGend.lib",
    "%LIBS_DIR%\glslang\install\lib\SPIRVd.lib"
) do (
    set "LIB_FILES_DEBUG=!LIB_FILES_DEBUG!;%%~L"
)

for %%L in (
    "%LIBS_DIR%\glslang\install\lib\glslang.lib",
    "%LIBS_DIR%\glslang\install\lib\OSDependent.lib",
    "%LIBS_DIR%\glslang\install\lib\MachineIndependent.lib",
    "%LIBS_DIR%\glslang\install\lib\glslang-default-resource-limits.lib",
    "%LIBS_DIR%\glslang\install\lib\GenericCodeGen.lib",
    "%LIBS_DIR%\glslang\install\lib\SPIRV.lib"
) do (
    set "LIB_FILES_RELEASE=!LIB_FILES_RELEASE!;%%~L"
)

:: Set the INCLUDE environment variable
::setx PROJECT_INCLUDE_DIRS "%INCLUDE_DIRS%" /M
::echo Include directories set: %INCLUDE_DIRS%

:: Set the LIB environment variable
::setx PROJECT_LIB_FILES "%LIB_FILES%" /M
::echo Library files set: %LIB_FILES%

:: Path to Visual Studio solution file (modify to match your project)
::set "SOLUTION_FILE=%PROJECT_ROOT%\MyProject.sln"

:: Open the Visual Studio solution
::start devenv "%SOLUTION_FILE%"

start ShaderManager.sln