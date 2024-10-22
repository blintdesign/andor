/*
 * *********************************************************************************
 * Processor microcode 
 * *********************************************************************************
 */
 
 // ROM 1
#define HLT 0b10000000000000000000000000000000    // Halt
#define MI  0b01000000000000000000000000000000    // Memory address input
#define RI  0b00100000000000000000000000000000    // ROM input
#define RO  0b00010000000000000000000000000000    // ROM output
#define IO  0b00001000000000000000000000000000    // Instruction register output
#define II  0b00000100000000000000000000000000    // Instruction register input
#define AI  0b00000010000000000000000000000000    // A register input
#define AO  0b00000001000000000000000000000000    // A register output

// ROM 2
#define EO  0b00000000100000000000000000000000    // Add
#define SU  0b00000000010000000000000000000000    // Subtract
#define BI  0b00000000001000000000000000000000    // B register input
#define IP  0b00000000000100000000000000000000    // Input Port
#define CE  0b00000000000010000000000000000000    // Counter enable
#define CO  0b00000000000001000000000000000000    // Counter output
#define J   0b00000000000000100000000000000000    // Jump
#define ICR 0b00000000000000010000000000000000    // Instruction clear

// ROM 3
#define SF  0b00000000000000001000000000000000    // Set flags
#define JZ  0b00000000000000000100000000000000    // Jump: Zero flag
#define JC  0b00000000000000000010000000000000    // Jump: Carry flag
#define JN  0b00000000000000000001000000000000    // Jump: Negative flag
#define JO  0b00000000000000000000100000000000    // Jump: Overflow flag
#define O1  0b00000000000000000000010000000000    // LCD 1
#define O2  0b00000000000000000000001000000000    // LCD 2
#define O3  0b00000000000000000000000100000000    // LCD 3

// ROM 4
#define ST  0b00000000000000000000000010000000    // Stack select
#define BO  0b00000000000000000000000001000000    // B register output
#define NA5 0b00000000000000000000000000100000    // ---
#define NA6 0b00000000000000000000000000010000    // ---
#define NA7 0b00000000000000000000000000001000    // ---
#define SND 0b00000000000000000000000000000100    // Send data to Sound chip
#define ROE 0b00000000000000000000000000000010    // RTC: Output Enable
#define RRW 0b00000000000000000000000000000001    // RTC: Read/Write


// Fetch (T1, T2)
#define FETCH MI|CO, RO|II|CE

// Null values ( T11 -> T16 )
#define NULLS ICR,0,0,0,0,0


