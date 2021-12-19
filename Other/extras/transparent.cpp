#include <GUIConstants.au3>
#include <Constants.au3>
#include <WindowsConstants.au3>
#include <WINAPI.au3>
#include "IrrlichtPluginUtils.au3"


HotKeySet("{ESC}", "Terminate")

#Region - GUI Create
$gui = GUICreate("trans", 300, 400, -1, -1, -1, $WS_EX_LAYERED)
GUICtrlCreateLabel("This is text on a transparent Layered GUI", 10, 10, 200, 20, -1, $GUI_WS_EX_PARENTDRAG)
GUICtrlSetTip(-1, "Click label to drag layered window")
$layButt = GUICtrlCreateButton("Button", 10, 40, 40)
GUISetBkColor(0xABCDEF)
_WinAPI_SetLayeredWindowAttributes($gui, 0x010101)
CreateDeviceOnWindow( $gui, $EDT_BURNINGSVIDEO, 10, 10, 320, 240, 32, 0, 0 )
GUISetState()

$Camera = AddCameraSceneNode( 0, 0, 25, -40, 0, 0, 0 )
$Node = AddCubeSceneNode( 20 )
$Au3Texture = GetTexture( "data\au3.bmp" )
SetMaterialTexture( $Node, 0, $Au3Texture )
SetMaterialFlag( $Node, $EMF_LIGHTING, 0 )
$Y = 0
$Direction = 2
#Region - GUI SelectLoop
GUISetStyle($WS_POPUP, -1, $gui)
_WinAPI_SetLayeredWindowAttributes($gui, 0xABCDEF, 255)

While IrrRun( )
SetRotation( $Node, 0, $Y, 0 )

BeginScene( true, true, 0, 171, 205, 239 )

SceneDraw( )
GuiDraw( )

EndScene( )

sleep( 20 )

$Y += $Direction
If $Y > 360 Then $Y -= 360
If $Y < 0 Then $Y += 360


WEnd

Func _WinAPI_SetLayeredWindowAttributes($hwnd, $i_transcolor, $Transparency = 255, $dwFlages = 0x03, $isColorRef = False)
If $dwFlages = Default Or $dwFlages = "" Or $dwFlages < 0 Then $dwFlages = 0x03

If Not $isColorRef Then
$i_transcolor = Hex(String($i_transcolor), 6)
$i_transcolor = Execute('0x00' & StringMid($i_transcolor, 5, 2) & StringMid($i_transcolor, 3, 2) & StringMid($i_transcolor, 1, 2))
EndIf
Local $Ret = DllCall("user32.dll", "int", "SetLayeredWindowAttributes", "hwnd", $hwnd, "long", $i_transcolor, "byte", $Transparency, "long", $dwFlages)
Select
Case @error
Return SetError(@error, 0, 0)
Case $Ret[0] = 0
Return SetError(4, _WinAPI_GetLastError(), 0)
Case Else
Return 1
EndSelect
EndFunc ;==>_WinAPI_SetLayeredWindowAttributes

Func Terminate()
Exit 0
EndFunc
