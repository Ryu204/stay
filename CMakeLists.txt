CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(STAY VERSION 0.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)

# Build or search for dependencies
INCLUDE(CMakeDependencies)

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
    SET(CMAKE_CXX_CLANG_TIDY "${USER_DEFINED_CLANG_TIDY}")
ENDIF()

# Add target and link libraries
ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_CODE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC LDtkLoader sfml-graphics sfml-audio jsoncpp_lib glm::glm EnTT::EnTT box2d)

# Copy asset folder
FILE(COPY "${PROJECT_NAME}/asset" DESTINATION ".")