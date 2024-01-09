#include "manager.hpp"
#include <cassert>

namespace stay 
{
    namespace asset 
    {
        Manager::Manager(Path rootDirectory)
            : mRootDirectory{std::move(rootDirectory)}
            , mWatcher(mRootDirectory)
        { }
        
        void Manager::remove(std::size_t id)
        {
            assert(mAssetsList.find(id) != mAssetsList.end() && "invalid asset id");
            mGenerator.erase(id);
            mWatcher.remove(*mAssetsList[id]);
            mAssetsList.erase(id);
        }
    } // namespace asset
} // namespace stay