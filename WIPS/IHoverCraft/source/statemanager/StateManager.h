#ifndef _T_GRAPH_H_INCLUDED__
#define _T_GRAPH_H_INCLUDED__

#include "State.h"

/*! \class GraphT
 *
 * This template represents a state machine graph. It is used along with 
 * the state template to allow code for independent game states to be
 * written and managed independently.
 */
template <class T, class E>
class StateManager
{
private:

	T* owner; //!< Owner of this state machine instance
	State<T,E>* currentState;  //!< Current state of the state machine
	State<T,E>* previousState; //!< Previous state of the state machine
	State<T,E>* globalState;   //!< Global state

public:

	StateManager(T* owner) :
		owner(owner),
		currentState(0),
		previousState(0),
		globalState(0)
	{
	}

	virtual ~StateManager()
	{
	}

	/*! Set the current state.
	 *
	 * Usually only used for setting the initial state. Could be dangerous
	 * if you are arbitrarily setting states without correctly entering or
	 * exiting the states. Use with caution. Normally you would call the
	 * setState() method to properly transition through states.
	 *
	 * \param state The state to use as the new current state.
	 */
	const void setCurrentState(State<T,E>* state)
	{
		this->currentState = state;
	}

	/*! Retrieve the current state.
	 *
	 * \return Pointer to the current state.
	 */
	State<T,E>* getCurrentState() const
	{
		return this->currentState;
	}

	/*! Set the previous state.
	 *
	 * Usually only used for setting the initial state. Could be dangerous
	 * if you are arbitrarily setting states without correctly entering or
	 * exiting the states. Use with caution. Normally you would call the
	 * setState() method to properly transition through states.
	 *
	 * \param state The state to use as the new previous state.
	 */
	const void setPreviousState(State<T,E>* state)
	{
		this->previousState = state;
	}

	/*! Retrieve the previous state.
	 *
	 * \return Pointer to the previous state.
	 */
	State<T,E>* getPreviousState() const
	{
		return this->previousState;
	}

	/*! Set the global state.
	 *
	 * The global state is updated before the current state
	 * on every update tick.
	 *
	 * \param state The new global state
	 */
	const void setGlobalState(State<T,E>* state)
	{
		this->globalState = state;
		//if (this->globalState)
		//	this->globalState->onEnter(this->owner);
	}

	/*! Retrieve the global state
	 *
	 * \return Pointer to the global state.
	 */
	State<T,E>* getGlobalState()
	{
		return this->globalState;
	}

	/*! Update the state for this machine.
	 *
	 * Updates the global state and then the current state.
	 */
	const void onUpdate() const
	{
		if (this->globalState)
			this->globalState->onUpdate(this->owner);

		if (this->currentState)
			this->currentState->onUpdate(this->owner);
	}

	/*! Process an incoming message.
	 *
	 * Forwards the message to the current state. If the current state
	 * does not process the message, it is forwarded to the global state.
	 *
	 * \param event The message to process.
	 * \return Returns true if the message was processed, otherwise false.
	 */
	const bool onEvent(const E& event) const
	{
		// allow current state to handle message
		if (this->currentState && this->currentState->onEvent(this->owner, event))
			return true;

		// message not processed by current state, try global state
		else if (this->globalState && this->globalState->onEvent(this->owner, event))
			return true;

		return false;
	}

	/*! Transition to next state.
	 *
	 * Exits the current state and enters the provided state. This is the
	 * preferred method for transitioning states. Please use this method
	 * instead of setCurrentState.
	 *
	 * \param state The state to transition to.
	 */
	const void setState(State<T,E>* state)
	{
		if (state == 0) return;

		//Previous = Current;
		setPreviousState(this->currentState);

		if (this->currentState)
			this->currentState->onLeave(this->owner);

		//Current = state;
		setCurrentState(state);

		if (this->currentState)
			this->currentState->onEnter(this->owner);
	}

	/*! Return true if machine is in the given state.
	 *
	 * \param state The state to compare the current state against.
	 * \return Returns true if the machine is in the provided state, else
	 * false.
	 */
	const bool isInState(const State<T,E>* state) const
	{
		return state == currentState;
	}
};

#endif // _T_GRAPH_H_INCLUDED__
