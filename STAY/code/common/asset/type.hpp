#pragma once

#include <filesystem>
#include <variant>

namespace stay 
{
    namespace asset
    {
        using Path = std::filesystem::path;
        
        struct Modify   { int foo; };
        struct Move     { std::filesystem::path newRelativePath{"HelloWorld.txt"}; };
        struct Delete   { int foo; };
        struct Create   { int foo;};
        using Action = std::variant<Modify, Move, Delete, Create>;
    } // namespace asset
} // namespace stay