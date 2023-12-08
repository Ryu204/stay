#pragma once

#include <mutex>
#include <string>
#include <unordered_map>

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
                    Listener(Path assetFolder);
                    void add(Asset& asset);
                    void handleFileAction(
                        efsw::WatchID /*watchid*/, const std::string& dir,
						const std::string& filename, efsw::Action action,
						std::string oldFilename = "" ) override;
                    void notify();
                private:
                    std::recursive_mutex mMutex;
                    Path mBaseFolder;
                    std::unordered_map<std::string, Asset*> mAssets{};
                    std::unordered_map<Asset*, Action> mQueued{};
            };
        } // namespace detail

        // @brief Watches a folder and callbacks at changes
        class FolderWatcher
        {
            public:
                FolderWatcher(Path folder);
                void add(Asset& asset);
                void flushChanges();
            private:
                std::filesystem::path mFolderPath;
                efsw::FileWatcher mWatcher;
                detail::Listener mListener;
        };
    } // namespace asset
} // namespace stay