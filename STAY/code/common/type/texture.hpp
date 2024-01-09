#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "asset/asset.hpp"

namespace stay 
{
    class Texture : public asset::Asset
    {
        public:
            Texture(std::filesystem::path path);
            sf::Texture& getSfmlTexture();
        protected:
            bool loadFromPath() override
            {
                return mInternalTexture.loadFromFile(absolutePath().string());
            }
        private:
            sf::Texture mInternalTexture;
    };
} // namespace stay