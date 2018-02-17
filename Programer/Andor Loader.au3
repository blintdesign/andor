#include <WindowsConstants.au3>
#include <GUIConstants.au3>
#include <GUIConstantsEx.au3>
#include <FileConstants.au3>
#include <MsgBoxConstants.au3>
#include <GUIConstantsEx.au3>
#include <GuiEdit.au3>
#include <FontConstants.au3>
#Include <Constants.au3>

#include "class_lpt.au3"
#include "class_numbers.au3"


; Port variablee
$DLLFileAndPath = @ScriptDir & "/inpout32.dll";
$DataPort = "0xE880";
$CtrlPort = "0xE882";

; Inicialize ports
Dim $StatusRegisterAddress, $ControlRegisterAddress
CalculateRegisterAddresses($DataPort)
CalculateRegisterAddresses($CtrlPort)


;test for DLL
If FileExists($DLLFileAndPath) = 0 Then
    MsgBox(0, "Missing DLL File", "Parallel Port Control DLL Missing at '" & $DLLFileAndPath & "'. Cannot Continue!" )
    Exit
 EndIf


; Create application GUI

$hGUI = GUICreate("ANDOR Programer v0.2", 695, 700, -1, -1, BitOR($GUI_SS_DEFAULT_GUI,$WS_SIZEBOX,$WS_THICKFRAME))


	$idFileMenu = GUICtrlCreateMenu("File")
	$idFileItem = GUICtrlCreateMenuItem("Open...", $idFileMenu)
	$idRecentFilesMenu = GUICtrlCreateMenu("Recent Files", $idFileMenu)
	$idSeparator1 = GUICtrlCreateMenuItem("", $idFileMenu)
	$idExitItem = GUICtrlCreateMenuItem("Exit", $idFileMenu)
	$idHelpMenu = GUICtrlCreateMenu("?")
	$idAboutItem = GUICtrlCreateMenuItem("About", $idHelpMenu)

$FieldEditor = GUICtrlCreateEdit("", 5, 50, 390, 545, BitOR($ES_WANTRETURN, $ES_MULTILINE, $ES_NOHIDESEL, $WS_VSCROLL))
GUICtrlSetFont(-1, 11, $FW_NORMAL, -1, "Consolas")

GUICtrlCreateLabel("Machine code", 400, 50)
$byteCount = GUICtrlCreateLabel("0 / 256 bytes", 490, 50, 200, 20, BitOR($SS_RIGHT,$SS_CENTERIMAGE))
$FieldHex = GUICtrlCreateEdit("",  400, 70, 290, 175, BitOR($ES_WANTRETURN, $ES_MULTILINE, $ES_NOHIDESEL))
GUICtrlSetFont(-1, 11, $FW_NORMAL, -1, "Consolas")


GUICtrlCreateLabel("Event logs", 400, 250)
$FieldLog = GUICtrlCreateEdit("",  400, 270, 290, 325, BitOR($ES_WANTRETURN, $ES_MULTILINE, $WS_VSCROLL))
GUICtrlSetFont(-1, 10, $FW_NORMAL, -1, "Consolas")

$idOpen = GUICtrlCreateButton("Open", 5, 10, 85, 25)
$idSave = GUICtrlCreateButton("Save", 95, 10, 85, 25)

$idComplie  = GUICtrlCreateButton("Complie", 515, 10, 85, 25)
$idLoad     = GUICtrlCreateButton("Load", 605, 10, 85, 25)


$AddrPager = GUICtrlCreateInput("0", 5, 600, 100, 25)
GUICtrlCreateUpdown($AddrPager)


$StatusBar = _GUICtrlStatusBar_Create($hGUI)

GUICtrlSetResizing(-1, $GUI_DOCKAUTO)
GUICtrlSetLimit(-1, 0xFFFFFF)
ControlClick($hGUI, "", $FieldEditor)



HotKeySet("^a", "_selectall")


GUISetState(@SW_SHOW)


SetLPTPortPins($CtrlPort, BinToDec( 1111 ) )




 Func _selectall()
     _SendMessage(ControlGetHandle("", "", ""), $EM_SETSEL, 0, -1)
  EndFunc  ;==>_selectall







