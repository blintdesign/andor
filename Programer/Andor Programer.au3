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

; PC Ports:
$DataPort = "0xE880";
$CtrlPort = "0xE882";

; Laptop Ports:
;$DataPort = "0x378";
;$CtrlPort = "0x37A";


; Inicialize ports
Dim $StatusRegisterAddress, $ControlRegisterAddress
CalculateRegisterAddresses($DataPort)
CalculateRegisterAddresses($CtrlPort)


;test for DLL
If FileExists($DLLFileAndPath) = 0 Then
    MsgBox(0, "Missing DLL File", "Parallel Port Control DLL Missing at '" & $DLLFileAndPath & "'. Cannot Continue!" )
    Exit
 EndIf


; Application GUI
$hGUI = GUICreate("ANDOR Programer v0.3", 795, 700, default, default, BitOR($GUI_SS_DEFAULT_GUI,$WS_SIZEBOX,$WS_MAXIMIZEBOX,$WS_THICKFRAME))


; Main menu
$idFileMenu = GUICtrlCreateMenu("&File")
$idRecentFilesMenu = GUICtrlCreateMenu("Recent Files", $idFileMenu)
$idSeparator1 = GUICtrlCreateMenuItem("", $idFileMenu)
$idExitItem = GUICtrlCreateMenuItem("Exit", $idFileMenu)

$idMenuOpen = GUICtrlCreateMenuItem("&Open", -1)
$idMenuOpenBin = GUICtrlCreateMenuItem("&Open Bin", -1)
$idMenuSave = GUICtrlCreateMenuItem("&Save", -1)


; Labels
$machineCode = GUICtrlCreateLabel("Machine code", 500, 50)
$byteCount   = GUICtrlCreateLabel("0 / 256 bytes", 590, 50, 200, 20, BitOR($SS_RIGHT,$SS_CENTERIMAGE))
$eventLog    = GUICtrlCreateLabel("Event log", 500, 260)

