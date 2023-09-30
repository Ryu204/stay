#pragma once

#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cstdarg>

#include "../world/node.hpp"
#include "componentLoader.hpp"

namespace stay
{
    struct SceneLoader
    {
            using Path = typename std::filesystem::path;
            SceneLoader(ecs::Manager* manager, Path&& filepath);
            template <typename T>
            void registerType(const std::string& name)
            {
                mLoader.registerComponent<T>(name);
            }
            // @return Root node
            Uptr<Node> load();
        private:
            static std::string& error();
            Path mFile;
            ComponentsLoader mLoader;
    };
} // namespace stay
