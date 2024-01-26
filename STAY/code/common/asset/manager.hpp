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
                void add(std::size_t id, const Path& relativePath)
                {
                    assert(mAssetsList.find(id) == mAssetsList.end() && "id added before");
                    mAssetsList.emplace(id, std::make_unique<Type>());
                    mAssetsList[id]->initPaths(mRootDirectory, relativePath);
                    mWatcher.add(*mAssetsList[id]);
                }
                template <typename Type, whereIs(Type, Asset)>
                Type& get(std::size_t id)
                {
                    assert(mAssetsList.find(id) != mAssetsList.end());
                    auto* derived = dynamic_cast<Type*>(mAssetsList.at(id).get());
                    assert(derived != nullptr && "get wrong type");
                    return *derived;
                }
                template <typename Type, whereIs(Type, Asset)>
                Type& addGet(Path relativePath, std::size_t& id)
                {
                    id = add<Type>(relativePath);
                    return get<Type>(id);
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