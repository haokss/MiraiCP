cmake_minimum_required(VERSION 3.14)
project(MiraiCP_pro LANGUAGES CXX) # MSVC detecting must be after project declaration

#
set(CMAKE_CXX_STANDARD 17) # not working on MSVC
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (MSVC)
    message("MSVC detected")
    # 以utf8编译, for msvc; EHa参数启用标准 C++ stack unwinding; 启用多线程编译；禁止无返回值的函数；禁用两个与dll export相关的warning
    add_compile_options(/W4 /utf-8 /EHa /std:c++17 /MP /we4715 /wd4251 /wd4275 /Zc:__cplusplus)
    # See Warning C4668
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    # Disable RTTI.
    # string(REGEX REPLACE "/GR" "" CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS})
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /GR-")
    message("modified cmake cxx flags: ${CMAKE_CXX_FLAGS}")
else ()
    # 禁止无返回值的函数
    add_compile_options(-Wall -Werror=return-type)
    # Disable RTTI.
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fno-rtti")
    message("modified cmake cxx flags: ${CMAKE_CXX_FLAGS}")
    # hide symbol
    add_link_options(-fvisibility=hidden)
endif ()

# optimize for Release build
if (CMAKE_BUILD_TYPE MATCHES ".*Rel.*")
    message("Release mode, enabling maximal optimization")
    if (MSVC)
        add_compile_options(/Ox)
    else ()
        add_compile_options(-O3)
    endif (MSVC)
endif (CMAKE_BUILD_TYPE MATCHES ".*Rel.*")

#
set(SRC_PATH ${CMAKE_CURRENT_SOURCE_DIR}/src)
file(GLOB MIRAICP_CPP_PATH ${SRC_PATH}/include/*.cpp)
file(GLOB MIRAICP_COMMON_PATH ${SRC_PATH}/common/*.cpp)
option(GOOGLE_TEST "GOOGLE_TEST" ON)

#
add_compile_definitions(JSON_HAS_CPP_17)

# -- automatic add include --
if (WIN32)
    execute_process(
            COMMAND python ../buildScript/addinclude.py
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (WIN32)
if (UNIX)
    execute_process(
            COMMAND python3 ../buildScript/addinclude.py
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (UNIX)
# -- end --

# -- jni required --
set(JAVA_AWT_LIBRARY NotNeeded)
# set(JAVA_JVM_LIBRARY NotNeeded)
set(JAVA_AWT_INCLUDE_PATH NotNeeded)
find_package(JNI REQUIRED)
if (JNI_FOUND)
    message(STATUS "FOUND JNI IN ${JAVA_INCLUDE_PATH}")
    set(JNI_PATH
            ${JAVA_INCLUDE_PATH}
            ${JAVA_INCLUDE_PATH2}
            )
    set(3RD_PATH
            ${SRC_PATH}/3rd_include/json
            ${SRC_PATH}/3rd_include/json/nlohmann
            ${SRC_PATH}/3rd_include/utf8
            ${SRC_PATH}/3rd_include/utf8/utf8
            )
else ()
    message(STATUS "Cannot find JNI")
endif ()
# -- end --

# -- delete outdated single headers task --
if (WIN32)
    # must use `\\`
    add_custom_target(
            deleteSingleHeaders
            COMMAND del single_include\\MiraiCP\\MiraiCP.cpp
            COMMAND del single_include\\MiraiCP\\MiraiCP.hpp
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (WIN32)
if (UNIX)
    add_custom_target(
            deleteSingleHeaders
            COMMAND rm single_include/MiraiCP/MiraiCP.cpp
            COMMAND rm single_include/MiraiCP/MiraiCP.hpp
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (UNIX)
# -- end --

# -- gengerate single headers --
if (WIN32)
    add_custom_target(
            generateSingleHeaders
            COMMAND python ../buildScript/addinclude.py
            COMMAND python ../buildScript/third-party/config.py
            COMMAND python ../buildScript/third-party/amalgamate.py -c ../buildScript/third-party/config.json -p ../buildScript/third-party/prologue -s . --verbose=yes
            COMMAND python ../buildScript/combineSource.py
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (WIN32)
if (UNIX)
    add_custom_target(
            generateSingleHeaders
            COMMAND python3 ../buildScript/addinclude.py
            COMMAND python3 ../buildScript/third-party/config.py
            COMMAND python3 ../buildScript/third-party/amalgamate.py -c ../buildScript/third-party/config.json -p ../buildScript/third-party/prologue -s . --verbose=yes
            COMMAND python3 ../buildScript/combineSource.py
            WORKING_DIRECTORY ${SRC_PATH}
    )
endif (UNIX)
# -- end --

add_subdirectory(src/include)
add_subdirectory(src/libloader)
if (GOOGLE_TEST)
    message(STATUS "loading test")
    add_subdirectory(src/tests)
endif ()
