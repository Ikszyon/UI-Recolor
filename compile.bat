@echo off

cls

rmdir build >> NUL

cmake -G "Visual Studio 17 2022" -B build -DCMAKE_BUILD_TYPE=Release -T host=x86 -A win32

timeout /T 1 /nobreak >> NUL

cmake --build build --config Release --target uirecolor

explorer %~dp0build\Release\

@echo on