Func AddrJump()
   $addr = GUICtrlRead( $AddrPager )

   Guictrlsetdata($FieldLog, $addr )

   $delay = 10

	  ; Set memory address
	  SetLPTPortPins($DataPort, $addr )		; Address
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1101 ) )		; MI
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )		; MI | CLK
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 0001 ) )		; RI|CLK 2
	  Sleep( $delay )


	  SetLPTPortPins($DataPort, 0 )		; Address
	  SetLPTPortPins($CtrlPort, BinToDec( 1111 ) )

EndFunc


; File open function

Func Open()
   ; Create a constant variable in Local scope of the message to display in FileOpenDialog.
   Local Const $sMessage = "Open program file"

   ; Display an open dialog to select a list of file(s).
   Local $sFileOpenDialog = FileOpenDialog($sMessage, @ScriptDir & "\", "Source Code (*.asm)", $FD_FILEMUSTEXIST )

   If Not @error Then
      ; Change the working directory (@WorkingDir)
      FileChangeDir(@ScriptDir)

      ; Replace instances of "|" with @CRLF in the string returned by FileOpenDialog.
      $sFileOpenDialog = StringReplace($sFileOpenDialog, "|", @CRLF)

	  GUICtrlCreateMenuItem($sFileOpenDialog, $idRecentFilesMenu)

	  ; Load file to editor
	  $file = FileOpen( $sFileOpenDialog )
	  Guictrlsetdata($FieldEditor, FileRead($file))
	  FileClose($file)

   EndIf
EndFunc


Func Complie()

   $scr_file = @ScriptDir & "/asm/source.asm"
   $hex_file = @ScriptDir & "/asm/source.hex"
   FileDelete( $scr_file )
   FileWrite( $scr_file, GUICtrlRead($FieldEditor) & @CRLF)


   $foo = Run( @ScriptDir & "/asm/casm.exe -f hexstr -o source.hex ao.cpu source.asm", @ScriptDir & "/asm" , @SW_HIDE, $STDOUT_CHILD)

   Local $sOut = ''
   While 1
	   $line = StdoutRead($foo)
	   If @error Then ExitLoop
	   $sOut &= $line
   Wend

   Guictrlsetdata($FieldLog, $sOut)

   $file = FileOpen( $hex_file )
   $data = FileRead($file)
   FileClose($file)


   Dim $result

   $aNumber = StringRegExp($data, "\N{2}", 3)

   $used = 0
   $free = 256

   For $i = 0 To UBound($aNumber) - 1
	   $result &= $aNumber[$i] & " "
	   $used += 1;
	   $free -= 1;
   Next

   $result = StringTrimRight($result, 1)

   Guictrlsetdata($FieldHex, $result)
   Guictrlsetdata($byteCount, $used & " / " & $free & " bytes")
EndFunc




Func Load()
   $delay = 5

   $split = StringSplit( GUICtrlRead( $FieldHex ) ," ")

   SetLPTPortPins($CtrlPort, BinToDec( 1101 ) )		; Init
   Sleep( 100 )

   For $x = 1 To $split[0]

	  ;$log = _HexToBinaryString(  $split[$x] ) & " - " & Dec( $split[$x] ) & @CRLF
	  $log = "Sending data to cumputer..."
	  Guictrlsetdata($FieldLog, $log )

	  _GUICtrlStatusBar_SetText($StatusBar, "Address: 0x" & HEX($x - 1, 2) & " Data: 0x" & $split[$x]   )

	  ; Set memory address
	  SetLPTPortPins($DataPort, $x - 1 )		; Address
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1101 ) )		; MI
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )		; MI | CLK
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1101 ) )		; RI|CLK 2
	  Sleep( $delay )

	  ; Write data
	  SetLPTPortPins($DataPort, BinToDec(_HexToBinaryString(  $split[$x] ) ) )	; Data
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 0001 ) )			; RI
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 0000 ) )			; RI|CLK
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 0001 ) )			; RI|CLK 2
	  Sleep( $delay )
   Next



   Sleep( 100 )
   SetLPTPortPins($DataPort, 0 )
   SetLPTPortPins($CtrlPort, BinToDec( 1111 ) )

   $log = "Done"
   Guictrlsetdata($FieldLog, $log )

EndFunc



Do
   Switch GUIGetMsg()
	  Case $idOpen
		 Open()
	  Case $AddrPager
		 AddrJump()
	  Case $idComplie
		 Complie()
	  Case $idLoad
		 Load()
      Case $GUI_EVENT_CLOSE
         GUIDelete()
         Exit
    EndSwitch
 Until False




