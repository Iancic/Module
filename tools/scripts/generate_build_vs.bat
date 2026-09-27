@echo off
REM Generates the Visual Studio files and then builds the whole engine (all configs).
REM Convenience wrapper: runs generate_vs.bat, then build_vs.bat.
REM Stops with a non-zero exit code if either step fails.

call "%~dp0build\generate_vs.bat" nopause || goto :error

call "%~dp0build\build_vs.bat" all nopause || goto :error

pause
exit /b 0

:error
pause
exit /b 1
