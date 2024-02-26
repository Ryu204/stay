#include "stay/physics/layer.hpp"

#include <cassert>

namespace stay
{
    namespace phys
    {
        Layer::Layer()
            : mMatrix(16, 0xFFFF)
        { }

        Layer& Layer::set(int layer, std::string&& name)
        {
            assert(layer >= 0 && layer < 16 && "physics layer out of range");
            mLayerBits.emplace(std::forward<std::string>(name), layer);
            mLayerNames.emplace(layer, std::forward<std::string>(name));
            return *this;
        }

        Layer& Layer::set(int layer1, int layer2, bool collide)
        {
            assert(layer1 >= 0 && layer1 < 16 && layer2 >= 0 && layer2 < 16 && "physics layer out of range");
            if (collide)
            {
                mMatrix[layer1] |= (1 << layer2);
                mMatrix[layer2] |= (1 << layer1);
            }
            else
            {
                mMatrix[layer2] &= ~(1 << layer1);
                mMatrix[layer1] &= ~(1 << layer2);
            }
            return *this;
        }

        Layer& Layer::set(const std::string& name1, const std::string& name2, bool collide)
        {
            return set(mLayerBits.at(name1), mLayerBits.at(name2), collide);
        }

        Layer& Layer::isolate(const std::string& name)
        {
            mMatrix.at(mLayerBits.at(name)) = 0;
            return *this;
        }

        int Layer::getID(const std::string& name) const
        {
            return mLayerBits.at(name);
        }

        const std::string& Layer::name(int id) const
        {
            return mLayerNames.at(id);
        }

        Layer::Mask Layer::getLayerMask(const std::string& name) const
        {
            Mask res = 0;
            res |= (1 << mLayerBits.at(name));
            return res;
        }

        Layer::Mask Layer::getLayerMask(int id)
        {
            assert(id >= 0 && id < 16 && "invalid layer range");
            Mask res = 0;
            res |= (1 << id);
            return res;
        }

        Layer::Mask Layer::getCollisionMask(const std::string& name) const
        {
            return mMatrix.at(mLayerBits.at(name));
        }

        Layer::Mask Layer::getCollisionMask(int id) const
        {
            return mMatrix.at(id);
        }

        void Layer::reset() 
        {
            mLayerBits.clear();
            mLayerNames.clear();
        }
    } // namespace phys
} // namespace stay
