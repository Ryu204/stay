#include <pybind11/pybind11.h>

#include "world/node.hpp"
#include "opaques.hpp" // IWYU pragma: keep

namespace stay
{
    namespace py
    {
        void initNode(pybind11::module_& m)
        {
            pybind11::class_<Node>(m, "Node")
                .def("entity", &Node::entity, "Get entity value of this node");
        }
    } // namespace py
} // namespace stay