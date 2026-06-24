gcc asset_processor\*.c -O2 -o source\asset_processor.exe

if "%1"=="-o" (
    cd source
    asset_processor
)