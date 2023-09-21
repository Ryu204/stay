#pragma once

#include <array>
#include <queue>

namespace stay
{
	namespace utils
	{
		// Manage ID creation and deletion
		// N is the number of maximum available IDs
        // This class asserts excessively in debug build if the parameters are faulty
		template <std::size_t N>
		class IDGenerator
		{
		public:
			IDGenerator();
			// Generate a new ID.
			std::size_t generate();
			// Erase an ID generated before
			void erase(std::size_t ID);
			// Check if ID is still in active state
			bool isActive(std::size_t ID);

		private:
            void checkInRange(std::size_t ID);

			std::queue<std::size_t> mAvailable;
			std::array<bool, N> mAliveLists;
		};

		template <std::size_t N>
		IDGenerator<N>::IDGenerator()
		{
			for (std::size_t i = 0; i < N; ++i) {
				mAvailable.push(i);
				mAliveLists[i] = false;
			}
		}

		template <std::size_t N>
		std::size_t IDGenerator<N>::generate()
		{
            assert(!mAvailable.empty() && "Max IDs number exceeded");

            const auto res = mAvailable.front();
            mAvailable.pop();
            mAliveLists[res] = true;
            return res;
		}

		template <std::size_t N>
		void IDGenerator<N>::erase(std::size_t ID)
		{
            checkInRange(ID);
            assert(mAliveLists[ID] && "Erase non-existed ID");
			mAliveLists[ID] = false;
			mAvailable.push(ID);
		}

		template <std::size_t N>
		bool IDGenerator<N>::isActive(std::size_t ID)
		{
            checkInRange(ID);
			return mAliveLists[ID];
		}

        template <std::size_t N>
        void IDGenerator<N>::checkInRange(std::size_t ID)
        {
            assert(ID < N && "ID query not in range");
        }
	} // namespace utils
} // namespace stay