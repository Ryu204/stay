#include "asset.hpp"
#include "utility/variantHelper.hpp"
#include <filesystem>
#include <cassert>

namespace stay
{
    namespace asset
    {
        void Asset::initPaths(Path baseDir, Path relative)
        {
            assert(std::filesystem::is_directory(baseDir) && "invalid directory");
            assert(std::filesystem::is_regular_file(baseDir/relative) && "not a regular file");
            mBaseDirectory = std::move(baseDir);
            mRelativePath = std::move(relative);
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

        bool Asset::load()
        {
            mLoaded = loadFromPath();
            return mLoaded;
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