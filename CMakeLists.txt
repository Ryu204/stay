CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(stay VERSION 0.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

IF (BUILD_SHARED_LIBS)
    SET(CMAKE_POSITION_INDEPENDENT_CODE ON)
ENDIF()

# Build or search for dependencies
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
    "source/*.cpp"
    "source/*.hpp"
    "include/*.hpp"
)

# Clang-tidy analyzer
OPTION(${PROJECT_NAME}_CLANG_TIDY "Enable clang-tidy checks" OFF)
IF (${PROJECT_NAME}_CLANG_TIDY)
    SET(CMAKE_CXX_CLANG_TIDY 
        "clang-tidy" 
        "--config-file=${CMAKE_CURRENT_SOURCE_DIR}/.clang-tidy" 
        "--header-filter=${CMAKE_CURRENT_SOURCE_DIR}/include/*.hpp"
        "--extra-arg=/EHsc")
ENDIF()

# Add target and link libraries
ADD_LIBRARY(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_CODE})

# Create target
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
TARGET_COMPILE_DEFINITIONS(${PROJECT_NAME} PRIVATE "GLM_ENABLE_EXPERIMENTAL")

# Add thread support
SET(THREADS_PREFER_PTHREAD_FLAG TRUE)
FIND_PACKAGE(Threads REQUIRED)
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PRIVATE Threads::Threads)

# Add common header to include directory
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)