#pragma once

#include <SFML/Graphics/Texture.hpp>

#include "asset/asset.hpp"
#include "asset/managerTyped.hpp"

namespace stay 
{
    class Texture : public asset::Asset
    {
        public:
            Texture() = default;
            const sf::Texture& getSfmlTexture() const
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

    using TextureManager = asset::ManagerTyped<std::string, Texture>;
} // namespace stay