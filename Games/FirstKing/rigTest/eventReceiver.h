// Multiple event receiver support
// By CuteAlien
class MainEventReceiver : public irr::IEventReceiver 
{ 
public: 
   virtual bool OnEvent(const SEvent& event_) 
   { 
      for ( unsigned int i = 0; i < mEventReceivers.size(); ++i ) 
      { 
         if ( mEventReceivers[i]->OnEvent(event_) ) 
            return true; 
      } 
      return false; 
   } 
    
   void AddEventReceiver(irr::IEventReceiver * receiver_) 
   { 
      mEventReceivers.push_back(receiver_); 
   } 
    
   bool RemoveEventReceiver(irr::IEventReceiver * receiver_) 
   { 
      for ( unsigned int i=0; i<mEventReceivers.size(); ++i ) 
      { 
         if ( mEventReceivers[i] == receiver_ ) 
         { 
            mEventReceivers.erase(i); 
            return true; 
         } 
      } 
      return false; 
   } 
    
private: 
   irr::core::array<irr::IEventReceiver*>   mEventReceivers; 
}; 