const PROGMEM uint32_t data[] = {
/*
T1,T2   T3          T4           T5             T6           T7          T8         T9          T10     T11-T16     CMD    BIN     HEX    ADDRESSING
*/
FETCH,  ICR,        0,           0,             0,           0,          0,         0,          0,      NULLS,   // NOP    00000   0x00   Implied
FETCH,  CO|MI,      RO|MI|CE,    RO|AI|SF,      ICR,         0,          0,         0,          0,      NULLS,   // LDA    00010   0x10   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         AI|SF|EO,    ICR,        0,         0,          0,      NULLS,   // ADC    00100   0x20   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         AI|SU|SF|EO, ICR,        0,         0,          0,      NULLS,   // SUB    00110   0x30   Absolute
FETCH,  CO|MI,      RO|MI|CE,    AO|RI,         ICR,         0,          0,         0,          0,      NULLS,   // STA    01000   0x40   Absolute
FETCH,  CO|MI,      RO|AI|CE,    SF,            ICR,         0,          0,         0,          0,      NULLS,   // LDI    01010   0x05   Immediate
FETCH,  CO|MI,      RO|J,        ICR,           0,           0,          0,         0,          0,      NULLS,   // JMP    01100   0x60   Absolute
FETCH,  CO|MI,      RO|BI|CE,    AI|SU|SF|EO,   ICR,         0,          0,         0,          0,      NULLS,   // SUB    01110   0x70   Immediate
FETCH,  CO|MI,      CE|RO|RRW,   CO|MI,         RO|MI|CE,    ROE|RI,     CO|MI,     RO|MI|CE,   ROE|RI, NULLS,   // RTR    10000   0x80   Absolute
FETCH,  CO|MI,      CE|RO|RRW,   CO|MI,         RO|CE|RRW,   CO|MI,      RO|CE|RRW, ICR,        0,      NULLS,   // RTW    10010   0x90   Immediate
FETCH,  ST|MI,      ST|CO|RI,    CO|MI,         RO|J,        ICR,        0,         0,          0,      NULLS,   // JSR    10100   0xA0   Absolute
FETCH,  ST|MI,      ST|RO|J,     CE,            ICR,         0,          0,         0,          0,      NULLS,   // RTS    10110   0xB0   Implied
FETCH,  IP|AI|SF,   IP|ICR,      0,             0,           0,          0,         0,          0,      NULLS,   // IPR    11000   0xC0   Immediate
FETCH,  CO|MI,      RO|MI|CE,    IP|RI,         IP|ICR,      0,          0,         0,          0,      NULLS,   // IPR    11010   0xD0   Absolute
FETCH,  CO|MI,      CE|RO|O3|O1, RO|O1|O2,      RO|O1,       RO,         ICR,       0,          0,      NULLS,   // OUT.d  11100   0xE0   Immediate
FETCH,  HLT,        ICR,         0,             0,           0,          0,         0,          0,      NULLS,   // HLT    11110   0xF0   Implied
FETCH,  CO|MI,      RO|JZ,       CE,            ICR,         0,          0,         0,          0,      NULLS,   // BEQ    00001   0x08   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         SU|SF|EO,    ICR,        0,         0,          0,      NULLS,   // CMP    00011   0x18   Absolute
FETCH,  CO|MI,      RO|JC,       CE,            ICR,         0,          0,         0,          0,      NULLS,   // BCS    00101   0x28   Absolute
//FETCH,CO|MI,      RO|JN,       CE,            ICR,         0,          0,         0,          0,      NULLS,   // BMI    00111   0x38   Absolute
FETCH,  CO|MI,      CE|RO|SND,   RO|SND,        RO|SND,      ICR,        0,         0,          0,      NULLS,   // SND    00111   0x38   Absolute
FETCH,  AO|BI,      AI|SF|EO,    ICR,           0,           0,          0,         0,          0,      NULLS,   // ASL    01001   0x48   Implied
FETCH,  CO|MI,      RO|BI|CE,    AO|MI,         BO|RI,       ICR,        0,         0,          0,      NULLS,   // STA.a  01011   0x58
//FETCH,AO|BI,      AI|SU|SF|EO, ICR,           0,           0,          0,         0,          0,      NULLS,   // LSR    01011   0x58   Implied
FETCH,  CO|MI,      RO|BI|CE,    AI|SF|EO,      ICR,         0,          0,         0,          0,      NULLS,   // ADC    01101   0x68   Immediate
FETCH,  CO|MI,      RO|BI|CE,    SU|SF|EO,      ICR,         0,          0,         0,          0,      NULLS,   // CMP    01111   0x78   Immediate
FETCH,  AO|MI,      RO|AI|SF,    ICR,           0,           0,          0,         0,          0,      NULLS,   // LDA.a  10001   0x88   Absolute
//FETCH,CO|MI,      RO|MI|CE,    RO|O2|O3,      RO,          ICR,        0,         0,          0,      NULLS,   // LCD    10001   0x88   Absolute
FETCH,  CO|MI,      RO|O2|O3,    CE,            0,           0,          0,         0,          ICR,    NULLS,   // LCD    10011   0x98   Immediate
FETCH,  AO|O2|O3,   AO,          0,             0,           0,          0,         0,          ICR,    NULLS,   // LCD    10101   0xA8   Implied
FETCH,  CO|MI,      RO|MI|CE,      RO|O1|O2|O3, RO,          ICR,        0,         0,          0,      NULLS,   // OUT.c  10111   0xB8   Absolute
FETCH,  CO|MI,      CE|RO|O1|O2|O3,RO,          ICR,         0,          0,         0,          0,      NULLS,   // OUT.c  11001   0xC8   Immediate
FETCH,  AO|MI,      RO|O1|O2|O3,   RO,          ICR,         0,          0,         0,          0,      NULLS,   // OUT.c  11011   0xD8   Implied
FETCH,  CO|MI,      RO|MI|CE,    RO|O3|O1,      RO|O1|O2,    RO|O1,      RO,        ICR,        0,      NULLS,   // OUT.d  11101   0xE8   Absolute
FETCH,  AO|O3|O1,   AO|O1|O2,    AO|O1,         AO,          ICR,        0,         0,          0,      NULLS,   // OUT.d  11111   0xF8   Implied
};

