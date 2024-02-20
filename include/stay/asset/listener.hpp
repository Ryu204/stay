#pragma once

#include <unordered_map>
#include <unordered_set>
#include <mutex>

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
                    Listener(Path baseDirectory, float minCallBackIntervalSeconds = 1.F);
                    ~Listener() override;
                    void add(Asset& asset);
                    void remove(Asset& asset);
                    void handleFileAction(
                        efsw::WatchID /* id */, const std::string& dir,
                        const std::string& filename, efsw::Action action,
                        std::string /* oldFilename */) override;
                private:
                    void notify();
                    Path mBaseDirectory;
                    std::unordered_map<Path, Asset*> mAssets{};

                    std::unordered_set<Asset*> mModifyQueue{};
                    std::unordered_set<Asset*> mDeleteQueue{};
                    std::recursive_mutex mModifyMutex{};
                    std::recursive_mutex mDeleteMutex{};

                    std::size_t mInvokeId{};
            };
        } // namespace detail
    } // namespace asset
} // namespace stay