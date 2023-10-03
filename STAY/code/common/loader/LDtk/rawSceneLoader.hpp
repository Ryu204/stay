#pragma once

#include <filesystem>

#include "../../ecs/manager.hpp"

/*
    This class is used in case there is no suitable file found for `SceneLoader`
*/

namespace stay
{
    class RawSceneLoader
    {
        public:
            using Path = typename std::filesystem::path;
            // @return Root node if succeeded, null if failed
            Uptr<Node> load(Path&& filename, const std::string& switchReason);
        private:
            static std::string& error();
            std::unordered_map<ecs::Entity, ecs::Entity> mParentOf;
    };
} // namespace stay
