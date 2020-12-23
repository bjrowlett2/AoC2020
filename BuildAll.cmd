@ECHO OFF

FOR /D %%G in (Day_*) DO (
    RD /S /Q ".\%%G\.bin\"
    CALL .\Build.cmd .\%%G\
)
