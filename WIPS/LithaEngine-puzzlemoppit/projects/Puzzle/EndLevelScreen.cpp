
#include "EndLevelScreen.h"
#include "MainState.h"
#include "Level.h"
#include "GUIPane.h"

const f32 timeBeforeShowing = 2.f;

EndLevelScreen::EndLevelScreen(MainState *mainState, Level *level)
	: mainState(mainState), level(level)
{
	engine = GetEngine();
	renderSystem = engine->GetRenderSystem();
	
	stats = level->GetLevelStats();
	scoreResult = input_score(level->GetShortName(), stats);
	
	engine->RegisterEventInterest(this, "ButtonDown");
	engine->RegisterEventInterest(this, "EndLevelText");
	engine->RegisterEventInterest(this, "ScreenFadeFinished");
	engine->RegisterEventInterest(this, "EndLevelScreenShow");
	engine->RegisterEventInterest(this, "EndLevelScreenClose");
	engine->RegisterEventInterest(this, "EndLevelScreenListItem");
	engine->RegisterEventInterest(this, "EndLevelScreenListItemFinalScore");
	
	sound = engine->GetSoundSystem()->CreateSound2D();
	sound->SetVolume(0.5);
	
	eventQueue = engine->CreateEventQueue();
	GetUpdater().AddUpdatable(eventQueue);
	eventQueue->drop();
	
	// Wait a brief period before showing.
	Event event("EndLevelScreenShow");
	eventQueue->AddTimeWait(timeBeforeShowing);
	eventQueue->AddEvent(event);
}

EndLevelScreen::~EndLevelScreen()
{
	if (sound)
		sound->drop();
	
	engine->UnregisterAllEventInterest(this);
	
	for (u32 i = 0; i < guiElements.size(); i ++)
		guiElements[i]->remove();
}

void offset_y(gui::IGUIElement *element, s32 y)
{
	core::recti rect = element->getRelativePosition();
	rect.UpperLeftCorner.Y += y;
	rect.LowerRightCorner.Y += y;
	element->setRelativePosition(rect);
}

