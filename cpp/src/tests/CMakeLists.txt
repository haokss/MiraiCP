# -- Test --
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
include(GoogleTest)
add_subdirectory(plugin/multi)
add_subdirectory(libLoader)
# -- end test --

# todo(ea): 更好的解决方法
# (Antares): 我觉得这样子可以，cmake本身就是个单纯的构建工具，运行测试用一个脚本没啥问题
if (WIN32)
    add_custom_target(AllTests
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target AllTestForPlugin_Multi
            COMMAND ${CMAKE_BINARY_DIR}/AllTestForPlugin_Multi.exe
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target AllTestForLibLoader
            COMMAND ${CMAKE_BINARY_DIR}/AllTestForLibLoader.exe
            )
else()
    add_custom_target(AllTests
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target AllTestForPlugin_Multi
            COMMAND ${CMAKE_BINARY_DIR}/AllTestForPlugin_Multi
            COMMAND ${CMAKE_COMMAND} --build ${CMAKE_BINARY_DIR} --target AllTestForLibLoader
            COMMAND ${CMAKE_BINARY_DIR}/AllTestForLibLoader
            )
endif (WIN32)
