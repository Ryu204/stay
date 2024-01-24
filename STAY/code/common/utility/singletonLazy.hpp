#pragma once

#include <memory>

namespace stay
{
    namespace utils
    {
        template <typename T>
        class Singleton
        {
            public:
                Singleton() = delete;
                virtual ~Singleton() = default;
                template <typename... Args>
                static void init(Args&&... args)
                {
                    std::make_unique<T>(std::forward<Args>(args)...).swap(mInstance());
                }
                static T& get()
                {
                    if (mInstance().get() == nullptr)
                    {
                        std::make_unique<T>().swap(mInstance());
                    }
                    return *mInstance();
                }
            private:
                static std::unique_ptr<T>& mInstance()
                {
                    static std::unique_ptr<T> res;
                    return res;
                };
        };
    } // namespace utils
} // namespace stay
