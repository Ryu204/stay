#pragma once

#include <filesystem>
#include <variant>

namespace stay 
{
    namespace asset
    {
        using Path = std::filesystem::path;
        
        struct Modify   { int foo; };
        struct Delete   { int foo; };
        using Action = std::variant<Modify, Delete>;

        enum ActionIndex
        {
            MODIFIED, DELETED
        };
    } // namespace asset
} // namespace stay