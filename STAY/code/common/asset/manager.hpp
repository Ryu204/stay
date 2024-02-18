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
        template <typename Id>
        class Manager 
        {
            public:
                Manager(Path rootDirectory)
                    : mRootDirectory{std::move(rootDirectory)}
                    , mWatcher(mRootDirectory)
                { }
                template <typename Type, whereIs(Type, Asset)>
                Type& add(const Id& id, const Path& relativePath)
                {
                    assert(mAssetsList.find(id) == mAssetsList.end() && "id added before");
                    mAssetsList.emplace(id, std::make_unique<Type>());
                    mAssetsList[id]->initPaths(mRootDirectory, relativePath);
                    mWatcher.add(*mAssetsList[id]);

                    return static_cast<Type&>(*mAssetsList[id]);
                }
                template <typename Type, whereIs(Type, Asset)>
                Type& get(const Id& id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end());
                    auto* derived = dynamic_cast<Type*>(mAssetsList.at(id).get());
                    assert(derived != nullptr && "get wrong type");
                    return *derived;
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
                std::unordered_map<Id, Uptr<Asset>> mAssetsList;
                FolderWatcher mWatcher;
        };
    } // namespace asset
    using AssetManager = asset::Manager<std::string>;
} // namespace stay