#include <iostream>

#include <json/json.h>

#include "application.hpp"

int main()
{
    try
    {
        stay::program::Application app;
        app.run();
        return 0;
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << '\n';
        return -1;
    }
    catch(...)
    {
        std::cerr << "Unknown error!\n";
        return -2;
    }
}