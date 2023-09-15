CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(STAY VERSION 0.1)

SET(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)


# Build or search for dependencies
# Disable clang-tidy when building 
ADD_SUBDIRECTORY(dep)

FILE(GLOB_RECURSE 
    ${PROJECT_NAME}_SOURCE_CODE
    CONFIGURE_DEPENDS
    "STAY/code/*.cpp"
    "STAY/code/*.hpp"
    "STAY/code/*.c"
    "STAY/code/*.h")

# clang-tidy
IF (${CMAKE_CXX_CLANG_TIDY} STREQUAL "SET")
    SET(LINTING_FOLDER "${PROJECT_NAME}/code")
    SET(CMAKE_CXX_CLANG_TIDY "clang-tidy")#--header-filter=${LINTING_FOLDER}/*.hpp")
    MESSAGE("-- Clang-tidy header filter: ${LINTING_FOLDER}")
ELSE()
    MESSAGE("-- Clang-tidy disabled")
    SET(CMAKE_CXX_CLANG_TIDY "")
ENDIF()

# Add target and link libraries
ADD_EXECUTABLE(${PROJECT_NAME} ${${PROJECT_NAME}_SOURCE_CODE})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} PUBLIC LDtkLoader sfml-graphics sfml-audio jsoncpp_lib glm::glm EnTT::EnTT)
