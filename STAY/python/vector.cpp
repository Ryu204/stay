#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include "type/vector.hpp"

namespace stay
{
    namespace py
    {
        void initVector(pybind11::module_& m)
        {
            pybind11::class_<Vector2>(m, "Vec2")
                .def(pybind11::init<float, float>())
                .def(pybind11::init<Vector3>())
                .def(pybind11::init<>())
                .def("__repr__", [](const Vector2& v) { return v.serialize().dump(); })
                .def_readwrite("x", &Vector2::x)
                .def_readwrite("y", &Vector2::y)
                .def("__mul__", [](const Vector2& v, float s) { return Vector2{v * s}; }, pybind11::is_operator())
                .def("__imul__", [](Vector2& v, float s) { v *= s; return v; }, pybind11::is_operator())
                .def("__rmul__", [](const Vector2& v, float s) { return Vector2{ v * s}; }, pybind11::is_operator())
                .def("__truediv__", [](const Vector2& v, float s) { assert(s != 0.F); return Vector2{v / s}; }, pybind11::is_operator())
                .def("__itruediv__", [](Vector2& v, float s) { v /= s; return v; }, pybind11::is_operator())
                .def("__add__", [](const Vector2& v, const Vector2& other) { return Vector2{v + other}; }, pybind11::is_operator())
                .def("__iadd__", [](Vector2& v, const Vector2& other) { v = v + other; return v; }, pybind11::is_operator())
                .def("__sub__", [](const Vector2& v, const Vector2& other) { return Vector2{v - other}; }, pybind11::is_operator())
                .def("__isub__", [](Vector2& v, const Vector2& other) { v = v - other; return v; }, pybind11::is_operator())
                .def("__neg__", [](const Vector2& v) { return Vector2{-v}; }, pybind11::is_operator());

                pybind11::class_<Vector3>(m, "Vec3")
                .def(pybind11::init<float, float, float>())
                .def(pybind11::init<>())
                .def("__repr__", [](const Vector3& v) { return v.serialize().dump(); })
                .def_readwrite("x", &Vector3::x)
                .def_readwrite("y", &Vector3::y)
                .def_readwrite("z", &Vector3::z)
                .def("__mul__", [](const Vector3& v, float s) { return Vector3{v * s}; }, pybind11::is_operator())
                .def("__imul__", [](Vector3& v, float s) { v *= s; return v; }, pybind11::is_operator())
                .def("__rmul__", [](const Vector3& v, float s) { return Vector3{ v * s}; }, pybind11::is_operator())
                .def("__truediv__", [](const Vector3& v, float s) { assert(s != 0.F); return Vector3{v / s}; }, pybind11::is_operator())
                .def("__itruediv__", [](Vector3& v, float s) { v /= s; return v; }, pybind11::is_operator())
                .def("__add__", [](const Vector3& v, const Vector3& other) { return Vector3{v + other}; }, pybind11::is_operator())
                .def("__iadd__", [](Vector3& v, const Vector3& other) { v = v + other; return v; }, pybind11::is_operator())
                .def("__sub__", [](const Vector3& v, const Vector3& other) { return Vector3{v - other}; }, pybind11::is_operator())
                .def("__isub__", [](Vector3& v, const Vector3& other) { v = v - other; return v; }, pybind11::is_operator())
                .def("__neg__", [](const Vector3& v) { return Vector3{-v}; }, pybind11::is_operator());
        }
    } // namespace py
} // namespace stay