#ifndef _I_REFERENCE_COUNTER_H_
#define _I_REFERENCE_COUNTER_H_

namespace zen
{
	class IReferenceCounter
	{
	public:

		IReferenceCounter()
			: References(1)
		{
		}

		virtual ~IReferenceCounter()
		{
		}

		void grab() const { ++References; }

		bool drop() const
		{
			--References;
			if (!References)
			{
				delete this;
				return true;
			}

			return false;
		}

	irr::s32 getReferenceCount() const
		{
			return References;
		}

	private:

		mutable irr::s32 References;
	};
}

#endif