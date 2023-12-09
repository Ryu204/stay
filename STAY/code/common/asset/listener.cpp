#include "listener.hpp"
#include "asset.hpp"
#include <cassert>
#include <mutex>

namespace stay
{
    namespace asset
    {
        namespace detail
        {
            Listener::Listener(Path assetFolder, float minCallbackIntervalSeconds)
                : mBaseFolder(std::move(assetFolder))
                , mUpdateInterval{minCallbackIntervalSeconds}
            {
                assert(std::filesystem::is_directory(mBaseFolder) && "invalid directory");
            }

            void Listener::add(Asset& asset)
            {
                assert(asset.baseFolder() == mBaseFolder && "asset is from other base folder");
                // This line is important. We do this instead of directly using the relative path method to ensure uniformity.
                auto relativePath = std::filesystem::relative(asset.absolutePath(), mBaseFolder);
                mAssets.emplace(relativePath.string(), &asset);
            }

            void Listener::handleFileAction(efsw::WatchID /*watchid*/, const std::string& dir,
                const std::string& filename, efsw::Action action,
                std::string oldFilename)
            {
                const auto oldName = Path(dir)/(action == efsw::Action::Moved ? oldFilename : filename);
                const auto relativePath = std::filesystem::relative(oldName, mBaseFolder);
                const bool untracked = mAssets.find(relativePath) == mAssets.end();
                if (untracked)
                    return;
                auto* asset = mAssets.at(relativePath);
                if (action == efsw::Action::Delete)
                {
                    std::lock_guard lock{mDeleteMutex};
                    mDeleteQueue.emplace_back(asset);
                    mAssets.erase(relativePath);
                }
                else if (action == efsw::Action::Modified)
                {
                    std::lock_guard lock{mModifyMutex};
                    mModifyQueue.insert(asset);
                }
                else if (action == efsw::Action::Moved)
                {
                    const auto newName = Path(dir)/filename;
                    {
                        std::lock_guard lock{mMoveMutex};
                        mMoveQueue[asset] = Rename{std::filesystem::relative(newName, mBaseFolder)};
                    }
                    mAssets.emplace(std::filesystem::relative(newName, mBaseFolder).string(), asset);
                    mAssets.erase(relativePath.string());
                }
            }

            void Listener::update(float dt)
            {
                mCurrentTime += dt;
                if (mCurrentTime > mUpdateInterval)
                {
                    mCurrentTime -= mUpdateInterval;
                    notify();
                }
            }

            void Listener::notify()
            {
                {
                    std::lock_guard lock{mModifyMutex};
                    for (const auto* i : mModifyQueue)
                        i->mOnChange.invoke(Modify{});
                    mModifyQueue.clear();
                }
                {
                    std::lock_guard lock{mDeleteMutex};
                    for (const auto* i : mDeleteQueue)
                        i->mOnChange.invoke(Delete{});
                    mDeleteQueue.clear();
                }
                {
                    std::lock_guard lock{mMoveMutex};
                    for (const auto& [asset, move] : mMoveQueue)
                    {
                        asset->mRelativePath = move.newRelativePath;
                        asset->mOnChange.invoke(move);
                    }
                    mMoveQueue.clear();
                }
            }
        } // namespace detail
    } // namespace asset
} // namespace stay