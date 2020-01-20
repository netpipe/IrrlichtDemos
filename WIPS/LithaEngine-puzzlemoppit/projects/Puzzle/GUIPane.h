
#ifndef GUIPANE_H
#define GUIPANE_H

class GUIPane : public gui::IGUIElement
{
	video::SColor col;
	
public:
	GUIPane(gui::IGUIElement *parent, core::recti rect, video::SColor col)
		: gui::IGUIElement(gui::EGUIET_ELEMENT, GetEngine()->GetIrrlichtDevice()->getGUIEnvironment(),
			parent, -1, rect), col(col)
	{
	}
	
	void setCol(const video::SColor &col)
	{
		this->col = col;
	}
	
	video::SColor getCol()
	{
		return col;
	}
	
	void draw()
	{
		if (IsVisible)
		{
			gui::IGUISkin *skin = Environment->getSkin();
			core::rect<s32> rect = AbsoluteRect;
			skin->draw2DRectangle(this, col, AbsoluteRect, &AbsoluteClippingRect);
		}
		
		gui::IGUIElement::draw();
	}
};

// Fades out parent
class GUIElementFade : public virtual gui::IGUIElement
{
	IUpdatable *updatable;
	f32 startTime;
	f32 finishTime;
	f32 fadeTime;
	f32 startFadeTime;
	video::SColor startCol;
	
	bool fadeOut;// otherwise fade in
	
	bool disabled;
	
public:
	// the element will last for lifetime
	// will become completely invisible at lifeTime
	// will start fading fadeTime before lifeTime
	GUIElementFade(gui::IGUIEnvironment *env, gui::IGUIElement *parent, IUpdatable *updatable,
			f32 lifeTime, f32 fadeTime, bool fadeOut)
			: IGUIElement(gui::EGUIET_ELEMENT, env, parent, -1, core::recti(0,0,1,1)) , fadeOut(fadeOut)
	{
		this->updatable = updatable;
		
		disabled = false;
		
		startTime = updatable->GetVirtualTime();
		finishTime = startTime + lifeTime;
		this->fadeTime = fadeTime;
		startFadeTime = finishTime - fadeTime;
		
		if (startFadeTime < startTime)
			startFadeTime = startTime;
		
		if (Parent) // Should always be a parent given!
		{
			if (Parent->getType() == gui::EGUIET_STATIC_TEXT)
				startCol = ((gui::IGUIStaticText *)Parent)->getOverrideColor();
			else if (dynamic_cast<GUIPane *>(Parent))
				startCol = (dynamic_cast<GUIPane *>(Parent))->getCol();
		}
	}
	
	virtual ~GUIElementFade(){}
	
	void OnPostRender(u32 timeMs)
	{
		if (disabled)
			return;
		
		gui::IGUIElement::OnPostRender(timeMs);
		
		f32 t = updatable->GetVirtualTime();
		
		if (Parent && t >= startFadeTime)
		{
			// interpolation amount
			
			f32 alpha = core::clamp((t - startFadeTime) / fadeTime, 0.f, 1.f);
			
			if (fadeOut)
				alpha = 1.f - alpha;
			
			// get current colour
			
			video::SColor current = startCol;
			
			// interpolate
			current = current.getInterpolated(video::SColor(0,
					current.getRed(),current.getBlue(),current.getGreen()), alpha);
			
			// re-set colour
			if (Parent->getType() == gui::EGUIET_STATIC_TEXT)
				((gui::IGUIStaticText *)Parent)->setOverrideColor(current);
			else if (dynamic_cast<GUIPane *>(Parent))
				(dynamic_cast<GUIPane *>(Parent))->setCol(current);
		}
		
		// Remove this element when fade has finished.
		if (Parent && t > finishTime)
		{
			// we use a disabled flag unstead of remove() since that
			// seemed to crash Irrlicht.
			//remove();
			
			disabled = true;
		}
	}
};


#endif

