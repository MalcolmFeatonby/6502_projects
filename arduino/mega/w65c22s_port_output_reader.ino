// Reads the output (PORT A and PORT B) of a 6522 at the clock tick
// and displays to serial out. 
//
// Target platform: Arduino Mega

const char PORT_A_LINE[] = {22, 24, 26, 28, 30, 32, 34, 36};
const char PORT_B_LINE[] = {38, 40, 42, 44, 46, 48, 50, 52};

#define CLOCK  2
#define RESET_DETECT  3
#define RW_FLAG 31

void setup() {

    pinMode(RW_FLAG, INPUT);
    for (int i = 0; i < sizeof(PORT_A_LINE); i++) {
      pinMode(PORT_A_LINE[i], INPUT);
    }
    
    for (int i = 0; i < sizeof(PORT_B_LINE); i++) {
      pinMode(PORT_B_LINE[i], INPUT);
    }

    pinMode(CLOCK, INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), showBusOnClockTick, RISING);

    Serial.begin(57600);
}

void showBusOnClockTick() {

    Serial.print("Port A [");

    unsigned int addressLine = 0;
    for (int i = 0; i < sizeof(PORT_A_LINE); i++) {
      int addrBit = digitalRead(PORT_A_LINE[i]) ? 1 : 0;
      addressLine = (addressLine << 1) + addrBit;
      Serial.print(addrBit);
    };

    char addrHex[6];
    
    sprintf(addrHex, "(%02x)", addressLine);
    
    Serial.print("]");
    Serial.print(addrHex);
    Serial.print(" | Port B [");

    unsigned int dataLine = 0;
    for (int i = 0; i < sizeof(PORT_B_LINE); i++) {
      int dataBit = digitalRead(PORT_B_LINE[i]) ? 1 : 0;
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
