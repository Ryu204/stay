#pragma once

#include <unordered_set>
#include <cassert>

namespace stay
{
	namespace utils
	{
		class IdGenerator
		{
		public:
			// Generate a new ID.
			std::size_t generate()
			{
				if (!mCaching.empty())
				{
					std::size_t res = *mCaching.begin();
					mCaching.erase(res);
					mUsing.insert(res);
					return res;
				}
				mUsing.insert(++mCurrentMax);
				return mCurrentMax;
			}
			// Erase an ID generated before
			void erase(std::size_t id)
			{
				assert(mUsing.count(id) != 0 && "erase non existing ID");
				mUsing.erase(id);
				mCaching.insert(id);
			}
			// Check if ID is still in active state
			bool isActive(std::size_t id)
			{
				return mUsing.count(id) > 0;
			}

			void swap(IdGenerator& other) noexcept
			{
				mUsing.swap(other.mUsing);
				mCaching.swap(other.mCaching);
				std::swap(mCurrentMax, other.mCurrentMax);
			}

		private:
            std::unordered_set<std::size_t> mUsing{};
			std::unordered_set<std::size_t> mCaching{};
			std::size_t mCurrentMax{0};
		};
	} // namespace utils
} // namespace stay