@echo off
pushd %~dp0

cd ..

call Dependencies\Premake\premake5.exe vs2019

popd