void EndLevelScreen::OnEvent(const Event &event)
{
	video::IVideoDriver *driver = engine->GetIrrlichtDevice()->getVideoDriver();
	s32 screenWidth = driver->getScreenSize().Width;
	s32 screenHeight = driver->getScreenSize().Height;
	
	const f32 betweenItemWait = 1.f;
	const f32 itemFadeOnTime = 0.5f;
	
	//f32 bgMinY = 0.3;//0.15;
	
	//bgMinY = (screenHeight - bgHeight) / 2 ;
	
	if (event.IsType("EndLevelScreenShow"))
	{
		gui::IGUIElement *bg;
		
		{
			bg = new GUIPane(engine->GetIrrlichtDevice()->getGUIEnvironment()->getRootGUIElement(),
					core::recti(-10,0,screenWidth+10,screenHeight),
					video::SColor(200, 0,0,0));
			bg->drop();
			guiElements.push_back(bg);
		}
		
		{
			core::stringw text = L"Level Complete!";
			
			/*
			std::vector<core::stringc> levels = find_levels();
			core::stringc levelShortName = level->GetShortName();
			
			for (u32 i = 0; i < levels.size(); i ++)
			{
				if (levels[i] == levelShortName)
				{
					text = L"Level ";
					text += i;
					text += L" of ";
					text += 30;
					text += L" Complete!";
					break;
				}
			}
			*/
			
			s32 yPos = s32(screenHeight * 0.05);
			gui::IGUIElement *element = add_static_text2( text.c_str() );
			core::recti rect = element->getRelativePosition();
			rect += core::vector2di( screenWidth/2-rect.getWidth()/2, yPos );
			element->setRelativePosition(rect);
			guiElements.push_back( element );
		}
		
		// apply fades to all stuff created so far
		// (fade on)
		
		for (u32 i = 0; i < guiElements.size(); i ++)
		{
			GUIElementFade *fade = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					guiElements[i], this, itemFadeOnTime, itemFadeOnTime, false);
			fade->drop();
			fade->OnPostRender(0);
		}
		
		// Now queue the rest of the stuff.
		
		f32 yPos = 0.15;
		const f32 yInc = 0.05f;
		
		{
			eventQueue->AddTimeWait(betweenItemWait);
			Event event("EndLevelScreenListItem");
			event["text"] << "Blocks pushed: " << stats.pushes;
			event["y_pos"] = yPos;
			eventQueue->AddEvent(event);
		}
		
		// presumably if there are elevators present, we will use them at least once.
		// so this if should fail only if there are no elevators.
		if (stats.elevatorMoves > 0)
		{
			yPos += yInc;
			
			eventQueue->AddTimeWait(betweenItemWait);
			Event event("EndLevelScreenListItem");
			event["text"] << "Elevator journeys: " << stats.elevatorMoves;
			event["y_pos"] = yPos;
			eventQueue->AddEvent(event);
		}
		
		// ugly hack.
		// we don't want to show undo stats on first level.
		// (since tutorial doesn't introduce undo until second level)
		if (level->GetShortName() != "first.lev")
		{
			yPos += yInc;
			
			eventQueue->AddTimeWait(betweenItemWait);
			Event event("EndLevelScreenListItem");
			event["text"] << "Undos used: " << stats.undos;
			event["y_pos"] = yPos;
			eventQueue->AddEvent(event);
		}
		
		yPos += yInc;
		
		{
			eventQueue->AddTimeWait(betweenItemWait);
			Event event("EndLevelScreenListItem");
			event["text"] << "Deaths: " << stats.deaths;
			event["y_pos"] = yPos;
			eventQueue->AddEvent(event);
		}
		
		yPos += 0.1f;
		
		{
			eventQueue->AddTimeWait(betweenItemWait);
			Event event("EndLevelScreenListItemFinalScore");
			event["text"] << "Your rating: ";
			event["text2"] << get_result_description(scoreResult);
			event["y_pos"] = yPos;
			eventQueue->AddEvent(event);
		}
		
		f32 bgHeight = yPos + 0.05f;
		
		// set bg correct height
		core::recti rect = bg->getRelativePosition();
		rect.LowerRightCorner.Y = s32(screenHeight * bgHeight);
		bg->setRelativePosition(rect);
		
		// now find downwards Y offset required to centre everything vertically.
		s32 yOffset = (screenHeight - s32(screenHeight*bgHeight)) / 2;
		f32 yOffsetFloat = (1.f - bgHeight) / 2.f;
		
		
		// apply to all gui elements created so far
		for (u32 i = 0; i < guiElements.size(); i ++)
			offset_y(guiElements[i], yOffset);
		
		// and all those waiting in events...
		// (hack alert!)
		
		std::vector<Event *> allEvents = eventQueue->GetAllEvents();
		
		for (u32 i = 0; i < allEvents.size(); i ++)
		{
			Event &event = *(allEvents[i]);
			
			if (event.HasKey("y_pos"))
				event["y_pos"] = event["y_pos"] + yOffsetFloat;
		}
		
		// Stop player teleport effects.
		// Not sure whether to do this or not.
		//level->ClearEndLevelTeleportEffects();
	}
	else if (event.IsType("EndLevelScreenListItem"))
	{
		{
			gui::IGUIElement *element = add_static_text( core::stringw(event["text"].To<core::stringc>()).c_str() );
			core::recti rect = element->getRelativePosition();
			rect += core::vector2di( screenWidth/2 - rect.getWidth()+75,
					s32(screenHeight*event["y_pos"].To<f32>())-rect.getHeight()/2 );
			element->setRelativePosition(rect);
			guiElements.push_back( element );
			
			GUIElementFade *fade = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					element, this, itemFadeOnTime, itemFadeOnTime, false);
			fade->drop();
			fade->OnPostRender(0);
			
			sound->Play("../projects/Puzzle/media/sfx/appear.ogg");
		}
	}
	else if (event.IsType("EndLevelScreenListItemFinalScore"))
	{
		{
			gui::IGUIElement *element = add_static_text2( core::stringw(event["text"].To<core::stringc>()).c_str() );
			core::recti rect = element->getRelativePosition();
			rect += core::vector2di( screenWidth/2-rect.getWidth()/2,
					s32(screenHeight*event["y_pos"].To<f32>())-rect.getHeight() );
			element->setRelativePosition(rect);
			guiElements.push_back( element );
			
			GUIElementFade *fade = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					element, this, itemFadeOnTime, itemFadeOnTime, false);
			fade->drop();
			fade->OnPostRender(0);
			
			
			// rating is separate
			
			gui::IGUIStaticText *textElement = add_static_text2( core::stringw(event["text2"].To<core::stringc>()).c_str() );
			core::recti rect2 = textElement->getRelativePosition();
			rect2 += core::vector2di( screenWidth/2-rect2.getWidth()/2,
					s32(screenHeight*event["y_pos"].To<f32>())-rect2.getHeight() );
			textElement->setRelativePosition(rect2);
			guiElements.push_back( textElement );
			
			textElement->setOverrideColor( get_rating_col(scoreResult)
					// and make a bit brighter since we are on a black background
					+ video::SColor(50,0,0,0) );
			
			GUIElementFade *fade2 = new GUIElementFade(engine->GetIrrlichtDevice()->getGUIEnvironment(),
					textElement, this, itemFadeOnTime, itemFadeOnTime, false);
			fade2->drop();
			fade2->OnPostRender(0);
			
			
			// now reposition both
			
			element->setRelativePosition( rect.UpperLeftCorner
					- core::vector2di(rect2.getWidth()/2, 0) );
			
			textElement->setRelativePosition( rect2.UpperLeftCorner
					+ core::vector2di(rect.getWidth()/2 , 0) );
			
			
			switch (scoreResult)
			{
			case ESR_AWFUL:
				sound->Play("../projects/Puzzle/media/sfx/laugh.ogg");
				break;
			case ESR_FAIR:
				sound->SetVolume(0.15);
				sound->Play("../projects/Puzzle/media/sfx/fair.ogg");
				break;
			case ESR_GOOD:
				sound->SetVolume(0.15);
				sound->Play("../projects/Puzzle/media/sfx/good.ogg");
				break;
			case ESR_EXCELLENT:
				sound->SetVolume(0.15);
				sound->Play("../projects/Puzzle/media/sfx/excellent.ogg");
				break;
			case ESR_PERFECT:
				sound->SetVolume(0.25);
				sound->Play("../projects/Puzzle/media/sfx/perfect.ogg");
				break;
			case ESR_EXTRAORDINARY:
				sound->SetVolume(0.25);
				sound->Play("../projects/Puzzle/media/sfx/extraordinary.ogg");
				break;
			}
		}
	}
	else if (event.IsType("ButtonDown") && eventQueue->IsEmpty())// && event["button"] == KEY_LBUTTON)
	{
		// Fade off
		renderSystem->ScreenFade(0.f, 2, true);
		
		// And close this.
		Event event("EndLevelScreenClose");
		TimedEvent(event, 2.f);
	}
	else if (event.IsType("ButtonDown") && !eventQueue->IsEmpty()
			&& eventQueue->IsEventWaiting("EndLevelScreenListItemFinalScore"))
	{
		// Speed up displaying...
		eventQueue->ScaleTimes(0.1);
	}
	else if (event.IsType("EndLevelScreenClose"))
	{
		// Finished.
		// So call next level and remove this screen.
		mainState->NextLevel(true);
		engine->GetWorld()->GetUpdater().RemoveUpdatable(this);
	}
	else if (event.IsType("EndLevelText"))
	{
		
	}
}

