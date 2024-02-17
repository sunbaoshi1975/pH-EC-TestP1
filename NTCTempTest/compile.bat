rem This compiles the P1 project via cloud through command line
@ECHO off
mkdir .\compile
del /q .\compile
for /R %%f in (*.h) do copy "%%f" .\compile
for /R %%f in (*.cpp) do copy "%%f" .\compile
for /R %%f in (*.ino) do copy "%%f" .\compile
call particle compile p1 .\compile --target 1.0.0 --saveTo TempTest.bin
call del /q .\compile
call rmdir /q .\compile
