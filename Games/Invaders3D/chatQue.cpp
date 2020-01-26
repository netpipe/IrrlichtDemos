/*
chatQue is copyright g0dsoft 2006
Version: 0.2.3
Code by: Mark Laprairie
Contact: webmaster@g0dsoft.com
Url: www.g0dsoft.com

chatQue.cpp
ChatQue is a object for simulating half-life/starcraft style chat display
capable of colors, special effects and any custom font.

Example usage:
ChatQue* chatQue;
IGUIFont* guiFont;
guiFont = gdevice->getGUIEnvironment()->getFont("Media/Fonts/haettenschweiler.bmp");
chatQue = new ChatQue();
//gdevice is a pointer to the current irrlicht device.
chatQue->initialise(gdevice,core::rect<s32>(10,100,gWIDTH-100,gHEIGHT-100),guiFont,8000,500);
chatQue->setDebug(true);
chatQue->setDefaultStyle(CQS_PLAIN); //This line isnt needed
    chatQue->addMessage( L"This is a test of the chatQue object," );
    chatQue->addMessage( L"for use with irrlicht." );
    chatQue->addMessage( L"Lots of great features including- Word wrrrrrapping! Do, do do, do the word wrap...auto matic happy! Aint life easy?  word wrap" );
    chatQue->addMessage( L"Rainbow Text",CQS_RAINBOW);
    chatQue->addMessage( L"Pulse Text",SColor(255,255,255,0),SColor(0,0,0,0), CQS_PULSE );
    chatQue->addMessage( L"Shadow Text ... Also text can fade off slowly :D",CQS_SHADOW);
    chatQue->addMessage( L"Text can be any color!", SColor(255,100,255,25));
//Just befor you call driver->endScene();
chatQue->draw();

ToDo:
     -Word Wrapping that preserves whole words.
*/

#include "chatQue.h"

ChatQue::ChatQue()
{
    chatQue_init = false;
}

ChatQue::ChatQue(IrrlichtDevice * device,const core::rect<s32> drawArea, IGUIFont* font, u32 textLife,
                         u32 fadeTime, bool setWordWrap)
{
     chatQue_init = false;
     initialise(device,drawArea,font,textLife,fadeTime,setWordWrap);
}

void ChatQue::initialise(IrrlichtDevice * device,const core::rect<s32> drawArea,IGUIFont* font, u32 textLife,
                         u32 fadeTime, bool setWordWrap)
{
     if(!chatQue_init){
         chatQue_device = device;
         if(!font)
            chatQue_font = chatQue_device->getGUIEnvironment()->getBuiltInFont();
         else
            chatQue_font = font;

         chatQue_init = true;
         chatQue_wordWrap = setWordWrap;
         chatQue_life = textLife;
         chatQue_visible = true;
         chatQue_debug = false;
         chatQue_fadeTime = fadeTime;
         chatQue_defaultStyle = CQS_PLAIN;
         m_fontHeight = 0;
         chatQue_calculateRect(drawArea);
         this->setFont(chatQue_font);
     }
}

ChatQue::~ChatQue()
{
   drop();
}

void ChatQue::drop()
{
   if(chatQue_init){
       chatQue_init = false;
       std::list<chatQueMessage>::iterator iter;
       for (iter=chatQue_list.begin(); iter != chatQue_list.end(); iter++)
       {
             if(iter->chatQue_memoryManaged == true)
                delete [] iter->message;
       }
       chatQue_list.erase(chatQue_list.begin(),chatQue_list.end());
   }
}

void ChatQue::addMessage(core::stringw text, SColor color1,SColor color2,chatQueStyle useStyle)
{
   if(chatQue_init){
     chatQueMessage lmsg;
     s32 l_cutPosition = chatQue_font->getCharacterFromPos(text.c_str(),m_xWidth);
     if(l_cutPosition != -1)
        wcscpy(lmsg.message,text.subString(0,l_cutPosition).c_str());
     else
        wcscpy(lmsg.message,text.subString(0,text.size()).c_str());
     lmsg.color1 = color1;
     lmsg.color2 = color2;
     lmsg.fade = 0;
     lmsg.chatQue_memoryManaged = false;
     //Assign Style
     if(useStyle == CQS_USEDEFAULT)
        useStyle = chatQue_defaultStyle;
     lmsg.Style = useStyle;

     lmsg.created = chatQue_device->getTimer()->getTime();
     chatQue_list.push_front(lmsg);

     if(l_cutPosition != -1 && chatQue_wordWrap)
         addMessage(text.subString(l_cutPosition,text.size()),color1,color2,useStyle);
  }
}

void ChatQue::addMessage(wchar_t* text, SColor color1,SColor color2,chatQueStyle useStyle)
{
   addMessage((const wchar_t*) text,color1,color2,useStyle);
}

