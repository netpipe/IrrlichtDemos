
class GUIElementIDs
{
public:
	// These identifiers represent GUI elements defined in the gui.xml file
	enum
	{
		// Params dialog
		EVOLUTION_PARAMS_WINDOW = 100,
		EVOLUTION_PARAMS_BUTTON_START = 101,
		EVOLUTION_PARAMS_BUTTON_EXIT = 112,
		
		EVOLUTION_PARAMS_EDITBOX_GEN_LEN = 102,
		EVOLUTION_PARAMS_EDITBOX_ITEM_COUNT = 103,
		
		EVOLUTION_PARAMS_EDITBOX_POP_SIZE = 104,
		EVOLUTION_PARAMS_EDITBOX_ELITE_COUNT = 105,
		EVOLUTION_PARAMS_COMBOBOX_SELECTION_METHOD = 106,
		EVOLUTION_PARAMS_EDITBOX_CROSSOVER_PROB = 107,
		EVOLUTION_PARAMS_EDITBOX_MUTATION_PROB = 108,
		EVOLUTION_PARAMS_EDITBOX_MAX_MUTATION = 109,
		
		EVOLUTION_PARAMS_EDITBOX_NEURON_COUNT = 110,
		EVOLUTION_PARAMS_EDITBOX_INITIAL_RAND = 111,
		
		// Camera view control dialog
		CAMERA_CONTROL_WINDOW = 200,
		CAMERA_CONTROL_BUTTON_BIRDSEYE = 201,
		CAMERA_CONTROL_BUTTON_FOLLOW = 202,
		
		// Fast forward dialog
		FAST_FORWARD_WINDOW = 300,
		FAST_FORWARD_BUTTON_RETURN = 301,
		
		// Evolution control dialog
		EVOLUTION_CONTROL_WINDOW = 400,
		EVOLUTION_CONTROL_BUTTON_FAST = 401,
		EVOLUTION_CONTROL_BUTTON_STOP = 402
	};
	
	// Recurse through GUI hierarchy to find the IGUIElement pointer of the specified id.
	static gui::IGUIElement *getElementFromID(s32 id, gui::IGUIElement *startElement = NULL)
	{
		if (startElement == NULL)
			startElement = engine.getGUIEnvironment()->getRootGUIElement();
		
		if (startElement->getID() == id)
			return startElement;
		
		core::list<gui::IGUIElement *>::ConstIterator begin = startElement->getChildren().begin();
		core::list<gui::IGUIElement *>::ConstIterator end = startElement->getChildren().end();
		
		for (; begin != end; begin ++)
		{
			if (gui::IGUIElement *found = getElementFromID(id, *begin))
				return found;
		}
		
		return NULL;
	}
};
