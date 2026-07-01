mkdir build
cd build
mkdir meshes
mkdir textures
mkdir shaders
cd ..

cd source
asset_processor
cd ..

xcopy source\configs.config build /E /I /Y

gcc ^
source/lib/*.c ^
source/lib/collections/*.c ^
source/lib/components/*.c ^
source/lib/math/*.c ^
source/dependencies/*.a ^
source/dependencies/*.c ^
-lopengl32 ^
-lgdi32 ^
-O2 -o build/game.exe

cd build
game.exe



