

;Functions -> where the action happens!
Func SetLPTPortPins($WriteAddress, $data)
   ;pass the call to inpout32.dll
    DllCall( $DLLFileAndPath, "int", "Out32", "int", $WriteAddress, "int", $data)
EndFunc  ;==>SetLPTPortPins

Func ReadPortStatus($ReadAddress)
;Read the port register (returns a BCD value)
    $CurrentPortStatusArray = DllCall($DLLFileAndPath, "int", "Inp32", "int", $ReadAddress)
    $StatusToDecode = $CurrentPortStatusArray[0]
;convert to bit status & store in array
    Dim $BitsReadArray[9]; [0] -> [7] the decoded bits, [8] the raw BCD value
    $BitsReadArray[8] = $CurrentPortStatusArray[0]
    $CurrentBitValue = 128
    For $BitCounter = 7 To 0 Step - 1
        If $StatusToDecode >= $CurrentBitValue Then
            $BitsReadArray[$BitCounter] = 1
            $StatusToDecode = $StatusToDecode - $CurrentBitValue
        Else
            $BitsReadArray[$BitCounter] = 0
        EndIf
        $CurrentBitValue = $CurrentBitValue / 2
    Next
;test if good decode obtained
    If $StatusToDecode <> 0 Then
        MsgBox(0, "Decoding error", "Error in decoding port '" & $PortAddress & "' register '" & $ReadAddress & " status '" & $CurrentPortStatusArray[0] & "' to bits.  Do not rely on the results.")
    EndIf

    Return $BitsReadArray
EndFunc  ;==>ReadPortStatus


Func CalculateRegisterAddresses($BaseAddress)
;check for correct hex prefix
    If Not (StringLeft($BaseAddress, 2) = "0x" Or StringLeft($BaseAddress, 2) = "0X") Then
        MsgBox(0, "Invalid Hex Notation", "The port address  " & $BaseAddress & "' entered was not in valid Hexadecimal notation.  It must start with '0x'")
        Exit
    EndIf
;check for valid hex characters in suffix
    If StringIsXDigit(StringTrimLeft($BaseAddress, 2)) = 0 Then
        MsgBox(0, "Invalid Hex Notation", "The port address  " & $BaseAddress & "' entered was not in valid Hexadecimal notation.  The characters after the '0x' must be only 0-9 and A-F")
        Exit
    EndIf
;calculate status register address
;Msgbox (0,"Debug","$BaseAddress = "& $BaseAddress & @CRLF & "StringTrimLeft($BaseAddress, 2) = " & StringTrimLeft($BaseAddress, 2) & @CRLF & "Dec(StringTrimLeft($BaseAddress, 2)) = " & Dec(StringTrimLeft($BaseAddress, 2)) & @CRLF & "Hex(Dec(StringTrimLeft($BaseAddress, 2)) + 1) = " & Hex(Dec(StringTrimLeft($BaseAddress, 2)) + 1))
    $RawHex = Hex(Dec(StringTrimLeft($BaseAddress, 2)) + 1)
;trim leading zeros
    While 1
        If StringLeft($RawHex, 1) = "0" Then
            $RawHex = StringTrimLeft($RawHex, 1)
        Else
            ExitLoop
        EndIf
    WEnd
    $StatusRegisterAddress = "0x" & $RawHex
;calculate control register address
    $RawHex = Hex(Dec(StringTrimLeft($BaseAddress, 2)) + 2)
;trim leading zeros
    While 1
        If StringLeft($RawHex, 1) = "0" Then
            $RawHex = StringTrimLeft($RawHex, 1)
        Else
            ExitLoop
        EndIf
    WEnd
    $ControlRegisterAddress = "0x" & $RawHex

 EndFunc  ;==>CalculateRegisterAddresses
