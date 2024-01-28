#include "program/application.hpp"

#include <pybind11/pybind11.h>

int run() 
{
    stay::program::Application app;
    app.run();
    return 0;
}

PYBIND11_MODULE(re_stay, m) 
{
    m.def("run", &run, "run the program");
}