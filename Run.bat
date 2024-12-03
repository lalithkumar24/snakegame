cls

REM Navigate to Scrip dir;
cd /d "%~dp0"

REM Bulding Soluction using MsBuild
echo Build Soluction...
MSBuild build\Snake_Game.Sln

REM Check if the build was successful 
IF %ERRORLEVEL% NEQ 0 (
    echo Build failed. Exiting.
    exit /b %ERRORLEVEL%
)

REM Changing to  the Builded Soluction
cd build\Debug

cls

REM Run the Executable
echo Running the executable..
Snake_Game.exe


cls