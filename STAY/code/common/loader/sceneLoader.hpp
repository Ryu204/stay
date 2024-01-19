#pragma once

#include <unordered_map>
#include <filesystem>

#include "componentLoader.hpp"

namespace stay
{
    class Node;
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
            Uptr<Node> tryLoad(std::string& log);
            Serializable::Data openFile();
            void loadEntity(Node& parent, const Serializable::Data& data);
            Serializable::Data createSaveObject(Node* topNode) const;
            Path mFile;
            ComponentsLoader mLoader;
            std::unordered_map<ecs::Entity, ecs::Entity> mParentOf;
    };
} // namespace stay
