/*
 * Microcode
 */
 
// ROM 1
#define HLT 0b10000000000000000000000000000000
#define MI  0b01000000000000000000000000000000
#define RI  0b00100000000000000000000000000000
#define RO  0b00010000000000000000000000000000
#define IO  0b00001000000000000000000000000000
#define II  0b00000100000000000000000000000000
#define AI  0b00000010000000000000000000000000
#define AO  0b00000001000000000000000000000000

// ROM 2
#define EO  0b00000000100000000000000000000000
#define SU  0b00000000010000000000000000000000
#define BI  0b00000000001000000000000000000000
#define OI  0b00000000000100000000000000000000    // OUT DEC
#define CE  0b00000000000010000000000000000000
#define CO  0b00000000000001000000000000000000
#define J   0b00000000000000100000000000000000
#define ICR 0b00000000000000010000000000000000

// ROM 3
#define SF  0b00000000000000001000000000000000    // Set flags
#define JZ  0b00000000000000000100000000000000    // Jump: Zero flag
#define JC  0b00000000000000000010000000000000    // Jump: Carry flag
#define JN  0b00000000000000000001000000000000    // Jump: Negative flag
#define JO  0b00000000000000000000100000000000    // Jump: Overflow flag
#define OB  0b00000000000000000000010000000000    // OUT BCD
#define NA1 0b00000000000000000000001000000000    // 
#define NA2 0b00000000000000000000000100000000    // 

// ROM 4
#define NA3 0b00000000000000000000000010000000    // 
#define NA4 0b00000000000000000000000001000000    // 
#define NA5 0b00000000000000000000000000100000    // 
#define NA6 0b00000000000000000000000000010000    // 
#define NA7 0b00000000000000000000000000001000    // 
#define RCS 0b00000000000000000000000000000100    // RTC: Chis Select
#define ROE 0b00000000000000000000000000000010    // RTC: Output Enable
#define RRW 0b00000000000000000000000000000001    // RTC: Read/Write


uint32_t data[] = {
// T1      T2         T3          T4        T5        T6           T7     T8        T9          T10  T11 <--> T16      CMD   BIN     HEX 
   MI|CO,  RO|II|CE,  ICR,        0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   // NOP   00000   0x00
   MI|CO,  RO|II|CE,  CO|MI,      RO|MI|CE, RO|AI,    ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // LDA   00010   0x10
   MI|CO,  RO|II|CE,  CO|MI,      RO|MI|CE, RO|BI,    AI|SF|EO,    ICR,   0,        0,          0,   0,0,0,0,0,0,   // ADD   00100   0x20
   MI|CO,  RO|II|CE,  CO|MI,      RO|MI|CE, RO|BI,    AI|SU|SF|EO, ICR,   0,        0,          0,   0,0,0,0,0,0,   // SUB   00110   0x30
   MI|CO,  RO|II|CE,  CO|MI,      RO|MI|CE, AO|RI,    ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // STA   01000   0x40
   MI|CO,  RO|II|CE,  CO|MI,      RO|AI|CE, ICR,      0,           0,     0,        0,          0,   0,0,0,0,0,0,   // LDI   01010   0x05
   MI|CO,  RO|II|CE,  CO|MI,      RO|J,     ICR,      0,           0,     0,        0,          0,   0,0,0,0,0,0,   // JMP   01100   0x60
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       01110   0x70
   MI|CO,  RO|II|CE,  IO|RRW|RCS, CO|MI,    RO|MI|CE, RCS|ROE|RI,  CO|MI, RO|MI|CE, RCS|ROE|RI, ICR, 0,0,0,0,0,0,   // RTR   10000   0x80
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   // RTW   10010   0x12
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       10100   0x14
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       10110   0x16
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       11000   0x18
   MI|CO,  RO|II|CE,  IO|OI,      0,        AO|OI,    ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // OUT   11010   0x1A
   MI|CO,  RO|II|CE,  0,          0,        0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       11100   0c1C
   MI|CO,  RO|II|CE,  HLT,        ICR,      0,        0,           0,     0,        0,          0,   0,0,0,0,0,0,   // HLT   11110   0xf0
   MI|CO,  RO|II|CE,  CO|MI,      RO|JZ,    CE,       ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // BEQ   00001   0x08
   MI|CO,  RO|II|CE,  CO|MI,      RO|MI|CE, RO|BI,    SU|SF|EO,    ICR,   0,        0,          0,   0,0,0,0,0,0,   // CMP   00011   0x18
   MI|CO,  RO|II|CE,  CO|MI,      RO|JC,    CE,       ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // BCS   00101   0x28
   MI|CO,  RO|II|CE,  CO|MI,      RO|JN,    CE,       ICR,         0,     0,        0,          0,   0,0,0,0,0,0,   // BMI   00111   0x38
   MI|CO,  RO|II|CE,  0,          0,         0,       0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       01001   0x48
   MI|CO,  RO|II|CE,  0,          0,         0,       0,           0,     0,        0,          0,   0,0,0,0,0,0,   //       01011
  
  /*
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 01101 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 01111 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 10001 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 10011 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 10111 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 11001 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 11011 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 11101 - 
   MI|CO,  RO|II|CE,  0,    0,          0,        0,      0,     0,  0,0,0,0,0,0,0,0,   // 11111 - 
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
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
 /*
void printContents() {
  for (int base = 0; base <= 768; base += 16) {
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
*/

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
  //for (int address = 0; address <= 2047; address += 1) {
  for (int address = 0; address <= 512; address += 1) {
    writeEEPROM(address, 0xff);

    if (address % 64 == 0) {
      Serial.print(".");
    }
  }
  Serial.println(" done");
*/

  

  // Program data bytes
  Serial.println("Programming");

  for (int address = 0; address < sizeof(data)/sizeof(data[0]); address += 1) {    
    writeEEPROM(address,        data[address] >> 24 );
    writeEEPROM(address +  512, data[address] >> 16 );
    writeEEPROM(address + 1024, data[address] >> 8 );
    writeEEPROM(address + 1536, data[address] >> 0 );

    if (address % 64 == 0) Serial.print(".");
  }

  Serial.println(" ");
  Serial.println("Done!");


  // Read and print out the contents of the EERPROM
  //Serial.println("Reading EEPROM");
  //printContents();
}


void loop() {
  // put your main code here, to run repeatedly:

}