//Absolute  : Megadott memoria cim
//Immediate : Kozvetlen ertek
//Implied   : A Reg



/*
 * *********************************************************************************
 * EEPROM WRITER
 * *********************************************************************************
 */
 
/*
 * Variables
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

int command;
int shift;

/*
 * Set EEPROM Address
 */
void setAddress(int address, bool outputEnable) {
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, (address >> 8) | (outputEnable ? 0x00 : 0x80));
  shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

  digitalWrite(SHIFT_LATCH, LOW);
  digitalWrite(SHIFT_LATCH, HIGH);
  digitalWrite(SHIFT_LATCH, LOW);
}

/*
 * Write a byte to the EEPROM at the specified address.
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
 * Read a byte from the EEPROM at the specified address.
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
 * Init the EEPROM Writer
 */
void setup() {
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  Serial.println("Write specified EEPROM: 1, 2, 3, 4 (n = EEPROM number)");
  Serial.println("Write shifted data: a");
  Serial.println("Erase EEPROM: e");
  Serial.println("Read EEPROM: r \n");
}

/*
 * Program loop
 */
void loop() {
  if (Serial.available() > 0) {
    command = Serial.read();

    switch (command) {
      
      //Erase EEPROM
      case 'e':
        Serial.print("Erasing EEPROM .");
        
        for (int address = 0; address <= 512; address += 1) {
          //writeEEPROM(address, 0xff);
          if (address % 64 == 0) Serial.print(".");
        }

        Serial.print(" Done!\n\n");
      break;

      //Read EEPROM
      case 'r':
        Serial.print("Reading EEPROM ... \n\n");
      
        for (int base = 0; base <= 512; base += 16) {
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
      break;

      //Write EEPROM
      case '5':
      case '4':
      case '3':
      case '2':
      case '1':
        Serial.print("Writeing EEPROM .");
        
        writeEEPROM( 0x200, 0x2 );
        writeEEPROM( 0x201, 0x4 );
        writeEEPROM( 0x202, 0x6 );
        writeEEPROM( 0x203, 0x8 );

        for (int address = 0; address < sizeof(data)/sizeof( pgm_read_dword(data + 0) ); address += 1) {
/*
          if ( command == '1' )  writeEEPROM(address + 2048, pgm_read_dword(data + address) >> 0 );
          if ( command == '2' )  writeEEPROM(address + 1536, pgm_read_dword(data + address) >> 8 );
          if ( command == '3' )  writeEEPROM(address + 1024, pgm_read_dword(data + address) >> 16 );
          if ( command == '4' )  writeEEPROM(address +  512, pgm_read_dword(data + address) >> 24 );
          if ( command == '5' )  writeEEPROM(address,        pgm_read_dword(data + address) >> 32 );
*/
          if (address % 64 == 0) Serial.print(".");
        }

        Serial.print(" Done!\n\n");
      break;

      // Write all EEPROM
      case 'a':
        Serial.print("Writeing EEPROM .");
      
        for (int address = 0; address < sizeof(data)/sizeof( pgm_read_dword(data + 0) ); address += 1) {
          writeEEPROM(address + 1536, pgm_read_dword(data + address) >> 0 );
          writeEEPROM(address + 1024, pgm_read_dword(data + address) >> 8 );
          writeEEPROM(address +  512, pgm_read_dword(data + address) >> 16 );
          writeEEPROM(address,        pgm_read_dword(data + address) >> 24 );

          if (address % 64 == 0) Serial.print(".");
        }

        Serial.print(" Done!\n\n");
      break;

    }
  }
}
