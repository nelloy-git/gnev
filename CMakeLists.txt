cmake_minimum_required(VERSION 3.1...3.17 FATAL_ERROR)

project(gnev VERSION 0.01 LANGUAGES C CXX)

add_subdirectory(./3rdparty/glfw)
add_subdirectory(./3rdparty/json)

add_compile_definitions(GLWPP_DEBUG)

if (WIN32)
    add_compile_definitions("IMGUI_API=__declspec(dllexport)")

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zi /WX /D_DEBUG /DEBUG:FULL /std:c++latest")
else ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++23 -fconcepts -pthread -fmax-errors=1 -ggdb")
endif (WIN32)

include_directories(./src)
include_directories(./sample)
include_directories(./3rdparty/bimap)
include_directories(./3rdparty/glad/include)
include_directories(./3rdparty/glfw/include)
include_directories(./3rdparty/json/include)
include_directories(./3rdparty/glm)
include_directories(./3rdparty/stb)
include_directories(./3rdparty/VLD/include)
link_directories(./3rdparty/VLD/lib/Win64)

# Options
set(GNEV_GL_BUFFER_VECTOR_CAP_BASE 1 CACHE STRING "GLBufferVector: number of reserved elements at creation")
set(GNEV_GL_BUFFER_VECTOR_CAP_MULT 2 CACHE STRING "GLBufferVector: capacity multiplier if not enought memory available")
set(GNEV_GL_BUFFER_VECTOR_CAP_ADD  0 CACHE STRING "GLBufferVector: capacity addition if not enought memory available")

set(GNEV_VERTEX_INFO_MAX_ATTRIBUTES  16 CACHE STRING "VertexInfo: maximum number of attribute for vertex")


file(GLOB_RECURSE SRC_FILES "src/**.cpp")
add_library (${PROJECT_NAME} SHARED
    3rdparty/glad/src/gl.c
    ${SRC_FILES}
)
target_compile_definitions(${PROJECT_NAME} PUBLIC
    GNEV_GL_BUFFER_VECTOR_CAP_BASE=${GNEV_GL_BUFFER_VECTOR_CAP_BASE}
    GNEV_GL_BUFFER_VECTOR_CAP_MULT=${GNEV_GL_BUFFER_VECTOR_CAP_MULT}
    GNEV_GL_BUFFER_VECTOR_CAP_ADD=${GNEV_GL_BUFFER_VECTOR_CAP_ADD}
    
    GNEV_VERTEX_INFO_MAX_ATTRIBUTES=${GNEV_VERTEX_INFO_MAX_ATTRIBUTES}
)
target_link_libraries(${PROJECT_NAME}
    nlohmann_json
)
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_20)

set_property (TARGET ${PROJECT_NAME}
  PROPERTY
    # Enable C++20 standard compliance
    CXX_STANDARD 20
)


# Main test

file(GLOB_RECURSE SAMPLE_SRC_FILES "sample/**.cpp")
add_executable(${PROJECT_NAME}_sample
    ${SAMPLE_SRC_FILES}
)
target_compile_features(${PROJECT_NAME}_sample PUBLIC cxx_std_20)

if (WIN32)
    target_link_libraries(${PROJECT_NAME}_sample
        ${PROJECT_NAME}
        glfw
    )
else()
    target_link_libraries(${PROJECT_NAME}_sample
        ${PROJECT_NAME}
        glfw
        dl
    )
endif (WIN32)

# configure_file(shaders/vertex_2d.vs vertex_2d.vs)
# configure_file(shaders/vertex_2d.fs vertex_2d.fs)