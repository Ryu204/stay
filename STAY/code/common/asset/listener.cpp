#include "listener.hpp"
#include "asset.hpp"
#include "utility/invoke.hpp"
#include <SFML/System/Clock.hpp>
#include <cassert>
#include <chrono>
#include <memory>
#include <cmath>

namespace stay
{
    namespace asset
    {
        namespace detail
        {
            Listener::Listener(Path baseDirectory, float minCallbackIntervalSeconds)
                : mBaseDirectory{std::move(baseDirectory)}
            {
                assert(std::filesystem::is_directory(mBaseDirectory) && "not a valid directory");
                mInvokeId = Invoke::after(
                    minCallbackIntervalSeconds, 
                    [this]{notify();}, 
                    /* repeated = */ true
                );
            }

            Listener::~Listener()
            {
                Invoke::removeCallback(mInvokeId);
            }

            void Listener::add(Asset& asset)
            {
                assert(asset.baseFolder() == mBaseDirectory && "asset doesn't have same base dir");
                mAssets[asset.relativePath()] = &asset;
            }

            void Listener::remove(Asset& asset)
            {
                const auto relativePath = asset.relativePath();
                assert(mAssets.find(relativePath) != mAssets.end() && "asset not added");
                mAssets.erase(relativePath);
                {
                    std::lock_guard lock{mDeleteMutex};
                    mDeleteQueue.erase(&asset);
                }
                {
                    std::lock_guard lock{mModifyMutex};
                    mModifyQueue.erase(&asset);
                }
            }

            void Listener::handleFileAction(efsw::WatchID /* id */, const std::string& dir,
                const std::string& filename, efsw::Action action,
                std::string /* oldFilename */)
            {
                const auto filePath = Path(dir)/filename;
                const auto relativePath = std::filesystem::relative(filePath, mBaseDirectory);
                const bool untracked = mAssets.find(relativePath) == mAssets.end();
                if (untracked)
                    return;
                auto* asset = mAssets.at(relativePath);
                if (action == efsw::Action::Delete)
                {
                    std::lock_guard lock{mDeleteMutex};
                    mDeleteQueue.insert(asset);
                    mAssets.erase(relativePath);
                }
                else if (action == efsw::Action::Modified)
                {
                    std::lock_guard lock{mModifyMutex};
                    mModifyQueue.insert(asset);
                }
            }

            void Listener::notify()
            {
                {
                    std::lock_guard lock{mModifyMutex};
                    for (const auto* i : mModifyQueue)
                        i->mOnChange.invoke(Action::MODIFIED);
                    mModifyQueue.clear();
                }
                {
                    std::lock_guard lock{mDeleteMutex};
                    for (const auto* i : mDeleteQueue)
                        i->mOnChange.invoke(Action::DELETED);
                    mDeleteQueue.clear();
                }
            }
        } // namespace detail
    } // namespace asset
} // namespace stay