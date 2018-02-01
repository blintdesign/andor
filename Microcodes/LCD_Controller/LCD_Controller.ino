/**
 * This sketch is specifically for programming the EEPROM used in the 8-bit
 * decimal display decoder described in https://youtu.be/dLh1n2dErzE
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
   Output the address bits and outputEnable signal using shift registers.
*/
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}


/*
   Read a byte from the EEPROM at the specified address.
*/
byte readEEPROM(int address) {
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, INPUT);
  }
  setAddress(address, /*outputEnable*/ true);

  byte data = 0;
  for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
    data = (data << 1) + digitalRead(pin);
  }
  return data;
}


/*
   Write a byte to the EEPROM at the specified address.
*/
void writeEEPROM(int address, byte data) {
  setAddress(address, /*outputEnable*/ false);
  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    pinMode(pin, OUTPUT);
  }

  for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
    digitalWrite(pin, data & 1);
    data = data >> 1;
  }
  digitalWrite(WRITE_EN, LOW);
  delayMicroseconds(1);
  digitalWrite(WRITE_EN, HIGH);
  delay(10);
}


/*
   Read the contents of the EEPROM and print them to the serial monitor.
*/
void printContents() {
  for (int base = 0; base <= 2047; base += 16) {
    byte data[16];
    for (int offset = 0; offset <= 15; offset += 1) {
      data[offset] = readEEPROM(base + offset);
    }

    char buf[80];
    sprintf(buf, "%03x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x %02x %02x %02x %02x %02x %02x",
            base, data[0], data[1], data[2], data[3], data[4], data[5], data[6], data[7],
            data[8], data[9], data[10], data[11], data[12], data[13], data[14], data[15]);

    Serial.println(buf);
  }
}


void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);



  // Erase entire EEPROM
/*
  Serial.print("Erasing EEPROM");
  for (int address = 0; address <= 2047; address += 1) {
    writeEEPROM(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");
*/

  // Bit patterns for the digits 0..9
  //byte digits[] = { 0x7e, 0x30, 0x6d, 0x79, 0x33, 0x5b, 0x5f, 0x70, 0x7f, 0x7b };
  //byte digits[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };
  byte digits[] = { 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39 };

  Serial.println("Programming ones place");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value, digits[value % 10]);
  }
  Serial.println("Programming tens place");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + 256, digits[(value / 10) % 10]);
  }
  Serial.println("Programming hundreds place");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + 512, digits[(value / 100) % 10]);
  }
  
  Serial.println("Programming sign");
  for (int value = 0; value <= 255; value += 1) {
    writeEEPROM(value + 768, 0);
  }
 
  
  
  Serial.println("Programming ones place (twos complement)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + 1024, digits[abs(value) % 10]);
  }
  Serial.println("Programming tens place (twos complement)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + 1280, digits[abs(value / 10) % 10]);
  }
  Serial.println("Programming hundreds place (twos complement)");
  for (int value = -128; value <= 127; value += 1) {
    writeEEPROM((byte)value + 1536, digits[abs(value / 100) % 10]);
  }
  
  Serial.println("Programming sign (twos complement)");
  for (int value = -128; value <= 127; value += 1) {
    if (value < 0) {
      writeEEPROM((byte)value + 1792, 0x01);
    } else {
      writeEEPROM((byte)value + 1792, 0);
    }
  }


  



//***********************************************************************************************************************************
//Modifying delete leading zeros
//Added Code not to display leading zeros

Serial.println("Modifying tens place");  // Write 0's to 10 addresses starting at address 256 (100h)  9 digits
  for (int value = 256; value <= 265; value += 1) {
    writeEEPROM(value, 0xA0);
  }

Serial.println("Modifying hundreds place");  // Write 0's to 100 addresses starting at address 512 (200h)  99 digits
  for (int value = 512; value <= 611; value += 1) {
    writeEEPROM(value, 0xA0);    
  }


Serial.println("Modifying tens place (twos complement)");  // Write 0's to 10 addresses starting at address 1280 (100h)  9 digits
  for (int value = 1280; value <= 1289; value += 1) {
    writeEEPROM(value, 0xA0);
  }

Serial.println("Modifying hundreds place (twos complement)");  // Write 0's to 100 addresses starting at address 1536 (200h)  99 digits
  for (int value = 1536; value <= 1635; value += 1) {
    writeEEPROM(value, 0xA0);    
   }  


Serial.println("Modifying tens place (twos complement)");  // Write 0's to 10 addresses starting at address 1527 (100h)  9 digits
  for (int value = 1527; value <= 1535; value += 1) {
    writeEEPROM(value, 0xA0);
  }

Serial.println("Modifying hundreds place (twos complement)");  // Write 0's to 100 addresses starting at address 1693 (200h)  99 digits
  for (int value = 1693; value <= 1791; value += 1) {
    writeEEPROM(value, 0xA0);
   }  
//***********************************************************************************************************************************





    Serial.println("Programming commands");
    // A10 es A0 - A1
    writeEEPROM(1024, 0x38 );   // Function set, 8 bit, 2 lines, 5x7 
    writeEEPROM(1025, 0xF );    // Display ON, Cursor On, Cursor Blinking 
    writeEEPROM(1026, 0x6 );    // Entry Mode, Increment cursor position, No display shift 
    writeEEPROM(1027, 0x1 );    // Display clear
    writeEEPROM(1028, 0xff );   // Nothing






  // Read and print out the contents of the EERPROM
  Serial.println("Reading EEPROM");
  printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}
