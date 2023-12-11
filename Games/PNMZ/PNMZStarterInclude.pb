; PureBasic Visual Designer v3.92 build 1460


;- Window Constants
;
Enumeration
  #Window_0
EndEnumeration

;- Gadget Constants
;
Enumeration
  #Combo_0
  #Text_0
  #CheckBox_0
  #Button_0
EndEnumeration


Procedure Open_Window_0()
  If OpenWindow(#Window_0, 216, 0, 291, 48,  #PB_Window_SystemMenu | #PB_Window_SizeGadget | #PB_Window_TitleBar | #PB_Window_ScreenCentered , "Please No More Zombies - Launcher V1.0")
    If CreateGadgetList(WindowID())
      ComboBoxGadget(#Combo_0, 57, 0, 162, 150,#PB_ComboBox_Editable)
      TextGadget(#Text_0, 0, 3, 60, 18, "Resolution:")
      CheckBoxGadget(#CheckBox_0, 222, 3, 72, 15, "FullScreen")
      ButtonGadget(#Button_0, 0, 24, 291, 24, "&Launch Please No More Zombies!")
      AddGadgetItem(#Combo_0,-1,"640 480")
      AddGadgetItem(#Combo_0,-1,"800 600")
      AddGadgetItem(#Combo_0,-1,"1024 768")
      AddGadgetItem(#Combo_0,-1,"1152 864")
      AddGadgetItem(#Combo_0,-1,"1280 960")
      AddGadgetItem(#Combo_0,-1,"1280 1024")
      AddGadgetItem(#Combo_0,-1,"1600 1200")
    EndIf
  EndIf
EndProcedure


; IDE Options = PureBasic v3.94 (Windows - x86)
; DisableDebugger
; HideErrorLog