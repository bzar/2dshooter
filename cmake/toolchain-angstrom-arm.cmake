INCLUDE(CMakeForceCompiler)

set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_VERSION 1)

set(PWD /home/bzar/src/panorama/pandora)
set(TOOLCHAIN_ROOT /usr/local/angstrom/arm)
set(PND_SDK ${TOOLCHAIN_ROOT}/arm-angstrom-linux-gnueabi)

CMAKE_FORCE_C_COMPILER(${TOOLCHAIN_ROOT}/bin/arm-angstrom-linux-gnueabi-gcc GNU)
CMAKE_FORCE_CXX_COMPILER(${TOOLCHAIN_ROOT}/bin/arm-angstrom-linux-gnueabi-g++ GNU)

set(CMAKE_FIND_ROOT_PATH ${PND_SDK}/usr)
set(CMAKE_SKIP_BUILD_RPATH  TRUE)
set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE) 
set(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib:${PND_SDK}/lib:${PND_SDK}/usr/lib")
set(CMAKE_INCLUDE_PATH "${CMAKE_INSTALL_PREFIX}/include:${PND_SDK}/include:${PND_SDK}/usr/include")
set(CMAKE_LIBRARY_PATH "${CMAKE_INSTALL_PREFIX}/lib:${PND_SDK}/lib:${PND_SDK}/usr/lib")
set(CMAKE_EXE_LINKER_FLAGS "-L${CMAKE_INSTALL_PREFIX}/lib -L${PND_SDK}/lib -L${PND_SDK}/usr/lib -Wl,-rpath,${PND_SDK}/usr/lib")
set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
set(CMAKE_BUILD_TYPE release)
set(CMAKE_CXX_FLAGS "-O3 -pipe -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -ffast-math -fsingle-precision-constant -fsigned-char -I${PND_SDK}/usr/include")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -pipe -march=armv7-a -mtune=cortex-a8 -mfpu=neon -mfloat-abi=softfp -ftree-vectorize -ffast-math -fsingle-precision-constant -fsigned-char -I${PND_SDK}/usr/include")
set(CMAKE_C_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM BOTH)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)

set(CMAKE_MODULE_PATH "${PND_SDK}/usr/lib/cmake" "${PND_SDK}/share/apps/cmake/modules" ${CMAKE_MODULE_PATH})