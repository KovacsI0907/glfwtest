cmake_minimum_required(VERSION 3.10)
project(gameengine)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add GLFW
add_subdirectory(external/glfw)
include_directories(external/glfw/include)

# Add GLAD manually
set(GLAD_SOURCE external/glad/src/glad.c)
set(GLAD_INCLUDE_DIR external/glad/include)
include_directories(${GLAD_INCLUDE_DIR})

# Include directory for your headers
include_directories(includes)

# Add glm
include_directories(external/glm)

include_directories(${CMAKE_SOURCE_DIR}/external)

# Source files
file(GLOB_RECURSE SOURCES src/*.cpp)
file(GLOB_RECURSE HEADERS includes/*.h includes/*.hpp)

# Define the executable
add_executable(${PROJECT_NAME} ${SOURCES} ${HEADERS} ${GLAD_SOURCE} )

# Link dependencies
target_link_libraries(${PROJECT_NAME} glfw)

# Copy gamefiles directory contents to the build directory
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/gamefiles
        $<TARGET_FILE_DIR:${PROJECT_NAME}>
)
