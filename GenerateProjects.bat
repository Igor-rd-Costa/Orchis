cd Core\vendor\assimp\
cmake -DBUILD_SHARED_LIBS=OFF -DUSE_STATIC_CRT=ON -DASSIMP_INSTALL=OFF -DASSIMP_NO_EXPORT=OFF -DASSIMP_BUILD_ZLIB=ON -DASSIMP_BUILD_TESTS=OFF -DASSIMP_USE_STB_IMAGE_STATIC=ON CMakeLists.txt
cmake --build . --config Release
cd ..\..\..\
call vendor\premake\premake5.exe vs2022
PAUSE