# Check for module name's existence
IF (NOT ${PROJECT_NAME}_PYTHON_MODULE)
    MESSAGE(FATAL_ERROR "Python module name is not defined")
ENDIF()

# Download `pybind11`
FETCHCONTENT_DECLARE(
  pybind
  GIT_REPOSITORY        https://github.com/pybind/pybind11
  GIT_TAG               "master"
  SYSTEM
)
FETCHCONTENT_MAKEAVAILABLE(pybind)

# Generate config header file
CONFIGURE_FILE(
    "${PROJECT_SOURCE_DIR}/STAY/python/config.hpp.in" 
    "${PROJECT_SOURCE_DIR}/STAY/python/config.hpp")

# Glob binding code
FILE(GLOB_RECURSE
    ${PROJECT_NAME}_BIND_CODE
    CONFIGURE_DEPENDS
    "STAY/python/*.hpp"
    "STAY/python/*.cpp")

# Generate target
MESSAGE("-- Configuring Python binding target")
IF (USER_DEFINED_CLANG_TIDY)
    SET(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
ENDIF()
PYBIND11_ADD_MODULE(
    ${${PROJECT_NAME}_PYTHON_MODULE}
    ${${PROJECT_NAME}_BIND_CODE}
    ${${PROJECT_NAME}_SOURCE_CODE})
SET_TARGET_PROPERTIES(${${PROJECT_NAME}_PYTHON_MODULE} PROPERTIES LINKER_LANGUAGE CXX)