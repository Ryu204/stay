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
            
        void Asset::fileChangedHanler(const Action& change)
        {
            std::visit(utils::VariantVisitor{
                [this](const Modify&) {
                    onModify();
                },
                [this](const Move& move) {
                    mRelativePath = move.newRelativePath.string();
                    onMove(absolutePath());
                },
                [](const Delete&) { /*Add code here*/},
                [](const Create&) { /*Add code here*/}
            }, change);
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
    } // namespace asset
} // namespace stay