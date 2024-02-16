#pragma once

#include <filesystem>

#include <LDtkLoader/Project.hpp>

#include "loader/iLoader.hpp"

namespace stay
{
    class Node;
    class Loader : public ILoader
    {
        public:
            void load(std::filesystem::path&& filename, Node* root) override;
        private:
            struct Detail 
            {
                float pixelsPerMeter;
                Vector2 worldOffset;
            } mDetail;

            Vector2 toWorldPosition(const Vector2& filePosition) const;

            void loadTileset(Node* parent, const ldtk::Layer& layer) const;
            void loadColliders(Node* parent, const ldtk::Layer& layer) const;
            void loadPlayer(Node* parent, const ldtk::Layer& layer) const;
    };
} // namespace stay
