@echo off
REM Clears the generated build output: deletes the build/vs folder (solution,
REM projects, and all compiled .lib/.obj output). A fresh generate_vs.bat rebuilds it.
REM Everything under build/vs is generated, so this is safe.

pushd "%~dp0..\.."

if exist "build\vs" (
    echo Deleting build\vs ...
    rmdir /s /q "build\vs"
    echo Done.
) else (
    echo Nothing to clean; build\vs does not exist.
)

popd
pause
exit /b 0
