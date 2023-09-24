#pragma once

#include <unordered_set>

namespace stay
{
	namespace utils
	{
		// Manage ID creation and deletion
		// N is the number of maximum available IDs
        // This class asserts excessively in debug build if the parameters are faulty
		class IDGenerator
		{
		public:
			// Generate a new ID.
			std::size_t generate()
			{
				if (mCaching.size() > 0)
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
			void erase(std::size_t ID)
			{
				assert(mUsing.count(ID) != 0 && "erase non existing ID");
				mUsing.erase(ID);
				mCaching.insert(ID);
			}
			// Check if ID is still in active state
			bool isActive(std::size_t ID)
			{
				return mUsing.count(ID) > 0;
			}

		private:
            std::unordered_set<std::size_t> mUsing{};
			std::unordered_set<std::size_t> mCaching{};
			std::size_t mCurrentMax{0};
		};
	} // namespace utils
} // namespace stay