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
        namespace detail
        {
            Listener::Listener(Path assetFolder)
                : mBaseFolder(std::move(assetFolder))
            {
                assert(std::filesystem::is_directory(mBaseFolder) && "invalid directory");
            }

            void Listener::add(Asset& asset)
            {
                assert(asset.baseFolder() == mBaseFolder && "asset is from other base folder");
                // This line is important. We do this instead of using the relative method to ensure uniformity.
                auto&& relativePath = std::filesystem::relative(asset.absolutePath(), mBaseFolder);
                mAssets.emplace(std::forward<Path>(relativePath).string(), &asset);
            }

            void Listener::handleFileAction(efsw::WatchID /*watchid*/, const std::string& dir,
                const std::string& filename, efsw::Action action,
                std::string oldFilename)
            {
                const auto relativePath = std::filesystem::relative(
                    Path(dir)/(action == efsw::Action::Moved ? oldFilename : filename), mBaseFolder);
                std::cout << relativePath.string() << ' ' << action << std::endl; /*debug*/
                const bool untracked = mAssets.find(relativePath) == mAssets.end();
                if (untracked)
                {
                    /*debug*/ std::cout << "untracked" << std::endl;
                    return;
                }
                auto* asset = mAssets.at(relativePath);
                {
                    std::lock_guard lock{mMutex};
                    switch (action)
                    {
                        case efsw::Action::Add:
                            mQueued.emplace(asset, Action{Create{}});
                            break;
                        case efsw::Action::Delete:
                            mQueued.emplace(asset, Action{Delete{}});
                            break;
                        case efsw::Action::Modified:
                            mQueued.emplace(asset, Action{Modify{}});
                            break;
                        case efsw::Action::Moved:
                        {
                            auto&& newRelativePath = std::filesystem::relative(Path(dir)/filename, mBaseFolder);
                            mQueued.emplace(asset, Action{Move{newRelativePath}});

                            // Move the asset to new key
                            mAssets.emplace(std::forward<Path>(newRelativePath.string()), asset);
                            std::cout << newRelativePath << std::endl;
                            mAssets.erase(relativePath.string());
                            break;
                        }
                    }
                }
            }

            void Listener::notify()
            {
                std::lock_guard lock{mMutex};
                for (auto [asset, action] : mQueued)
                    asset->fileChangedHanler(action);
                mQueued.clear();
            }
        } // namespace detail

        FolderWatcher::FolderWatcher(Path folder)
            : mFolderPath{std::move(folder)}
            , mListener{mFolderPath}
        {
            assert(std::filesystem::is_directory(mFolderPath) && "invalid folder address");
            mWatcher.addWatch(mFolderPath.string(), &mListener, true);
            mWatcher.watch();
        }
        
        void FolderWatcher::add(Asset& asset)
        {
            mListener.add(asset);
        }

        void FolderWatcher::flushChanges() 
        {
            mListener.notify();
        }
    } // namespace asset
} // namespace stay