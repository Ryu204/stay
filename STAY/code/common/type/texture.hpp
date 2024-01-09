#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "asset/asset.hpp"

namespace stay 
{
    class Texture : public asset::Asset
    {
        public:
            Texture() = default;
            const sf::Texture& getSfmlTexture()
            {
                return mInternalTexture;
            }
        protected:
            bool loadFromPath() override
            {
                return mInternalTexture.loadFromFile(absolutePath().string());
            }
        private:
            sf::Texture mInternalTexture;
    };
} // namespace stay