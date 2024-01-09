#pragma once

#include <cstddef>
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
        template <typename Type, whereIs(Type, Asset)>
        class ManagerTyped
        {
            public:
                ManagerTyped(Path rootDirectory)
                    : mRootDirectory{std::move(rootDirectory)}
                    , mWatcher(mRootDirectory)
                { }
                void add(std::size_t id, const Path& relativePath)
                {
                    assert(mAssetsList.find(id) == mAssetsList.end() && "id added before");
                    mAssetsList.emplace(id, std::make_unique<Type>());
                    mAssetsList[id]->initPaths(mRootDirectory, relativePath);
                    mWatcher.add(*mAssetsList[id]);
                }
                Type& get(std::size_t id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end());
                    return *mAssetsList.at(id);
                }
                Type& addGet(Path relativePath, std::size_t& id)
                {
                    id = add(relativePath);
                    return get(id);
                }
                void remove(std::size_t id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end() && "invalid asset id");
                    mGenerator.erase(id);
                    mWatcher.remove(*mAssetsList[id]);
                    mAssetsList.erase(id);
                }
            private:
                const Path mRootDirectory;
                utils::IDGenerator mGenerator;
                std::unordered_map<std::size_t, Uptr<Type>> mAssetsList;
                FolderWatcher mWatcher;
        };
    } // namespace asset
    using AssetManager = asset::Manager;
} // namespace stay