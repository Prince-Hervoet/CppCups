@echo off
REM 删除当前目录下的 build 文件夹
rmdir /s /q build

REM 创建一个新的 build 文件夹
mkdir build

REM 汇编
gcc -c simple_context.s -o simple_context.o

REM 进入 build 文件夹
cd build

REM 使用 CMake 生成 MinGW Makefiles
cmake -G "MinGW Makefiles" ..

REM 使用 make 构建项目
make

a.exe

REM 退出 build 文件夹
cd ..
