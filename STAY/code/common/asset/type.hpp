#pragma once

#include <filesystem>
#include <variant>

namespace stay 
{
    namespace asset
    {
        using Path = std::filesystem::path;
        
        struct Modify   { int foo; };
        struct Rename     { std::filesystem::path newRelativePath{"HelloWorld.txt"}; };
        struct Delete   { int foo; };
        using Action = std::variant<Modify, Rename, Delete>;

        enum ActionIndex
        {
            MODIFIED, RENAMED, DELETED
        };
    } // namespace asset
} // namespace stay