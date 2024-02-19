#pragma once

#include <unordered_map>

#include "utility/IDgen.hpp"
#include "utility/typedef.hpp"
#include "type.hpp"
#include "folderWatcher.hpp"
#include "asset.hpp"

namespace stay 
{
    namespace asset 
    {
        template <typename Id, typename Type, std::enable_if_t<std::is_base_of_v<Asset, Type>, bool>>
        class ManagerTyped
        {
            public:
                ManagerTyped(Path rootDirectory)
                    : mRootDirectory{std::move(rootDirectory)}
                    , mWatcher(mRootDirectory)
                { }
                Type& add(const Id& id, const Path& relativePath)
                {
                    assert(mAssetsList.find(id) == mAssetsList.end() && "id added before");
                    mAssetsList.emplace(id, std::make_unique<Type>());
                    mAssetsList[id]->initPaths(mRootDirectory, relativePath);
                    mWatcher.add(*mAssetsList[id]);

                    return *mAssetsList[id];
                }
                Type& get(const Id& id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end());
                    return *mAssetsList[id];
                }
                void remove(const Id& id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end() && "invalid asset id");
                    mGenerator.erase(id);
                    mWatcher.remove(*mAssetsList[id]);
                    mAssetsList.erase(id);
                }
                bool has(const Id& id) const
                {
                    return mAssetsList.find(id) != mAssetsList.end();
                }
            private:
                const Path mRootDirectory;
                utils::IdGenerator mGenerator;
                std::unordered_map<Id, Uptr<Type>> mAssetsList;
                FolderWatcher mWatcher;
        };
    } // namespace asset
} // namespace stay