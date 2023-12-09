#include "asset.hpp"
#include "utility/variantHelper.hpp"
#include <filesystem>
#include <cassert>
namespace stay
{
    namespace asset
    {
        Asset::Asset(Path baseDirectory, Path relativePath)
            : mBaseDirectory{std::move(baseDirectory)}
            , mRelativePath{std::move(relativePath)}
        {
            assert(std::filesystem::is_directory(mBaseDirectory) && "invalid directory");
            assert(std::filesystem::is_regular_file(absolutePath()) && "not a regular file");
        }
        
        Path Asset::absolutePath() const
        {
            return mBaseDirectory/mRelativePath;
        }

        Path Asset::relativePath() const
        {
            return mRelativePath;
        }

        Path Asset::baseFolder() const
        {
            return mBaseDirectory;
        }

        void Asset::load()
        {
            mLoaded = loadFromPath();
        }

        bool Asset::loaded() const
        {
            return mLoaded;
        }

        void Asset::removeHandler(std::size_t id)
        {
            mOnChange.removeListener(id);
        }
    } // namespace asset
} // namespace stay