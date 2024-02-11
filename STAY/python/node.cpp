#include <pybind11/pybind11.h>

#include "world/node.hpp"

namespace stay
{
    namespace py
    {
        void initNode(pybind11::module_& m)
        {
            pybind11::class_<Node>(m, "Node")
                .def("stray", &Node::stray)
                .def("entity", &Node::entity)
                .def_property("parent", 
                    static_cast<Node*(Node::*)()>(&Node::parent), 
                    static_cast<void (Node::*)(Node*)>(&Node::setParent))
                .def("destroy", static_cast<void(Node::*)(Node*)>(&Node::destroy))
                .def("createChild", static_cast<Node*(Node::*)()>(&Node::createChild), pybind11::return_value_policy::reference);
        }
    } // namespace py
} // namespace stay