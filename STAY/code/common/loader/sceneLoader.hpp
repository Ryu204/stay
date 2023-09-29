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

            SceneLoader(ecs::Manager* manager, Path&& filepath)
                : mFile(filepath)
                , mLoader(manager)
            {}

            template <typename T>
            void registerType(const std::string& name)
            {
                mLoader.registerComponent<T>(name);
            }
            // @return Root node
            Uptr<Node> load()
            {
                Json::Value all;
                std::ifstream(mFile) >> all;
                auto res = std::make_unique<Node>(static_cast<ecs::Entity>(all["root"].asInt()));
                all = all["entities"];
                static std::unordered_map<ecs::Entity, ecs::Entity> parents;
                parents.clear();
                for (const auto& entity : all)
                {
                    auto id = static_cast<ecs::Entity>(entity["id"].asInt());
                    auto* node = res->createChild(id);
                    parents.emplace(id, static_cast<ecs::Entity>(entity["parent"].asInt()));
                    mLoader.loadAllComponents(id, entity["components"]);
                }
                for (const auto [child, parent] : parents)
                {
                    Node::getNode(child)->setParent(parent);
                }
                return res;
            }

        private:
            Path mFile;
            ComponentsLoader mLoader;
    };
} // namespace stay
