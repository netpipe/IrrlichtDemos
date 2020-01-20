#include "CCompositeEventReceiver.h"

#include <stdexcept>
class EventReceiver : public CCompositeEventReceiver
{
	irr::core::array<CCompositeEventReceiver*> Children;
public:
	virtual bool OnEvent(const SEvent& event)
	{
		for (irr::u32 i=0; i<Children.size();++i)
		{
			if (Children[i]->OnEvent(event)) return true;
		}
		return false;
	}

	void add(CCompositeEventReceiver* newChild)
	{
		Children.push_back(newChild);
	}

	void remove(CCompositeEventReceiver* oldChild)
	{
		s32 idx = Children.binary_search(oldChild);
		if (idx != -1) Children.erase(idx);
		else std::runtime_error("EventReceiver: Not a child");
	}
};
