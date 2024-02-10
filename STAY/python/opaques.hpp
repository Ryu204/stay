#pragma once

#include <pybind11/pybind11.h>

#include "world/node.hpp"

// Opaque types, allow pass by reference in C++ functionss

PYBIND11_MAKE_OPAQUE(stay::Node);