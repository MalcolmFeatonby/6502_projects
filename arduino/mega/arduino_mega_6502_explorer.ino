// 6502 Explorer / diag Arduino code for a 6502 
// 
// Target platform: Arduino Mega

const char ADDR_LINE[] = {
    22, // A0  - 9
    24, // A1  - 10
    26, // A2  - 11
    28, // A3  - 12
    30, // A4  - 13
    32, // A5  - 14
    34, // A6  - 15
    36, // A7  - 16
    38, // A8  - 17
    40, // A9  - 18
    42, // A10 - 19
    44, // A11 - 20
    46, // A12 - 22
    48, // A13 - 23
    50, // A14 - 24
    52};// A15 - 25

const char DATA_LINE[] = {
    39, // D0 - 33
    41, // D1 - 32
    43, // D2 - 31
    45, // D3 - 30
    47, // D4 - 29
    49, // D5 - 28
    51, // D6 - 27
    53};// D7 - 26


#define CLOCK 2
#define RW_FLAG 4

void setup() {
    Serial.begin(115200);

    Serial.println("---> INITIALIZING....");
      
    pinMode(RW_FLAG, INPUT);
    for (int i = 0; i < 16; i++) {
      pinMode(ADDR_LINE[i], INPUT);
    }
    
    for (int i = 0; i < 8; i++) {
      pinMode(DATA_LINE[i], INPUT);
    }

    pinMode(CLOCK, INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), showBusOnClockTick, RISING);

}

void showBusOnClockTick() {

    Serial.print("Addr [");

    unsigned int addressLine = 0;
    for (int i = 15; i >= 0 ; i--) {
      int addrBit = digitalRead(ADDR_LINE[i]) ? 1 : 0;
      addressLine = (addressLine << 1) + addrBit;
      Serial.print(addrBit);
    };

    char addrHex[6];
    
    sprintf(addrHex, "(%04x)", addressLine);
    
    Serial.print("]");
    Serial.print(addrHex);
    Serial.print(" Data [");

    unsigned int dataLine = 0;
    for (int i = 7; i >= 0; i--) { // Read in reverse
      int dataBit = digitalRead(DATA_LINE[i]) ? 1 : 0;
      dataLine = (dataLine << 1) + dataBit;
      Serial.print(dataBit);
    };

    char dataHex[6];
    sprintf(dataHex,"(%02x)", dataLine);

    Serial.print("] ");
    Serial.print(dataHex);
    Serial.print(" (");
    Serial.print(digitalRead(RW_FLAG) ? "R" : "W");
    Serial.println(")");

}

void loop() {
}
