@echo %off
pushd "%~dp0.."

echo Running ecc for ClangD, Clang Tidy and Format.
premake/premake5.exe ecc || goto: error
premake/premake5.exe tidy || goto: error
premake/premake5.exe format || goto: error

pause
exit /b 0

:error
echo Step failed, stopping.
popd
pause

pause
exit /b 1