void ChatQue::addMessage(const wchar_t* text, SColor color1,SColor color2,chatQueStyle useStyle)
{
     //I'm sure there is a better way to do this conversion,
     //Ideas?
     stringw text2;
     text2.append(text);
     addMessage(text2,color1,color2,useStyle);
}

//More addMessage functions, Because clients are lazy!
void ChatQue::addMessage(wchar_t* text, chatQueStyle useStyle){
     addMessage((const wchar_t*)text,SColor(255,255,255,255),SColor(255,0,0,0),useStyle);
}
void ChatQue::addMessage(const wchar_t* text, chatQueStyle useStyle){
     addMessage(text,SColor(255,255,255,255),SColor(255,0,0,0),useStyle);
}
void ChatQue::addMessage(core::stringw text, chatQueStyle useStyle){
     addMessage(text,SColor(255,255,255,255),SColor(255,0,0,0),useStyle);
}

void ChatQue::draw()
{
   if(chatQue_init){
       //Debug
       if(chatQue_debug && chatQue_visible)
         chatQue_device->getVideoDriver()->draw2DRectangle(video::SColor(100,155,155,255),
                                core::rect<s32>(m_x, m_y - m_yHeight,m_x + m_xWidth,m_y));
       //Main Update/Draw
       std::list<chatQueMessage>::iterator iter;
       u16 count = 0;
       u32 m_y_tmp = m_y;
       u32 l_time = chatQue_device->getTimer()->getTime();
       for (iter=chatQue_list.begin(); iter != chatQue_list.end(); iter++)
       {
          //Max lines means no fade, special case delete
          if(count > chatQue_maxLines){
             if(iter->chatQue_memoryManaged == true)
                delete [] iter->message;
             chatQue_list.erase(iter,chatQue_list.end());
             return;
          }
          //Update
          if(chatQue_life >= 0 && (iter->created + chatQue_life) < l_time){
             if(iter->fade == 0 && chatQue_fadeTime > 0)
             {
                  iter->fade = l_time;
                  iter->color1Fade = iter->color1;
                  iter->color2Fade = iter->color2;
                  if(iter->Style == CQS_SHADOW)
                     iter->Style = CQS_INTERNALUSEONLY_FADE2;
                  else
                     iter->Style = CQS_INTERNALUSEONLY_FADE;
             }
             //Kill text that has faded away.
             if( ((l_time - iter->fade) >= chatQue_fadeTime) || chatQue_fadeTime <= 0 )
             {
                 if(iter->chatQue_memoryManaged == true)
                    delete [] iter->message;
                 chatQue_list.erase(iter,chatQue_list.end());
                 return;
             }
            // Old Fade code, left incase you would rather fade to a color
            // Here it fades to black, w/ alpha fade.
             f32 l_alpha = (   (f32)(l_time - iter->fade)/(f32)chatQue_fadeTime );
             iter->color1 = iter->color1Fade.getInterpolated(SColor(0,0,0,0),1-l_alpha);
             iter->color2 = iter->color2Fade.getInterpolated(SColor(0,0,0,0),1-l_alpha);

             // Proposed alpha fade code, does not work with irrlicht 1.1
             //Fade code
             //f32 l_alpha = (   (f32)(l_time - iter->fade)/(f32)chatQue_fadeTime );
             //iter->color1.setAlpha((s32)(iter->color1Fade.getAlpha() * (1-l_alpha)));
             //iter->color2.setAlpha((s32)(iter->color2Fade.getAlpha() * (1-l_alpha)));

          }
          //Draw
          if (chatQue_visible && chatQue_font){
             s16 l_offSetX = 0;
             s16 l_offSetY = 0;
             bool l_render2 = false;
             //Apply style
             switch (iter->Style){
                    case CQS_SHADOW:
                         l_offSetX = 1;
                         l_offSetY = 1;
                         l_render2 = true;
                         break;
                    case CQS_RAINBOW:
                         iter->color1 = SColor(255,(l_time/2)%255,(l_time/3)%255,(l_time)%255);
                         break;
                    case CQS_MULTIRAINBOW:
                         l_offSetX = 1;
                         l_offSetY = 1;
                         iter->color1 = SColor(255,(l_time/2)%255,(l_time/3)%255,(l_time)%255);
                         iter->color2 = SColor(255,(l_time)%255,(l_time/2)%255,(l_time/3)%255);
                         l_render2 = true;
                         break;
                    case CQS_BACKGROUND:
                          chatQue_device->getVideoDriver()->draw2DRectangle(iter->color2,
                          core::rect<s32>(m_x,m_y_tmp - m_fontHeight,m_x+m_xWidth,m_y_tmp));
                         break;
                    case CQS_CROPBACKGROUND:{
                          dimension2du l_fontDimensions = chatQue_font->getDimension(iter->message);
                          chatQue_device->getVideoDriver()->draw2DRectangle(iter->color2,
                          core::rect<s32>(m_x,m_y_tmp - m_fontHeight,m_x+l_fontDimensions.Width,m_y_tmp));
                         break;}
                    case CQS_PULSE:
                         iter->color1Fade = iter->color1;
                         iter->Style = CQS_INTERNALUSEONLY_PULSE;
                    case CQS_INTERNALUSEONLY_PULSE:
                         iter->color1 = iter->color1Fade.getInterpolated(iter->color2,(l_time%1000)/999.9);
                         break;
                    case CQS_INTERNALUSEONLY_FADE2:
                         l_offSetX = 1;
                         l_offSetY = 1;
                         l_render2 = true;
                         break;
                    default:{break;}
             }
             if(l_render2)
                   chatQue_font->draw(iter->message,
                         core::rect<s32>(m_x + l_offSetX,m_y_tmp - m_fontHeight+ l_offSetY,m_x+m_xWidth + l_offSetX,m_y_tmp + l_offSetY),
                         iter->color2);

             chatQue_font->draw(iter->message,
                   core::rect<s32>(m_x,m_y_tmp - m_fontHeight,m_x+m_xWidth,m_y_tmp),
                   iter->color1);

             m_y_tmp -= m_fontHeight;
          }
          count++;
       }
   }
}

