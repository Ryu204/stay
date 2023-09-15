#pragma once

namespace stay
{
    namespace utils
    {
        template <typename T>
        struct Assignable
        {
                void assign(T val)
                {
                    mInternal = val;
                    mAssigned |= true;
                    postAssignment();
                };
                const T& get()
                {
                    assert(mAssigned && "unassigned value");   
                    return mInternal;
                }
                bool assigned() const
                {
                    return mAssigned;
                }
            protected:
                virtual void postAssignment() {}
            private:
                bool mAssigned{false};
                T mInternal{static_cast<T>(0)};
        };
    } // namespace utils
    
} // namespace stay
