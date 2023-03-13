@echo off
pushd %~dp0\..\
call Pinecone\vendor\premake\bin\premake5.exe vs2022
popd
PAUSE