GUICtrlSetResizing($machineCode, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($byteCount, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($eventLog, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)


; Edit boxes
$FieldEditor = GUICtrlCreateEdit("", 5, 5, 485, 600, BitOR($ES_WANTRETURN, $ES_MULTILINE, $ES_NOHIDESEL, $WS_VSCROLL))
GUICtrlSetFont(-1, 11, $FW_NORMAL, -1, "Consolas")

$FieldHex = GUICtrlCreateEdit("",  500, 70, 290, 175, BitOR($ES_WANTRETURN, $ES_MULTILINE, $ES_NOHIDESEL, $WS_VSCROLL))
GUICtrlSetFont(-1, 11, $FW_NORMAL, -1, "Consolas")

$FieldLog = GUICtrlCreateEdit("",  500, 280, 290, 325, BitOR($ES_WANTRETURN, $ES_MULTILINE, $WS_VSCROLL))
GUICtrlSetFont(-1, 10, $FW_NORMAL, -1, "Consolas")

GUICtrlSetResizing($FieldEditor, $GUI_DOCKTOP + $GUI_DOCKRIGHT + $GUI_DOCKLEFT + $GUI_DOCKBOTTOM )
GUICtrlSetResizing($FieldHex, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($FieldLog, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT + $GUI_DOCKBOTTOM )


; Buttons
$idCompALad = GUICtrlCreateButton("Complie && Load", 500, 5, 110, 30)
$idComplie  = GUICtrlCreateButton("Complie", 615, 5, 85, 30)
$idLoad     = GUICtrlCreateButton("Load", 705, 5, 85, 30)

$idReset = GUICtrlCreateButton("Reset", 690, 620, 100, 25)
$idMI    = GUICtrlCreateButton("MI",  120, 620, 50, 25)
$idRI    = GUICtrlCreateButton("RI",  180, 620, 50, 25)
$idCLK   = GUICtrlCreateButton("CLK", 240, 620, 50, 25)
$idDEF   = GUICtrlCreateButton("Defaults", 300, 620, 100, 25)

GUICtrlSetResizing($idCompALad, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($idComplie, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($idLoad, $GUI_DOCKTOP + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)
GUICtrlSetResizing($idReset, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKRIGHT)

GUICtrlSetResizing($idDEF, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)
GUICtrlSetResizing($idMI, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)
GUICtrlSetResizing($idRI, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)
GUICtrlSetResizing($idCLK, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)

; Status bar
$StatusBar = _GUICtrlStatusBar_Create($hGUI)
GUICtrlSetResizing($StatusBar, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)


; Address pager
$AddrPager = GUICtrlCreateInput("0", 5, 620, 100, 25)
GUICtrlCreateUpdown($AddrPager)
GUICtrlSetResizing($AddrPager, $GUI_DOCKBOTTOM + $GUI_DOCKSIZE + $GUI_DOCKLEFT)


GUICtrlSetLimit(-1, 0xFFFFFF)
ControlClick($hGUI, "", $FieldEditor)



GUIRegisterMsg($WM_SIZE, "WM_SIZE")

HotKeySet("^a", "_selectall")


GUISetState(@SW_SHOW)


; Resize the status bar when GUI size changes
Func WM_SIZE($hWnd, $iMsg, $wParam, $lParam)
    #forceref $hWnd, $iMsg, $wParam, $lParam
    _GUICtrlStatusBar_Resize($StatusBar)
    Return $GUI_RUNDEFMSG
EndFunc   ;==>WM_SIZE




; P=   SZ=   T=   N=CLK
SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )


 Func _selectall()
     _SendMessage(ControlGetHandle("", "", ""), $EM_SETSEL, 0, -1)
  EndFunc  ;==>_selectall



Func Reset()
   SetLPTPortPins($CtrlPort, BinToDec( 0000 ) )  ; Reset
   Sleep(1);
   SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )
EndFunc


Func AddrJump()
   $addr = GUICtrlRead( $AddrPager )

   Guictrlsetdata($FieldLog, $addr )
#comments-start
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
#comments-end
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


; File open function

Func OpenBin()
   ; Create a constant variable in Local scope of the message to display in FileOpenDialog.
   Local Const $sMessage = "Open binary file"

   ; Display an open dialog to select a list of file(s).
   Local $sFileOpenDialog = FileOpenDialog($sMessage, @ScriptDir & "\", "Machine Code (*.bin)", $FD_FILEMUSTEXIST )

   If Not @error Then
      ; Change the working directory (@WorkingDir)
      FileChangeDir(@ScriptDir)

      ; Replace instances of "|" with @CRLF in the string returned by FileOpenDialog.
      $sFileOpenDialog = StringReplace($sFileOpenDialog, "|", @CRLF)

	  GUICtrlCreateMenuItem($sFileOpenDialog, $idRecentFilesMenu)

	  ; Load file to hex editor
	  $file = FileOpen( $sFileOpenDialog )
	  Guictrlsetdata($FieldHex, FileRead($file))
	  FileClose($file)

   EndIf
EndFunc


#include <Array.au3>
Func Complie()
   $scr_file = @ScriptDir & "/asm/source.asm"
   $hex_file = @ScriptDir & "/asm/source.hex"
   FileDelete( $scr_file )
   FileWrite( $scr_file, GUICtrlRead($FieldEditor) & @CRLF)

   $foo = Run( @ScriptDir & "/asm/casm.exe -f hexstr -o source.hex andor.cpu source.asm", @ScriptDir & "/asm" ,  @SW_HIDE, $STDERR_CHILD + $STDOUT_CHILD)

   Local $sOut = ''
   While 1
	   $sOut &= StdoutRead($foo)
	   ;$line = StdoutRead($foo)
	   ;If @error Then ExitLoop
	   ;$sOut &= $line

            If @error Then
                ExitLoop
            Else
                GUICtrlSetData($FieldLog, @LF, True)
                GUICtrlSetData($FieldLog, $sOut, True)
            EndIf

	Wend



   ;Guictrlsetdata($FieldLog, $sOut)

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

   Reset();
   Sleep( $delay )

   For $x = 1 To $split[0]
	  $log = "Sending data to cumputer..."
	  Guictrlsetdata($FieldLog, $log )

	  _GUICtrlStatusBar_SetText($StatusBar, "Address: 0x" & HEX($x - 1, 2) & " Data: 0x" & $split[$x]   )

	  ; Set memory address
	  SetLPTPortPins($DataPort, $x - 1 )		; Address
	  SetLPTPortPins($CtrlPort, BinToDec( 0110 ) )		; MI
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 0111 ) )		; MI | CLK
	  Sleep( $delay )

	  ; Write data
	  SetLPTPortPins($DataPort, BinToDec(_HexToBinaryString(  $split[$x] ) ) )	; Data
	  SetLPTPortPins($CtrlPort, BinToDec( 1010 ) )			; RI
	  Sleep( $delay )
	  SetLPTPortPins($CtrlPort, BinToDec( 1011 ) )			; RI|CLK
	  Sleep( $delay )

   Next

   Sleep( 10 )
   SetLPTPortPins($DataPort, 0 )
   SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )

   Reset();

   $log = "Done"
   Guictrlsetdata($FieldLog, $log )

   $log = "Controll Port: " & $CtrlPort & "; Data Port: " & $DataPort
   Guictrlsetdata($FieldLog, $log )
EndFunc

Func SetPorts()
   $CtrlPort = GUICtrlRead( $CtrlAddr )
   $DataPort = GUICtrlRead( $DataAddr )

   $log = "Controll Port: " & $CtrlPort & "; Data Port: " & $DataPort
   Guictrlsetdata($FieldLog, $log )

EndFunc

Func SetState($sate)

   ; P=MI   SZ=RI   T=RO   N=CLK
   ; 1100 - Init

   Switch $sate
	  Case "CLK"
		 SetLPTPortPins($CtrlPort, BinToDec( 1101 ) ) ; Clock
		 Sleep(100)
		 SetLPTPortPins($CtrlPort, BinToDec( 1100 ) )
	  Case "MI"
		 SetLPTPortPins($CtrlPort, BinToDec( 0110 ) ) ; MI
	  Case "RI"
		 SetLPTPortPins($CtrlPort, BinToDec( 1010 ) ) ; RI
	  Case "DEF"
		 SetLPTPortPins($CtrlPort, BinToDec( 1100 ) ) ; Default
    EndSwitch
EndFunc


Func ComplieAndLoad()
   Complie()
   Load()
EndFunc


Do
   Switch GUIGetMsg()
	  Case $AddrPager
		 AddrJump()
	  Case $idComplie
		 Complie()
	  Case $idMenuOpen
		 Open()
	  Case $idMenuOpenBin
		 OpenBin()
	  Case $idLoad
		 Load()
	  Case $idCompALad
		 ComplieAndLoad()
	  Case $idReset
		 Reset()
	  Case $idCLK
		 SetState("CLK")
	  Case $idMI
		 SetState("MI")
	  Case $idRI
		 SetState("RI")
	  Case $idDEF
		 SetState("DEF")
	        Case $GUI_EVENT_CLOSE
         GUIDelete()
         Exit
    EndSwitch
 Until False




