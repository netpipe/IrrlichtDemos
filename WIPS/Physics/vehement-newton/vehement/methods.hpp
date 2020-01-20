// Vehement (C) Steven Dorman 2008
#define	 VEHEMENT_METHODS_HPP

// using the vehe namespace
namespace vehe
{
	//! Create a singleton instance of the factory
	dfactory *dfactory::dinstance;

	#include "receiver.hpp"

	//! Set up the  factory and returns its pointer
	inline dfactory *CreateDevice(int width, int height, bool fullscr) 
	{
		dfactory *facto = dfactory::instance();
		MyEventReceiver * rcv	= new MyEventReceiver();	
		facto->init(width,height,fullscr, rcv);
		return  facto;
	}

	//! Get the pointer to the factory
	inline dfactory *GetDevice() {	return 	dfactory::instance(); }

};