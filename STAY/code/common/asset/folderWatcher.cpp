#include "folderWatcher.hpp"
#include "asset.hpp"
#include "asset/type.hpp"
#include <filesystem>
#include <cassert>
#include <iostream> /*debug*/
#include <mutex>

namespace stay
{
    namespace asset
    {
        FolderWatcher::FolderWatcher(Path folder, float minCallbackIntervalSeconds)
            : mFolderPath{std::move(folder)}
            , mListener{mFolderPath, minCallbackIntervalSeconds}
        {
            assert(std::filesystem::is_directory(mFolderPath) && "invalid folder address");
            mWatcher.addWatch(mFolderPath.string(), &mListener, true);
            mWatcher.watch();
        }
        
        void FolderWatcher::add(Asset& asset)
        {
            mListener.add(asset);
        }

        void FolderWatcher::update(float dt)
        {
            mListener.update(dt);
        }
    } // namespace asset
} // namespace stay