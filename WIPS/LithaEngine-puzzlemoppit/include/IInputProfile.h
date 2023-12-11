
#ifndef I_INPUT_PROFILE_H
#define I_INPUT_PROFILE_H

#include "litha_internal.h"
#include "IPausable.h"

class IWantInput;

enum E_INPUT_AXIS
{
	AXIS_MOUSE_X = 0,
	AXIS_MOUSE_Y,
	AXIS_MOUSE_WHEEL
};

// This maps "virtual buttons" to real buttons. A real button can be a key or a mouse button.
// So a virtual button can have several real buttons mapped to it.
// e.g. a virtual button for moving the player forwards might have both the up key and W key mapped to it.
class IInputProfile : public virtual IReferenceCounted, public IPausable
{
public:
	virtual ~IInputProfile(){}
	
	// Bind a virtual button (id) to a real button.
	virtual void BindButton(s32 id, s32 button) = 0;
	
	// Bind a virtual axis (id) to a real axis (E_INPUT_AXIS).
	virtual void BindAxis(s32 id, s32 axis, bool inverted = false) = 0;
	
	// Bind a virtual axis (id) to a real *button*.
	// delta is the amount (positive or negative) the axis changes in response to a single button press.
	// This allows a button to be used where an axis is normally used.
	// e.g. using buttons to control the camera rotation or zoom. You could bind left arrow
	// with delta  -1.0 and right arrow with +1.0
	virtual void BindButtonAsAxis(s32 id, s32 button, f32 delta) = 0;
	
	// This queries the hardware to get the virtual button state.
	virtual bool IsButtonDown(s32 id) = 0;
	
	// Force a virtual button to change to a particular state.
	// This state will remain until it is changed by user input.
	virtual void ForceButtonState(s32 id, bool state) = 0;
	
	// Set a virtual button to be enabled or disabled.
	// On disabling a button up event will be generated if the button was down.
	// (equivalent to ForceButtonState(id, false))
	// THIS IS NOT TESTED, I'm not sure if it's implemented correctly
	// (particularly, whether events are generated correctly when re-enabling)
	virtual void SetButtonEnabled(s32 id, bool enabled) = 0;
	
	// Set a virtual axis to be enabled or disabled.
	// Disabling means no events will be generated for this axis.
	virtual void SetAxisEnabled(s32 id, bool enabled) = 0;
	
	// Used internally
	// Calculates input state and notifies the given subscribers.
	virtual void ProcessAndNotify(const Set<IWantInput *> &subscribers) = 0;
};

#endif
