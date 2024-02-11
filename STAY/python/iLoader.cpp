#include <pybind11/pybind11.h>

#include "loader/iLoader.hpp"

namespace stay 
{
    namespace py
    {
        class PyILoader : public ILoader
        {
            public:
                using ILoader::ILoader;
                void load(const std::filesystem::path& path, Node* node) override
                {
                    PYBIND11_OVERRIDE(void, ILoader, load, path, node);
                }
        };
        void initLoader(pybind11::module_& m)
        {
            pybind11::class_<std::filesystem::path>(m, "FilePath")
                .def(pybind11::init<std::string>());
            pybind11::implicitly_convertible<std::string, std::filesystem::path>();
            pybind11::class_<ILoader, std::shared_ptr<ILoader>, PyILoader>(m, "ILoader")
                .def(pybind11::init<>())
                .def("load", &ILoader::load);
            m.def("setLoader", 
                [](ILoader& ref){ setLoader(ref.shared_from_this()); }, 
                "Set the default scene loader");
        }
    } // namespace py
} // namespace stay
