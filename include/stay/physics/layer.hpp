#pragma once

#include <unordered_map>
#include <string>
#include <cstdint>
#include <vector>

namespace stay
{
    namespace phys
    {
        // @brief Initially, all layers collide pairwise
        class Layer
        {
            public:
                using Mask = std::uint16_t;
                Layer();
                // @brief Set name of a layer
                // @param layer an interger in range [0, 16)
                Layer& set(int layer, std::string&& name);
                // @brief Set `layer1` and `layer2` to collide in scene
                Layer& set(int layer1, int layer2, bool collide = false);
                Layer& set(const std::string& name1, const std::string& name2, bool collide = false);
                Layer& isolate(const std::string& name);
                int getID(const std::string& name) const;
                const std::string& name(int id) const;
                Mask getLayerMask(const std::string& name) const;
                Mask getCollisionMask(const std::string& name) const;
                static Mask getLayerMask(int id);
                Mask getCollisionMask(int id) const;
                void reset();
            private:
                std::unordered_map<int, std::string> mLayerNames;
                std::unordered_map<std::string, int> mLayerBits;
                std::vector<Mask> mMatrix;
        };
    } // namespace phys
} // namespace stay
