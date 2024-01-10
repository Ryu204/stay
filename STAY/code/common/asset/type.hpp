#pragma once

#include <filesystem>
#include <variant>

namespace stay 
{
    namespace asset
    {
        using Path = std::filesystem::path;

        enum class Action
        {
            MODIFIED, DELETED
        };
    } // namespace asset
} // namespace stay