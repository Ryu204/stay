#pragma once

#include <memory>
#include <cassert>

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
                static void shutdown()
                {
                    assert(mInstance().get() != nullptr && "Call init first");
                    mInstance().reset();
                }
                static T& get()
                {
                    assert(mInstance().get() != nullptr && "call init first");
                    return *mInstance();
                }
                static bool avail()
                {
                    return mInstance().get() != nullptr;
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
