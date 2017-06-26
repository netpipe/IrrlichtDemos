
class GUI : public IEventReceiver
{
	World *world;
	CameraController *cameraController;
	
	EvolutionParams params;
	
	gui::IGUIWindow *windowEvolutionParams;
	gui::IGUIWindow *windowCameraControl;
	gui::IGUIWindow *windowFastForward;
	gui::IGUIWindow *windowEvolutionControl;
	
	gui::IGUIEditBox *generationLengthEditBox;
	gui::IGUIEditBox *itemCountEditBox;
	gui::IGUIEditBox *populationSizeEditBox;
	gui::IGUIEditBox *eliteCountEditBox;
	gui::IGUIComboBox *selectionMethodComboBox;
	gui::IGUIEditBox *crossoverProbabilityEditBox;
	gui::IGUIEditBox *mutationProbabilityEditBox;
	gui::IGUIEditBox *maximumMutationEditBox;
	gui::IGUIEditBox *neuronCountEditBox;
	gui::IGUIEditBox *initialRandEditBox;
	
	void startEvolution()
	{
		world->startEvolution(params);
	}
	
	void stopEvolution()
	{
		world->stopEvolution();
		// Clear the data written to the screen as it is no longer relevant
		engine.getScreenLog()->clear();
	}
	
	// Called when the parameters in the GUI are modified
	// If the new parameters are valid, they are saved,
	// otherwise the GUI parameters are reverted to what they were before.
	void evolutionParamsChanged()
	{
		// Input validation.
		// If valid values were put into the edit boxes, then update the parameters
		// with these values. Otherwise, revert the edit boxes to their old values.
		// The ComboBox doesn't need validation as it cannot have an invalid setting...
		if (
			strIsNumber(generationLengthEditBox->getText())
			&& strIsNumber(itemCountEditBox->getText())
			&& strIsNumber(populationSizeEditBox->getText())
			&& strIsNumber(eliteCountEditBox->getText())
			&& strIsNumber(crossoverProbabilityEditBox->getText())
			&& strIsNumber(mutationProbabilityEditBox->getText())
			&& strIsNumber(maximumMutationEditBox->getText())
			&& strIsNumber(neuronCountEditBox->getText())
			&& strIsNumber(initialRandEditBox->getText())
			)
		{
			params.genTime			= wcstod( generationLengthEditBox->getText(), NULL );
			params.itemCount		= (int)wcstod( itemCountEditBox->getText(), NULL );
			params.popSize			= (int)wcstod( populationSizeEditBox->getText(), NULL );
			params.eliteCount		= (int)wcstod( eliteCountEditBox->getText(), NULL );
			params.selectorType		= (E_SELECTOR_TYPE)selectionMethodComboBox->getSelected();
			params.probCrossover	= wcstod( crossoverProbabilityEditBox->getText(), NULL );
			params.probMutate		= wcstod( mutationProbabilityEditBox->getText(), NULL );
			params.mutateScale		= wcstod( maximumMutationEditBox->getText(), NULL );
			params.hiddenLayerSize	= (int)wcstod( neuronCountEditBox->getText(), NULL );
			params.initialRandScale	= wcstod( initialRandEditBox->getText(), NULL );
		}
		else
		{
			// set all GUI from memory
			updateGuiContents();
		}
	}
	
	void updateGuiContents()
	{
		// set all GUI from memory (params)
		generationLengthEditBox		->setText( numberToString(params.genTime).c_str() );
		itemCountEditBox			->setText( numberToString(params.itemCount).c_str() );
		populationSizeEditBox		->setText( numberToString(params.popSize).c_str() );
		eliteCountEditBox			->setText( numberToString(params.eliteCount).c_str() );
		selectionMethodComboBox		->setSelected(params.selectorType);
		crossoverProbabilityEditBox	->setText( numberToString(params.probCrossover).c_str() );
		mutationProbabilityEditBox	->setText( numberToString(params.probMutate).c_str() );
		maximumMutationEditBox		->setText( numberToString(params.mutateScale).c_str() );
		neuronCountEditBox			->setText( numberToString(params.hiddenLayerSize).c_str() );
		initialRandEditBox			->setText( numberToString(params.initialRandScale).c_str() );
	}
	
