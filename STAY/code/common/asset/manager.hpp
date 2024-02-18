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
        class Manager 
        {
            public:
                Manager(Path rootDirectory);
                template <typename Type, whereIs(Type, Asset)>
                Type& add(std::size_t id, const Path& relativePath)
                {
                    assert(mAssetsList.find(id) == mAssetsList.end() && "id added before");
                    mAssetsList.emplace(id, std::make_unique<Type>());
                    mAssetsList[id]->initPaths(mRootDirectory, relativePath);
                    mWatcher.add(*mAssetsList[id]);

                    return static_cast<Type&>(*mAssetsList[id]);
                }
                template <typename Type, whereIs(Type, Asset)>
                Type& get(std::size_t id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end());
                    auto* derived = dynamic_cast<Type*>(mAssetsList.at(id).get());
                    assert(derived != nullptr && "get wrong type");
                    return *derived;
                }
                void remove(std::size_t id);
            private:
                const Path mRootDirectory;
                utils::IdGenerator mGenerator;
                std::unordered_map<std::size_t, Uptr<Asset>> mAssetsList;
                FolderWatcher mWatcher;
        };
    } // namespace asset
    using AssetManager = asset::Manager;
} // namespace stay