//!-- Start Helpers

void ChatQue::chatQue_calculateRect(const core::rect<s32> drawArea)
{
   m_xWidth  = drawArea.getWidth();
   m_yHeight = drawArea.getHeight();
   position2d<s32> l_positionTmp = drawArea.LowerRightCorner;
   m_y = l_positionTmp.Y;
   l_positionTmp = drawArea.UpperLeftCorner;
   m_x = l_positionTmp.X;
}

void ChatQue::chatQue_calculateFontDimensions(irr::gui::IGUIFont* font)
{
     dimension2du l_fontDimensions = font->getDimension(L"W");
     m_fontHeight = l_fontDimensions.Height;
     chatQue_calculateMaxLines();
}

void ChatQue::chatQue_calculateMaxLines()
{
     chatQue_maxLines = ((m_yHeight - m_fontHeight) / m_fontHeight);
}

//!-- End Helpers

//Set Functions

void ChatQue::setFadeTime(u32 fadeTime)
{
   if(chatQue_init)
     chatQue_fadeTime = fadeTime;
}

void ChatQue::setMaxLines(u16 maxLines)
{
   if(chatQue_init){
     chatQue_calculateMaxLines();
     if(maxLines < chatQue_maxLines)
        chatQue_maxLines = maxLines;
   }
}

void ChatQue::setFont(irr::gui::IGUIFont* font)
{
   if(chatQue_init){
     chatQue_font = font;
     chatQue_calculateFontDimensions(font);
   }
}

void ChatQue::setLife(u32 setLife)
{
   if(chatQue_init)
     chatQue_life = setLife;
}

void ChatQue::setVisible(bool setVisible)
{
   if(chatQue_init)
     chatQue_visible = setVisible;
}

void ChatQue::setWordWrap(bool setWordWrap)
{
   if(chatQue_init)
     chatQue_wordWrap = setWordWrap;
}

void ChatQue::setDrawArea(const core::rect<s32> drawArea)
{
   if(chatQue_init){
     chatQue_calculateRect(drawArea);
     setFont(chatQue_font);
   }
}

void ChatQue::setDebug(bool setDebug)
{
   if(chatQue_init)
     chatQue_debug = setDebug;
}

void ChatQue::setDefaultStyle(chatQueStyle useStyle)
{
   if(chatQue_init){
     if(useStyle == CQS_USEDEFAULT)
        useStyle = CQS_PLAIN;
     chatQue_defaultStyle = useStyle;
   }
}

//Get Functions
chatQueStyle ChatQue::getDefaultStyle()
{
  if(chatQue_init)
     return chatQue_defaultStyle;
  else
     return CQS_USEDEFAULT;
}
bool ChatQue::getVisible()
{
  if(chatQue_init)
     return chatQue_visible;
  else
     return false;
}
u32 ChatQue::getFadeTime()
{
  if(chatQue_init)
     return chatQue_fadeTime;
  else
     return 0;
}
bool ChatQue::getDebug()
{
  if(chatQue_init)
     return chatQue_debug;
  else
     return false;
}
irr::gui::IGUIFont* ChatQue::getFont()
{
  if(chatQue_init && chatQue_font)
     return chatQue_font;
  else
     return NULL;
}