	void centreHorizontally(gui::IGUIElement *element)
	{
		core::rect<s32> rect = element->getRelativePosition();
		int width = rect.getWidth();
		
		rect.UpperLeftCorner.X = engine.getVideoDriver()->getViewPort().getWidth()/2 - width/2;
		rect.LowerRightCorner.X = rect.UpperLeftCorner.X + width;
		
		element->setRelativePosition(rect);
	}
	
	void centreVertically(gui::IGUIElement *element)
	{
		core::rect<s32> rect = element->getRelativePosition();
		int height = rect.getHeight();
		
		rect.UpperLeftCorner.Y = engine.getVideoDriver()->getViewPort().getHeight()/2 - height/2;
		rect.LowerRightCorner.Y = rect.UpperLeftCorner.Y + height;
		
		element->setRelativePosition(rect);
	}
	
public:

	GUI(World *world, CameraController *cameraController)
	{
		this->world = world;
		this->cameraController = cameraController;
		
		// Set the default evolution parameters
		// DEFAULT_EVOLUTION_PARAMS must be declared as a variable before it can be assigned
		EvolutionParams defaultParams = DEFAULT_EVOLUTION_PARAMS;
		params = defaultParams;
		
		engine.getDevice()->setEventReceiver(this);
		engine.getCursorControl()->setVisible(true);
		
		gui::IGUIEnvironment *guienv = engine.getGUIEnvironment();
		
		// Load a nicer font than Irrlicht's default one
		guienv->getSkin()->setFont( guienv->getFont("media/font1/font.xml") );
		
		// Load the GUI.
		// The GUI xml file was created using the GUI creation tool provided with Irrlicht
		guienv->loadGUI("gui.xml");
		
		// Save pointers to some GUI elements for easy access
		
		windowEvolutionParams = (gui::IGUIWindow *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_WINDOW
				);
		windowCameraControl = (gui::IGUIWindow *)GUIElementIDs::getElementFromID(
				GUIElementIDs::CAMERA_CONTROL_WINDOW
				);
		windowFastForward = (gui::IGUIWindow *)GUIElementIDs::getElementFromID(
				GUIElementIDs::FAST_FORWARD_WINDOW
				);
		windowEvolutionControl = (gui::IGUIWindow *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_CONTROL_WINDOW
				);
		
		generationLengthEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_GEN_LEN
				);
		itemCountEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_ITEM_COUNT
				);
		populationSizeEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_POP_SIZE
				);
		eliteCountEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_ELITE_COUNT
				);
		selectionMethodComboBox = (gui::IGUIComboBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_COMBOBOX_SELECTION_METHOD
				);
		crossoverProbabilityEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_CROSSOVER_PROB
				);
		mutationProbabilityEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_MUTATION_PROB
				);
		maximumMutationEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_MAX_MUTATION
				);
		neuronCountEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_NEURON_COUNT
				);
		initialRandEditBox = (gui::IGUIEditBox *)GUIElementIDs::getElementFromID(
				GUIElementIDs::EVOLUTION_PARAMS_EDITBOX_INITIAL_RAND
				);
		
		// Position the Evolution params window in the centre of the screen
		centreHorizontally(windowEvolutionParams);
		centreVertically(windowEvolutionParams);
		
		// Hide the "x" close buttons on the windows
		windowEvolutionParams->getCloseButton()->setVisible(false);
		windowCameraControl->getCloseButton()->setVisible(false);
		windowFastForward->getCloseButton()->setVisible(false);
		windowEvolutionControl->getCloseButton()->setVisible(false);
		
		// Some should not initially be visible
		windowCameraControl->setVisible(false);
		windowFastForward->setVisible(false);
		windowEvolutionControl->setVisible(false);
		
		// Fill the GUI with the default evolution parameters
		updateGuiContents();
	}
	
	core::stringw numberToString(float n)
	{
		core::stringw str(n);
		
		// Remove trailing zeroes (just makes the string look nicer in the GUI,
		// e.g. turns 1.50000 into 1.5)
		for (int i = 0; i < str.size(); i ++)
		{
			if (str[i] == '.')
			{
				for (int j = str.size()-1; j > 0 && str[j] == '0' && j > i+1; j --)
					str.erase(j);
				
				break;
			}
		}
		
		return str;
	}
	
	core::stringw numberToString(int n)
	{
		return core::stringw(n);
	}

	bool OnEvent(const SEvent &event)
	{
		// Shutdown the application if the Escape key is pressed
		if (event.EventType == EET_KEY_INPUT_EVENT && event.KeyInput.Key == KEY_ESCAPE)
		{
			engine.getDevice()->closeDevice();
			return true;
		}
		
		if (event.EventType != EET_GUI_EVENT)
			return false;
		
		const SEvent::SGUIEvent &guiEvent = event.GUIEvent;
		
		s32 callerID = guiEvent.Caller->getID();
		
		switch (guiEvent.EventType)
		{
		case gui::EGET_BUTTON_CLICKED:
			switch (callerID)
			{
			
			// Exit
			case GUIElementIDs::EVOLUTION_PARAMS_BUTTON_EXIT:
				engine.getDevice()->closeDevice();
				break;
			
			// Start the Evolution
			case GUIElementIDs::EVOLUTION_PARAMS_BUTTON_START:
				windowEvolutionParams->setVisible(false);
				windowCameraControl->setVisible(true);
				windowEvolutionControl->setVisible(true);
				startEvolution();
				break;
			// Stop the Evolution
			case GUIElementIDs::EVOLUTION_CONTROL_BUTTON_STOP:
				windowEvolutionParams->setVisible(true);
				windowCameraControl->setVisible(false);
				windowEvolutionControl->setVisible(false);
				stopEvolution();
				break;
			
			
			// Select bird's eye view camera
			case GUIElementIDs::CAMERA_CONTROL_BUTTON_BIRDSEYE:
				cameraController->setCameraType(ECT_BIRDSEYE);
				engine.getScreenLog()->writeLine(L"Top down camera view selected.");
				break;
			// Select follow camera
			case GUIElementIDs::CAMERA_CONTROL_BUTTON_FOLLOW:
				cameraController->setCameraType(ECT_FOLLOW);
				engine.getScreenLog()->writeLine(L"Camera is now following the best performing vehicle.");
				break;
			
			
			// Enter fast forward mode
			case GUIElementIDs::EVOLUTION_CONTROL_BUTTON_FAST:
				windowCameraControl->setVisible(false);
				windowEvolutionControl->setVisible(false);
				windowFastForward->setVisible(true);
				engine.setFastForward(true);
				break;
			// Exit fast forward mode
			case GUIElementIDs::FAST_FORWARD_BUTTON_RETURN:
				windowCameraControl->setVisible(true);
				windowEvolutionControl->setVisible(true);
				windowFastForward->setVisible(false);
				engine.setFastForward(false);
				break;
			
			
			default:
				break;
			}
			break;
		
		case gui::EGET_ELEMENT_FOCUSED:
			if (guiEvent.Caller->getType() == gui::EGUIET_EDIT_BOX)
				guiEvent.Caller->setText(L"");
			break;
		
		case gui::EGET_ELEMENT_FOCUS_LOST:
		case gui::EGET_EDITBOX_ENTER:
		case gui::EGET_COMBO_BOX_CHANGED:
			evolutionParamsChanged();
			break;
			
		default:
			break;
		}
		
		return false;
	}
};
