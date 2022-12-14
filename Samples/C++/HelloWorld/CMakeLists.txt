if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_LIST_DIR)
    cmake_minimum_required(VERSION 3.6)
    # set the platform
    option(BUILD_LINUX "Build samples for running on Linux x86_64" ON)
    option(BUILD_ARM "Build samples for running on ARM x86" OFF)
    option(BUILD_ARM64 "Build samples for running on ARM x64" OFF)

    if(BUILD_LINUX)
        set(LIBPATH ../../../Lib/Linux/x64)
    elseif(BUILD_ARM)
        set(LIBPATH ../../../Lib/ARM32)
        add_compile_options(-DDM_ARM -D__ARM_NEON__ -mfpu=neon)
    elseif(BUILD_ARM64)
        set(LIBPATH ../../../Lib/ARM64)
        add_compile_options(-DDM_ARM -D__ARM_NEON__)
    else()
        message(FATAL_ERROR "Please specify a supported platform")
    endif()

    set (CMAKE_CXX_STANDARD 11)
    add_compile_options(-O2 -fPIC)
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -O2 -fvisibility=hidden -fvisibility-inlines-hidden -L ${LIBPATH} -Wl,-rpath,${LIBPATH} -Wl,-rpath,'$ORIGIN' -static-libgcc -static-libstdc++ -s")
endif()

project(HelloWorld)

file(GLOB FILE_SRCS
HelloWorld.cpp
)
add_executable(HelloWorld ${FILE_SRCS})
set_target_properties(HelloWorld PROPERTIES SKIP_BUILD_RPATH TRUE)
target_link_libraries(HelloWorld DynamsoftCore)
target_link_libraries(HelloWorld DynamsoftDocumentNormalizer)

