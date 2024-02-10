#include <pybind11/pybind11.h>

#include "ecs/type.hpp"

namespace stay
{
    namespace py
    {
        void initEntity(pybind11::module_& m)
        {
            pybind11::enum_<ecs::Entity>(m, "Entity");
        }
    } // namespace py
} // namespace stay