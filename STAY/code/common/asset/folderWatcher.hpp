#pragma once

#include <memory>
#include <mutex>

#include <efsw/efsw.hpp>
#include <thread>

#include "type.hpp"
#include "listener.hpp"

namespace stay
{

    namespace asset
    {
        class Asset;
        /// @brief Watches a folder and callbacks at changes
        class FolderWatcher
        {
            public:
                FolderWatcher(const Path& folder, float watchInterval = 1.F);
                void add(Asset& asset);
                void remove(Asset& asset);
            private:
                std::filesystem::path mFolderPath;
                efsw::FileWatcher mWatcher;
                detail::Listener mListener;
        };
    } // namespace asset
} // namespace stay