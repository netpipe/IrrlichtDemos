; PureBasic Visual Designer v3.92 build 1460

IncludeFile "PNMZStarterInclude.pb"

Procedure SaveSettings()
  If CreatePreferences("./PNMZ.ini")
    WritePreferenceString("Resolution", GetGadgetText(#Combo_0))
    WritePreferenceString("Fullscreen", Str(GetGadgetState(#CheckBox_0)))
    ClosePreferences()
  EndIf 
EndProcedure

Procedure LoadSettings()
  If OpenPreferences("./PNMZ.ini")
    str.s=Trim(ReadPreferenceString("Resolution","640 480"))
    If str<>""
      SetGadgetText(#Combo_0,str)
    EndIf 
    
    strb.b=Val(ReadPreferenceString("Fullscreen","0"))
    SetGadgetState(#CheckBox_0,strb)
  EndIf
EndProcedure


Open_Window_0()
LoadSettings()

Repeat ; Start of the event loop
  
  Event = WaitWindowEvent() ; This line waits until an event is received from Windows
  
  WindowID = EventWindowID() ; The Window where the event is generated, can be used in the gadget procedures
  
  GadgetID = EventGadgetID() ; Is it a gadget event?
  
  EventType = EventType() ; The event type
  
  ;You can place code here, and use the result as parameters for the procedures
  
  If Event = #PB_EventGadget
   
    If GadgetID = #Button_0
      SaveSettings()
      checkVal.s=" "+Trim(Str(GetGadgetState(#CheckBox_0)))
      If RunProgram("./PNMZ.exe",GetGadgetText(#Combo_0)+checkVal,"")=0
        MessageRequester("Error","Couldn't launch program.")
      EndIf 
      End
    EndIf
    
  EndIf
  
Until Event = #PB_Event_CloseWindow ; End of the event loop

End
;

; IDE Options = PureBasic v3.94 (Windows - x86)
; Executable = PNMZStarter.exe
; DisableDebugger
; HideErrorLog