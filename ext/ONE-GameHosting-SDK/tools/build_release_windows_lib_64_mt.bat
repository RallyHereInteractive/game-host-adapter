 cd ..
 mkdir build
 
 :: Toolchain version 15 - Visual studio 2017.
 cmake -G "Visual Studio 15" -S . -B ./build -T host=x64 -A x64 -DBUILD_MSVC_MT=ON
 cmake --build ./build --config Release --target ALL_BUILD