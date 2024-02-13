# On Windows, `pybind11` generates `*.pyd` library file, which is imported in python script.
# However, Python's way of resolving DLLs has changed since 3.8. See:
# https://docs.python.org/3/whatsnew/3.8.html#bpo-36085-whatsnew
# Thus, even though it's possible to compile with MinGW, Ninja, .etc, user will have to 
# manually resolve DLL directories, for e.g:
# ```py
# import os
# os.add_dll_directory('C:\\My\\MinGW\\installation\\bin\\')
# # The real import step
# import my_cpp_module_with_pybind11
# ```
# Prefer MSVC if possible.

# Check for module name's existence
IF (NOT ${PROJECT_NAME}_PYTHON_MODULE)
    MESSAGE(FATAL_ERROR "Python module name is not defined")
ENDIF()

ADD_SUBDIRECTORY(extern/pybind11)

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

# Patch minor errors
IF (USER_DEFINED_CLANG_TIDY)
    SET(CMAKE_INTERPROCEDURAL_OPTIMIZATION OFF)
ENDIF()
SET_TARGET_PROPERTIES(sfml-graphics PROPERTIES POSITION_INDEPENDENT_CODE TRUE)

# Generate target
MESSAGE("-- Configuring Python binding target")
PYBIND11_ADD_MODULE(
    ${${PROJECT_NAME}_PYTHON_MODULE}
    ${${PROJECT_NAME}_BIND_CODE}
    ${${PROJECT_NAME}_SOURCE_CODE})
SET_TARGET_PROPERTIES(${${PROJECT_NAME}_PYTHON_MODULE} PROPERTIES LINKER_LANGUAGE CXX)