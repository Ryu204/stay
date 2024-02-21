#pragma once

#include "stay/asset/folderWatcher.hpp"
#include "type.hpp"
#include "stay/event/event.hpp"
#include "stay/utility/typedef.hpp"

namespace stay
{
    namespace asset
    {
        template <typename Id>
        class Manager;
        template <typename Id, typename T, whereIs(T, Asset)>
        class ManagerTyped;
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
                void initPaths(Path baseDir, Path relative); 
                template <typename Id>
                friend class Manager;
                template <typename Id, typename T, std::enable_if_t<std::is_base_of_v<Asset, T>, bool>>
                friend class ManagerTyped;

                friend detail::Listener;
                event::Event<Action> mOnChange;

                Path mBaseDirectory;
                Path mRelativePath;
                bool mLoaded;
        };
    } // namespace asset
} // namespace stay