#include <pybind11/pybind11.h>

#include "config.hpp"
#include "program/application.hpp"
#include "opaques.hpp" // IWYU pragma: keep

namespace stay
{
    namespace py
    {
        void initEntity(pybind11::module_& m);
        void initNode(pybind11::module_& m);
        void initLoader(pybind11::module_& m);

        PYBIND11_MODULE(STAY_MODULE_NAME, m)
        {
            initEntity(m);
            initNode(m);
            initLoader(m);
            m.def("run", []() { program::Application().run(); }, "Run the game");
        }
    }
} // namespace stay