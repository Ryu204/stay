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
            SceneLoader& registerComponent(const std::string& name)
            {
                mLoader.registerComponent<T>(name);
                return *this;
            }
            // @return Root node
            Uptr<Node> load();
            void save(Node* root, bool overrideIn = false) const;
        private:
            Node* tryLoad(std::string& log);
            Json::Value openFile();
            void loadEntity(Node* currentRoot, const Json::Value& data);
            Json::Value createSaveObject(Node* root) const;
            Path mFile;
            ComponentsLoader mLoader;
            std::unordered_map<ecs::Entity, ecs::Entity> mParentOf;
    };
} // namespace stay
