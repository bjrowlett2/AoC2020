@ECHO OFF
SETLOCAL EnableDelayedExpansion

IF "%1" == "" (
    ECHO Usage:
    ECHO .\Build.cmd ^<Puzzle_Directory^>
    EXIT /B 1
)

SET ARCHITECTURE=x86_64
IF "%ARCHITECTURE%" == "x86" (
    REM Activate the Visual Studio 2019 Developer Command Prompt (x86)
    REM This allows us to use some Visual Studio tools, for example: CL and LINK
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"

    SET LIB=%CD%\Package\Lib32;%LIB%
    SET INCLUDE=%CD%\Package\Include;%INCLUDE%
) ELSE IF "%ARCHITECTURE%" == "x86_64" (
    REM Activate the Visual Studio 2019 Developer Command Prompt (x86_64)
    REM This allows us to use some Visual Studio tools, for example: CL and LINK
    CALL "C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars64.bat"

    SET LIB=%CD%\Package\Lib64;!LIB!
    SET INCLUDE=%CD%\Package\Include;!INCLUDE!
)

PUSHD "%1"
    IF NOT EXIST ".bin\" (
        MKDIR ".bin\"
    )

    PUSHD ".bin\"
        REM Compiler Flags:
        REM     /FC (Full Path of Source Code File in Diagnostics)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/fc-full-path-of-source-code-file-in-diagnostics?view=msvc-160
        REM
        REM     /MD, /MT, /LD (Use Run-Time Library)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/md-mt-ld-use-run-time-library?view=msvc-160
        REM
        REM     /Od (Optimize code, Disable (Debug))
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/od-disable-debug?view=msvc-160
        REM
        REM     /Oi (Generate Intrinsic Functions)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/oi-generate-intrinsic-functions?view=msvc-160
        REM
        REM     /W0, /W1, /W2, /W3, /W4 (Warning level)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-160
        REM
        REM     /WX (Treat compiler warnings as errors)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/compiler-option-warning-level?view=msvc-160
        REM
        REM     /Z7, /Zi, /ZI (Debug Information Format)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/z7-zi-zi-debug-information-format?view=msvc-160

        REM Linker Flags
        REM     /INCREMENTAL (Link Incrementally)
        REM     https://docs.microsoft.com/en-us/cpp/build/reference/incremental-link-incrementally?view=msvc-160

        REM Disabled Warnings:
        REM     Compiler Warning (level 4) C4100
        REM     https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4100?view=msvc-160
        REM
        REM     Compiler Warning (level 4) C4127
        REM     https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4127?view=msvc-160
        REM
        REM     Compiler Warning (level 4) C4189
        REM     https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4189?view=msvc-160
        REM
        REM     Compiler Warning (level 4) C4201
        REM     https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4201?view=msvc-160
        REM
        REM     Compiler Warning (level 4) C4702
        REM     https://docs.microsoft.com/en-us/cpp/error-messages/compiler-warnings/compiler-warning-level-4-c4702?view=msvc-160

        CL "..\Solution.cpp" /DDEBUG /FC /MTd /Od /Oi /std:c++latest /W4 /wd4100 /wd4127 /wd4189 /wd4201 /wd4702 /WX /Z7 /link /INCREMENTAL:NO
    POPD

    IF "%ERRORLEVEL%" == "0" (
        CLS
        CALL ".bin\Solution.exe"
    )
POPD
