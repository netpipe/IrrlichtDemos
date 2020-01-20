
#include "InputProfile.h"
#include "IEngine.h"
#include "IWantInput.h"
#include "Event.h"

InputProfile::InputProfile(s32 buttonCount, s32 axesCount, IEngine *engine)
{
	this->engine = engine;
	
	lastRealButtonStates = engine->GetButtonStates();
	
	// general axis movement
	engine->RegisterEventInterest(this, "AxisMoved");
}

InputProfile::~InputProfile()
{
	engine->UnregisterAllEventInterest(this);
}

s32 InputProfile::GetButtonBinding(s32 button)
{
	if (buttonBindings.count(button))
	{
		return buttonBindings[button];
	}
	return -1;
}

s32 InputProfile::GetAxisBinding(s32 axis)
{
	if (axisBindings.count(axis))
	{
		return axisBindings[axis];
	}
	return -1;
}

Set<s32> InputProfile::GetReverseButtonBinding(s32 id)
{
	if (reverseButtonBindings.count(id))
	{
		return reverseButtonBindings[id];
	}
	return Set<s32>();
}

Set<s32> InputProfile::GetReverseAxisBinding(s32 id)
{
	if (reverseAxisBindings.count(id))
	{
		return reverseAxisBindings[id];
	}
	return Set<s32>();
}

void InputProfile::BindButton(s32 id, s32 button)
{
	buttonBindings[button] = id;
	
	// then add to reverse button binding list for this id.
	// (if not already present)
	
	Set<s32> &buttons = reverseButtonBindings[id];
	buttons.Insert(button);
	
	// probably not needed, but clear any previously set forced state
	if (forcedVirtualButtons.count(id))
		forcedVirtualButtons.erase(forcedVirtualButtons.find(id));
}

void InputProfile::BindAxis(s32 id, s32 axis, bool inverted)
{
	axisBindings[axis] = id;
	
	if (inverted)
		axisInverted[axis] = inverted;
	
	// then add to reverse axes binding list for this id.
	// (if not already present)
	
	Set<s32> &axes = reverseAxisBindings[id];
	axes.Insert(axis);
}

void InputProfile::BindButtonAsAxis(s32 id, s32 button, f32 delta)
{
	axisAsButtonBindings[button] = id;
	
	// Store the delta value for the real button.
	axisAsButtonDeltas[button] = delta;
	
	// then add to reverse axes binding list for this id.
	// (if not already present)
	
	Set<s32> &axesAsButtons = reverseAxisAsButtonBindings[id];
	axesAsButtons.Insert(button);
}

bool InputProfile::IsButtonDown(s32 id)
{
	// If any virtual buttons are forced, then return that overridden state.
	if (forcedVirtualButtons.count(id))
		return forcedVirtualButtons[id];
	
	// See if any of the bound buttons are down...
	
	Set<s32> realButtons = GetReverseButtonBinding(id);
	
	for (u32 i = 0; i < realButtons.size(); i ++)
	{
		if (engine->GetButtonState(realButtons[i]))
			return true;
	}
	
	// False if button does not exist...
	return false;
}

void InputProfile::ForceButtonState(s32 id, bool state)
{
	// Only set if not already at the desired state AND not already set.
	if (IsButtonDown(id) != state && !forcedVirtualButtons.count(id))
	{
		forcedVirtualButtons[id] = state;
		
		// Send the event for this change in state.
		if (state)
		{
			Event event("ProfileButtonDown");
			event["button"] = id;
			profileEventCache.push_back(event);
		}
		else
		{
			Event event("ProfileButtonUp");
			event["button"] = id;
			profileEventCache.push_back(event);
		}
	}
}

f32 InputProfile::GetAxisInversionAsFloat(s32 axis)
{
	if (axisInverted.count(axis))
		return axisInverted[axis] ? -1.f : 1.f;
	else
		return 1.f;
}

void InputProfile::NotifyAxisChanged(s32 id, f32 delta, const Set<IWantInput *> &subscribers)
{
	if (id != -1)
	{
		for (u32 i = 0; i < subscribers.size(); i ++)
			subscribers[i]->OnAxisChange(id, delta);
	}
}

void InputProfile::NotifyButtonDown(s32 id, const Set<IWantInput *> &subscribers)
{
	if (id != -1)
	{
		for (u32 i = 0; i < subscribers.size(); i ++)
			subscribers[i]->OnButtonDown(id);
	}
}

void InputProfile::NotifyButtonUp(s32 id, const Set<IWantInput *> &subscribers)
{
	if (id != -1)
	{
		for (u32 i = 0; i < subscribers.size(); i ++)
			subscribers[i]->OnButtonUp(id);
	}
}

void InputProfile::SetButtonEnabled(s32 id, bool enabled)
{
	if (!enabled)
		ForceButtonState(id, false);
	
	buttonEnabled[id] = enabled;
}

void InputProfile::SetAxisEnabled(s32 id, bool enabled)
{
	axisEnabled[id] = enabled;
}

bool InputProfile::IsButtonEnabled(s32 id)
{
	if (buttonEnabled.count(id))
		return buttonEnabled[id];
	else
		return true;
}

bool InputProfile::IsAxisEnabled(s32 id)
{
	if (axisEnabled.count(id))
		return axisEnabled[id];
	else
		return true;
}

void InputProfile::OnPause()
{
}

void InputProfile::OnResume()
{
}

