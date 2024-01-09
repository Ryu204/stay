#include "folderWatcher.hpp"
#include "asset.hpp"
#include "asset/type.hpp"
#include <SFML/System/Clock.hpp>
#include <filesystem>
#include <cassert>

namespace stay
{
    namespace asset
    {
        FolderWatcher::FolderWatcher(const Path& folder, float watchInterval)
            : mFolderPath{folder}
            , mListener{folder, watchInterval}
        {
            assert(std::filesystem::is_directory(mFolderPath) && "invalid directory");
            mWatcher.addWatch(mFolderPath.string(), &mListener, true);
            mWatcher.watch();
        }
        
        void FolderWatcher::add(Asset& asset)
        {
            mListener.add(asset);
        }

        void FolderWatcher::remove(Asset& asset)
        {
            mListener.remove(asset);
        }
    } // namespace asset
} // namespace stay