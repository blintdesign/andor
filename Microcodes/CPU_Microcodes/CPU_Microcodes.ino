/*
 * Microcode
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
#define NA  0b00000000000100000000000000000000    // ---
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
#define NA4 0b00000000000000000000000001000000    // ---
#define NA5 0b00000000000000000000000000100000    // ---
#define NA6 0b00000000000000000000000000010000    // ---
#define NA7 0b00000000000000000000000000001000    // ---
#define RCS 0b00000000000000000000000000000100    // RTC: Chis Select
#define ROE 0b00000000000000000000000000000010    // RTC: Output Enable
#define RRW 0b00000000000000000000000000000001    // RTC: Read/Write

// Fetch (T1, T2)
#define FETCH MI|CO, RO|II|CE

// Null values ( T11 -> T16 )
#define NULS 0,0,0,0,0,0

const PROGMEM uint32_t data[] = {
/*
T1,T2   T3          T4           T5             T6           T7            T8        T9          T10  T11+       CMD    BIN      HEX    ADDRESSING
*/
FETCH,  ICR,        0,           0,             0,           0,            0,        0,          0,   NULS,   // NOP    00000    0x00   Implied
FETCH,  CO|MI,      RO|MI|CE,    RO|AI|SF,      ICR,         0,            0,        0,          0,   NULS,   // LDA    00010    0x10   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         AI|SF|EO,    ICR,          0,        0,          0,   NULS,   // ADC    00100    0x20   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         AI|SU|SF|EO, ICR,          0,        0,          0,   NULS,   // SUB    00110    0x30   Absolute
FETCH,  CO|MI,      RO|MI|CE,    AO|RI,         ICR,         0,            0,        0,          0,   NULS,   // STA    01000    0x40   Absolute
FETCH,  CO|MI,      RO|AI|CE,    SF,            ICR,         0,            0,        0,          0,   NULS,   // LDI    01010    0x05   Immediate
FETCH,  CO|MI,      RO|J,        ICR,           0,           0,            0,        0,          0,   NULS,   // JMP    01100    0x60   Absolute
FETCH,  CO|MI,      RO|BI|CE,    AI|SU|SF|EO,   ICR,         0,            0,        0,          0,   NULS,   // SUB    01110    0x70   Immediate
FETCH,  IO|RRW|RCS, CO|MI,       RO|MI|CE,      RCS|ROE|RI,  CO|MI, RO|MI|CE, RCS|ROE|RI,        ICR, NULS,   // RTR    10000    0x08   Absolute
FETCH,  IO|RRW|RCS, CO|MI,       RO|CE|RCS|RRW, CO|MI,       RO|CE|RCS|RRW,ICR,      0,          0,   NULS,   // RTW    10010    0x09   Immediate
FETCH,  ST|MI,      ST|CO|RI,    CO|MI,         RO|J,        ICR,          0,        0,          0,   NULS,   // JSR    10100    0xA0   Absolute
FETCH,  ST|MI,      ST|RO|J,     CE,            ICR,         0,            0,        0,          0,   NULS,   // RTS    10110    0xB0   Implied
FETCH,  0,          0,           0,             0,           0,            0,        0,          0,   NULS,   //        11000    0xC0   
FETCH,  0,          0,           0,             0,           0,            0,        0,          0,   NULS,   //        11010    0xD0   
FETCH,  CO|MI,      CE|RO|O2|O1, RO|O3|O1,      RO|O2|O3|O1, RO,           ICR,      0,          0,   NULS,   // OUT.d   11100   0xE0   Immediate
FETCH,  HLT,        ICR,         0,             0,           0,            0,        0,          0,   NULS,   // HLT    11110    0xF0   Implied
FETCH,  CO|MI,      RO|JZ,       CE,            ICR,         0,            0,        0,          0,   NULS,   // BEQ    00001    0x08   Absolute
FETCH,  CO|MI,      RO|MI|CE,    RO|BI,         SU|SF|EO,    ICR,          0,        0,          0,   NULS,   // CMP    00011    0x18   Absolute
FETCH,  CO|MI,      RO|JC,       CE,            ICR,         0,            0,        0,          0,   NULS,   // BCS    00101    0x28   Absolute
FETCH,  CO|MI,      RO|JN,       CE,            ICR,         0,            0,        0,          0,   NULS,   // BMI    00111    0x38   Absolute
FETCH,  AO|BI,      AI|SF|EO,    ICR,           0,           0,            0,        0,          0,   NULS,   // ASL    01001    0x48   Implied
FETCH,  AO|BI,      AI|SU|SF|EO, ICR,           0,           0,            0,        0,          0,   NULS,   // LSR    01011    0x58   Implied
FETCH,  CO|MI,      RO|BI|CE,    AI|SF|EO,      ICR,         0,            0,        0,          0,   NULS,   // ADC    01101    0x68   Immediate
FETCH,  CO|MI,      RO|BI|CE,    SU|SF|EO,      ICR,         0,            0,        0,          0,   NULS,   // CMP    01111    0x78   Immediate
FETCH,  CO|MI,      RO|MI|CE,    RO|O2|O3,      RO,          ICR,          0,        0,          0,   NULS,   // LCD    10001    0x88   Absolute
FETCH,  CO|MI,      RO|O2|O3|CE, ICR,           0,           0,            0,        0,          0,   NULS,   // LCD    10011    0x98   Immediate
FETCH,  AO|O2|O3,   AO,          ICR,           0,           0,            0,        0,          0,   NULS,   // LCD    10101    0xA8   Implied
FETCH,  CO|MI,      RO|MI|CE,    RO|O1,         RO,          ICR,          0,        0,          0,   NULS,   // OUT.c  10111    0xB8   Absolute
FETCH,  CO|MI,      CE|RO|O1,    RO,            ICR,         0,            0,        0,          0,   NULS,   // OUT.c  11001    0xC8   Immediate
FETCH,  AO|MI,      RO|O1,       RO,            ICR,         0,            0,        0,          0,   NULS,   // OUT.c  11011    0xD8   Implied
FETCH,  CO|MI,      RO|MI|CE,    RO|O2|O1,      RO|O3|O1,    RO|O2|O3|O1,  RO,       ICR,        0,   NULS,   // OUT.d  11101    0xE8   Absolute
FETCH,  AO|O2|O1,   AO|O3|O1,    AO|O2|O3|O1,   AO,          ICR,          0,        0,          0,   NULS,   // OUT.d  11111    0xF8   Implied

/*
FETCH,  0,          0,           0,             0,           0,            0,        0,          0,   NULS,   // OUT.d  000001   0x04   
*/
};


/*
 * Pin definicion
 */
#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

/*
 * Output the address bits and outputEnable signal using shift registers.
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

void setup() {
  // put your setup code here, to run once:
  pinMode(SHIFT_DATA, OUTPUT);
  pinMode(SHIFT_CLK, OUTPUT);
  pinMode(SHIFT_LATCH, OUTPUT);
  digitalWrite(WRITE_EN, HIGH);
  pinMode(WRITE_EN, OUTPUT);
  Serial.begin(57600);

  // Program data bytes
  Serial.println("Writing");

  for (int address = 0; address < sizeof(data)/sizeof( pgm_read_dword(data + 0) ); address += 1) {    
    writeEEPROM(address,        pgm_read_dword(data + address) >> 24 );
    writeEEPROM(address +  512, pgm_read_dword(data + address) >> 16 );
    writeEEPROM(address + 1024, pgm_read_dword(data + address) >> 8 );
    writeEEPROM(address + 1536, pgm_read_dword(data + address) >> 0 );
  
    if (address % 64 == 0) Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("Done!");
}


void loop() {}
