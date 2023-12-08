#pragma once

#include "type.hpp"

namespace stay
{
    namespace asset
    {
        class Asset
        {
            public:
                Asset(Path baseDirectory, Path relativePath);
                virtual ~Asset() = default;
                virtual void load() = 0;
                void fileChangedHanler(const Action& change);
                Path absolutePath() const;
                Path relativePath() const;
                Path baseFolder() const;
            protected:
                virtual void onModify() {};
                virtual void onMove(const Path& newRelative) {};
            private:
                Path mBaseDirectory;
                Path mRelativePath;
        };
    } // namespace asset
} // namespace stay