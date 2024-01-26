#pragma once

#include <filesystem>

#include "ecs/manager.hpp"

namespace stay
{
    class Node;
    struct SceneLoader
    {
            using Path = typename std::filesystem::path;
            SceneLoader(ecs::Manager& manager, Path&& filepath);
            // @return Root node
            Uptr<Node> load();
            void save(Node* root, bool overrideIn = false) const;
        private:
            Uptr<Node> tryLoad(std::string& log);
            Serializable::Data openFile();
            void loadEntity(Node& parent, const Serializable::Data& data);
            Serializable::Data createSaveObject(Node* topNode) const;

            Path mFile;
            std::unordered_map<ecs::Entity, ecs::Entity> mParentOf;
            ecs::Manager& mManager;
    };
} // namespace stay
