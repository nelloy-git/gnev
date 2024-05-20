cmake_minimum_required(VERSION 3.1...3.17 FATAL_ERROR)

project(gnev VERSION 0.01 LANGUAGES C CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

enable_testing()

add_subdirectory(./3rdparty/glfw)
add_subdirectory(./3rdparty/json)
add_subdirectory(./3rdparty/gtest)
add_subdirectory(./3rdparty/quill)


add_compile_definitions(GLWPP_DEBUG)

if (WIN32)
    add_compile_definitions("IMGUI_API=__declspec(dllexport)")
    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /WX /D_DEBUG /DEBUG:FULL /std:c++20 ")
else ()
    set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -gdwarf-4")
endif (WIN32)

include_directories(./src)
include_directories(./sample)
include_directories(./3rdparty/bimap)
include_directories(./3rdparty/boost_pp/include)
include_directories(./3rdparty/glad/include)
include_directories(./3rdparty/glfw/include)
include_directories(./3rdparty/json/include)
include_directories(./3rdparty/glm)
include_directories(./3rdparty/magic_enum/include)
include_directories(./3rdparty/pfr/include)
include_directories(./3rdparty/quill/quill/include)
include_directories(./3rdparty/stb)
include_directories(./3rdparty/thread_pool/include())
include_directories(./3rdparty/VLD/include)

link_directories(./3rdparty/VLD/lib/Win64)

file(GLOB_RECURSE SRC_FILES "src/**.cpp")
add_library (${PROJECT_NAME} SHARED
    3rdparty/glad/src/gl.c
    ${SRC_FILES}
)
target_link_libraries(${PROJECT_NAME}
    nlohmann_json
    quill
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_23)
target_compile_options(${PROJECT_NAME} PRIVATE "/Zi" "/WX" "/D_DEBUG" "/DEBUG:FULL")

set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++20 standard compliance
    CXX_STANDARD 20
)

# Auto tests

include_directories(./test/shared)
include_directories(./test/auto)

file(GLOB_RECURSE AUTO_TEST_FILES
    "test/shared/**.cpp"
    "test/auto/**.cpp"
)

add_executable(auto_test
    3rdparty/glad/src/gl.c
    ${AUTO_TEST_FILES}
)
if (WIN32)
    target_link_libraries(auto_test
        ${PROJECT_NAME}
        glfw
        quill
        GTest::gtest_main
        GTest::gmock_main
    )
else()
    target_link_libraries(auto_test
        ${PROJECT_NAME}
        glfw
        dl
        quill
        GTest::gmock_main
    )
endif (WIN32)
target_compile_features(auto_test PUBLIC cxx_std_20)

include(GoogleTest)
gtest_discover_tests(auto_test)

# Manual test

include_directories(./test/shared)
include_directories(./test/manual)

file(GLOB_RECURSE MANUAL_TEST_FILES
    "test/shared/**.cpp"
    "test/manual/**.cpp"
)

add_executable(manual_test
    3rdparty/glad/src/gl.c
    ${MANUAL_TEST_FILES}
)
if (WIN32)
    target_link_libraries(manual_test
        ${PROJECT_NAME}
        glfw
    )
else()
    target_link_libraries(manual_test
        ${PROJECT_NAME}
        glfw
        dl
    )
endif (WIN32)

target_compile_features(manual_test PUBLIC cxx_std_20)