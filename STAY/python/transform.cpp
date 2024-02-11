#include <pybind11/pybind11.h>

#include "world/transform.hpp"

namespace stay
{
    namespace py
    {
        void initTransform(pybind11::module_& m)
        {
            pybind11::class_<Transform>(m, "Transform")
                .def(pybind11::init<>())
                .def("__repr__", [](const Transform& v) { return v.serialize().dump(); })
                .def("move", static_cast<void (Transform::*)(const Vector2&)>(&Transform::move))
                .def_property("position", &Transform::getPosition, 
                    static_cast<void (Transform::*)(const Vector3& pos)>(&Transform::setPosition))
                .def("rotate", &Transform::rotate)
                .def_property("rotation", &Transform::getRotation, &Transform::setRotation);
        }
    } // namespace py
} // namespace stay