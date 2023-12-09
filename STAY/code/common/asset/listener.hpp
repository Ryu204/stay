#pragma once

#include <shared_mutex>
#include <string>
#include <unordered_map>
#include <unordered_set>

#include <efsw/efsw.hpp>

#include "type.hpp"

namespace stay
{

    namespace asset
    {
        class Asset;
        namespace detail
        {
            class Listener : public efsw::FileWatchListener
            {
                public:
                    Listener(Path assetFolder, float minCallBackIntervalSeconds = 1.F);
                    void add(Asset& asset);
                    void update(float dt);
                    void handleFileAction(
                        efsw::WatchID /*watchid*/, const std::string& dir,
						const std::string& filename, efsw::Action action,
						std::string oldFilename = "" ) override;
                private:
                    void notify();
                    const Path mBaseFolder;
                    std::unordered_map<std::string, Asset*> mAssets{};
                    std::unordered_set<Asset*> mModifyQueue{};
                    std::unordered_map<Asset*, Rename> mMoveQueue{};
                    std::vector<Asset*> mDeleteQueue{};

                    std::shared_mutex mModifyMutex{};
                    std::shared_mutex mMoveMutex{};
                    std::shared_mutex mDeleteMutex{};

                    const float mUpdateInterval{};
                    float mCurrentTime{0.F};
            };
        } // namespace detail
    } // namespace asset
} // namespace stay