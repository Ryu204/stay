#include <iostream>

#include <json/json.h>

#include "application.hpp"
#include "../world/node.hpp"

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

// test
int main1()
{
    try
    {
        auto* node1 = stay::Node::root().createEmptyChild();
        auto* node2 = stay::Node::root().createEmptyChild();
        auto* node3 = stay::Node::create();

        node3->setParent(node1);
        node1->setParent(node2);
        std::cout << node3->isChildOf(node2) << node2->isParentOf(node3) << std::endl;
        stay::Node::destroy(node2);
        std::cout << "about to kill node2\n";
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    return -1;
}