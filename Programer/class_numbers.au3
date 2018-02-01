Func DecToBin($iNumber)
    Local $sBinString = ""
    Do
        $sBinString = BitAND($iNumber, 1) & $sBinString
        $iNumber = BitShift($iNumber, 1)
    Until $iNumber <= 0
    If $iNumber < 0 Then SetError(1, 0, 0)
    Return $sBinString
EndFunc   ;==>_Convert_To_Binary


Func BinToDec($B)
    Return BitOr((StringLen($B) > 1 ? BitShift(BinToDec(StringTrimRight($B, 1)), -1) : 0), StringRight($B, 1))
 EndFunc    ;==> _BinToDec()


; Hex to Decimal Conversion ; Correct till Decimal 65789 ?!
Func _HexToDecimal_NotCorrect($Input)
Local $Temp, $i, $Pos, $Ret, $Output

If StringRegExp($input,'[[:xdigit:]]') then
$Temp = StringSplit($Input,"")

For $i = 1 to $Temp[0]
    $Pos = $Temp[0] - $i
    $Ret = Dec (Hex ("0x" & $temp[$i] )) * 16 ^ $Pos
    $Output &= $Ret
Next
    return $Output
Else
    MsgBox(0,"Error","Wrong input, try again ...")
    Return
EndIf
EndFunc

; Hex To Binary
Func _HexToBinaryString($HexValue)
Local $Allowed = '0123456789ABCDEF'
Local $Test,$n
Local $Result = ''
if $hexValue = '' then
SetError(-2)
Return
EndIf

$hexvalue = StringSplit($hexvalue,'')
for $n = 1 to $hexValue[0]
if not StringInStr($Allowed,$hexvalue[$n]) Then
SetError(-1)
return 0
EndIf
Next

Local $bits = "0000|0001|0010|0011|0100|0101|0110|0111|1000|1001|1010|1011|1100|1101|1110|1111"
$bits = stringsplit($bits,'|')
for $n = 1 to $hexvalue[0]
$Result &= $bits[Dec($hexvalue[$n])+1]
Next

Return $Result

EndFunc
