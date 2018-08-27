@echo off

:: Configure build variables.
set BUILD_CONFIGURATION=Release
set BUILD_ARCH=x64
set BUILD_DIR=msvc15-%BUILD_ARCH%-%BUILD_CONFIGURATION%
set BUILD_GENERATOR="Visual Studio"
set ENABLE_TESTING=TRUE

set VS_VERSION=15
set VS_YEAR=2017
set VS_ARCH=Win64

echo Creating a build directory...

pushd ..

if not exist build mkdir build
pushd build

if not exist %BUILD_DIR% mkdir %BUILD_DIR%
pushd %BUILD_DIR%

echo [Conan] Downloading packages...
conan install ^
	--build gtest ^
	--build missing ^
	-s os="Windows" ^
	-s build_type=%BUILD_CONFIGURATION% ^
	-s compiler="Visual Studio" ^
	-s compiler.version=%VS_VERSION% ^
	-s compiler.runtime="MT" ^
	../..

echo [CMake] Generating the project...
cmake ^
	-G "Visual Studio %VS_VERSION% %VS_YEAR% %VS_ARCH%" ^
	-DENABLE_TESTING:BOOL=%ENABLE_TESTING% ^
	-DCMAKE_BUILD_TYPE:STRING=%BUILD_CONFIGURATION% ^
	../..

echo [MSVC] Building the project...
cmake --build . --config %BUILD_CONFIGURATION%

IF %ENABLE_TESTING% == TRUE (
	echo [MSVC] Running tests ...
	ctest -C %BUILD_CONFIGURATION% ./tests
)

echo Project has been built successfully!
popd
popd
popd
goto :eof

:error
echo [ERROR] Failed to generate project.
popd
popd
popd
exit /b 1