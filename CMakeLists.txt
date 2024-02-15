CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(STAY VERSION 0.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build or search for dependencies
MESSAGE("-- Fetching dependencies from GitHub")
ADD_SUBDIRECTORY(extern/box2d)
ADD_SUBDIRECTORY(extern/efsw)
ADD_SUBDIRECTORY(extern/entt)
ADD_SUBDIRECTORY(extern/glm)
ADD_SUBDIRECTORY(extern/json)
ADD_SUBDIRECTORY(extern/SFML)

# Collect source files
FILE(GLOB_RECURSE 
    ${PROJECT_NAME}_SOURCE_CODE
    CONFIGURE_DEPENDS
    "STAY/code/*.cpp"
    "STAY/code/*.hpp"
    "STAY/code/*.c"
    "STAY/code/*.h"
    "STAY/code/*.inl")

# Initialize clang-tidy if used
IF(USER_DEFINED_CLANG_TIDY)
    MESSAGE("-- clang-tidy will be used")
    IF (MSVC)
        SET(USER_DEFINED_CLANG_TIDY "${USER_DEFINED_CLANG_TIDY};--extra-arg=/EHsc")
    ENDIF() 
    SET(CMAKE_CXX_CLANG_TIDY "${USER_DEFINED_CLANG_TIDY}")
ENDIF()

# Problem with big obj file size when compiling with MinGW
IF (MINGW)
    MESSAGE("-- Adjusting compiler flags for MinGW")
    ADD_COMPILE_OPTIONS(-Wa,-mbig-obj -Os)
ENDIF()

# Add target and link libraries
ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_CODE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC 
    nlohmann_json::nlohmann_json 
    sfml-graphics 
    sfml-audio
    glm::glm-header-only
    EnTT::EnTT 
    box2d
    efsw
)

# Specific libraries macros
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PUBLIC "GLM_ENABLE_EXPERIMENTAL")

# Add thread support
SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Threads::Threads)

# Add common header to include directory
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/STAY/code/common)

# Copy asset folder
FILE(COPY "${PROJECT_NAME}/asset" DESTINATION ".")
MESSAGE("-- Asset folder is copied to ${CMAKE_BINARY_DIR}/asset")