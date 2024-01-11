#pragma once

#include "asset/folderWatcher.hpp"
#include "type.hpp"
#include "event/event.hpp"

namespace stay
{
    namespace asset
    {
        class Manager;
        class Asset
        {
            public:
                using Handler = std::function<void(const Action&)>;
                Asset() = default;
                virtual ~Asset() = default;
                Path absolutePath() const;
                Path relativePath() const;
                Path baseFolder() const;
                bool load();
                bool loaded() const;

                template <typename Func>
                std::size_t addHandler(Func&& handler)
                {
                    return mOnChange.addEventListener<Func>(std::forward<Func>(handler));
                }
                void removeHandler(std::size_t id);
            protected:
                virtual bool loadFromPath() = 0;
            private:
                friend class Manager;
                void initPaths(Path baseDir, Path relative); 

                friend detail::Listener;
                event::Event<Action> mOnChange;

                Path mBaseDirectory;
                Path mRelativePath;
                bool mLoaded;
        };
    } // namespace asset
} // namespace stay