void InputProfile::OnEvent(const Event &event)
{
	if (IsPaused())
		return;
	
	// We will only be receiving AxisMoved events.
	// Generate the ProfileAxisMoved event containing the virtual axis id.
	
	s32 virtualAxis = GetAxisBinding(event["axis"]);
	
	if (virtualAxis != -1 && IsAxisEnabled(virtualAxis))
	{
		Event newEvent("ProfileAxisMoved");
		newEvent["axis"] = virtualAxis;
		newEvent["delta"] = event["delta"].To<f32>() * GetAxisInversionAsFloat(event["axis"]);
		profileEventCache.push_back(newEvent);
	}
}
	
void InputProfile::ProcessAndNotify(const Set<IWantInput *> &subscribers)
{
	if (IsPaused())
		return;
	
	// We don't use button input events, instead generate our own using button states.
	// otherwise would have to consider missed events when paused.
	
	ButtonStates newRealButtonStates = engine->GetButtonStates();
	
	// Now find deltas, but only for those buttons we are interested in...
	// For all reverse button bindings...
	for (std::map<s32, Set<s32> >::const_iterator i = reverseButtonBindings.begin();
			i != reverseButtonBindings.end(); i ++)
	{
		s32 id = i->first;
		const Set<s32> &realButtons = i->second;
		
		// Not sure if this is the right place for this.
		// (not tested...)
		if (!IsButtonEnabled(id))
			continue;
		
		// Find if any physical buttons bound to this virtual button is currently down,
		// and also if any were previously down.
		
		bool anyDownNow = false;
		bool anyDownPreviously = false;
		
		for (u32 j = 0; j < realButtons.size(); j ++)
		{
			if (newRealButtonStates[realButtons[j]])
				anyDownNow = true;
			
			if (lastRealButtonStates[realButtons[j]])
				anyDownPreviously = true;
		}
		
		// Handle virtual button forced states.
		if (forcedVirtualButtons.count(id))
		{
			// See if the button forcing is over.
			// Does real state match the forced state yet? If so, remove.
			if (forcedVirtualButtons[id] == anyDownNow)
			{
				forcedVirtualButtons.erase(forcedVirtualButtons.find(id));
				
				// We still continue below, as the button delta event was generated
				// earlier by the call to ForceButtonState.
			}
			
			// and then do nothing...
			continue;
		}
		
		// *any* can be down now, where all were not down previously to generate down event.
		
		if (anyDownNow && !anyDownPreviously)
		{
			Event event("ProfileButtonDown");
			event["button"] = id;
			profileEventCache.push_back(event);
		}
		
		// *all* must be up, where at least one was down previously, to generate up event.
		
		if (!anyDownNow && anyDownPreviously)
		{
			Event event("ProfileButtonUp");
			event["button"] = id;
			profileEventCache.push_back(event);
		}
	}
	
	// Now do a similar thing, but for all reverse axisAsButton bindings.
	// Here we may generate some ProfileAxisMoved events as a result of button presses.
	for (std::map<s32, Set<s32> >::const_iterator i = reverseAxisAsButtonBindings.begin();
			i != reverseAxisAsButtonBindings.end(); i ++)
	{
		s32 id = i->first;
		const Set<s32> &realButtons = i->second;
		
		// axis enabled?
		if (!IsAxisEnabled(id))
			continue;
		
		// Final result for the axis delta is the sum of all deltas from new real button events.
		f32 axisDeltaTotal = 0.f;
		
		// Find if any physical buttons bound to this virtual *axis* have been pressed down.
		for (u32 j = 0; j < realButtons.size(); j ++)
		{
			// Was there a button down event for this real button?
			// If so, we add the delta value for that real button to the total.
			if (newRealButtonStates[realButtons[j]] && !lastRealButtonStates[realButtons[j]])
				axisDeltaTotal += axisAsButtonDeltas[realButtons[j]];
		}
		
		// If the resulting axis delta is nonzero, the axis has moved!
		if (axisDeltaTotal != 0.f)
		{
			Event event("ProfileAxisMoved");
			event["axis"] = id;
			event["delta"] = axisDeltaTotal;
			profileEventCache.push_back(event);
		}
	}
	
	lastRealButtonStates = newRealButtonStates;
	
	// Handle the newly generated events.
	// We do them here rather than inline above because the events could well delete this input profile
	// and we don't want to be accessing member attributes at the time.
	// For that reason we also need to operate on a copy of the vector.
	
	// Better solution may be to use the new QueueEvent rather than PostEvent below...
	
	std::vector<Event> profileEventCacheCOPY = profileEventCache;
	profileEventCache.clear();
	
	// Send the newly generated events everywhere else
	// CHANGE: we used to do this after the below NotifyButtonDown etc
	// but now we do it before, since the button downs may delete this InputProfile.
	// We also queue the event instead of instantly posting it for a similar reason.
	for (u32 i = 0; i < profileEventCacheCOPY.size(); i ++)
		engine->QueueEvent(profileEventCacheCOPY[i]);
	
	// Not gonna die as a result of below notifications!
	grab();
	
	for (u32 i = 0; i < profileEventCacheCOPY.size(); i ++)
	{
		const Event &event = profileEventCacheCOPY[i];
		
		if (event.IsType("ProfileButtonDown"))
			NotifyButtonDown(event["button"], subscribers);
		else if (event.IsType("ProfileButtonUp"))
			NotifyButtonUp(event["button"], subscribers);
		else if (event.IsType("ProfileAxisMoved"))
			NotifyAxisChanged(event["axis"], event["delta"], subscribers);
	}
	
	// Ok, can now die if you really want.
	drop();
}

