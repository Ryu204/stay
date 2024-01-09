#include "listener.hpp"
#include "asset.hpp"
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
                , mUpdateInterval{minCallbackIntervalSeconds}
                , mDestructorCalled{false}
            {
                assert(std::filesystem::is_directory(mBaseDirectory) && "not a valid directory");
                mWorkerThread = std::make_unique<std::thread>(&Listener::launch, this);
            }

            Listener::~Listener()
            {
                mDestructorCalled.store(true);
                mWorkerThread->join();
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

            void Listener::launch() 
            {
                sf::Clock clock;
                float elapsed = clock.restart().asSeconds();
                while (!mDestructorCalled.load())
                {
                    elapsed += clock.restart().asSeconds();
                    if (elapsed > mUpdateInterval)
                    {
                        elapsed = std::fmod(elapsed, mUpdateInterval);
                        notify();
                    }
                    const auto halfInterval = std::chrono::milliseconds((int)(mUpdateInterval * 1000.F / 2.F));
                    std::this_thread::sleep_for(halfInterval);
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
            }
        } // namespace detail
    } // namespace asset
} // namespace stay