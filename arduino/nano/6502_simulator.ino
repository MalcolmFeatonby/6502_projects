// Simulator that models a 6502 input to a 6522 

const char RS0_3[] = {A8, A9, A10, A11};

const char CS1_2B[] = {3, 4};

const char OUTPUT_B[] = {5, 6, 7, 8, 9, 10, 11, 12};

const char DATA_LINE[] = {39, 41, 43, 45, 47, 49, 51, 53};

const char STATE[] = {0, 0, 1, 1, 2, 2, 3, 3, 4, 4};

#define CLOCK  2
#define RESET_DETECT  22
#define RW_FLAG 24

#define TICK 1;
#define TOCK 0;

int clockTick = TICK;

int current_state;

void setup() {
    current_state = 0;

    pinMode(RW_FLAG, OUTPUT);
    
    for (int i = 0; i < sizeof(OUTPUT_B) ; i++) {
      pinMode(OUTPUT_B[i], INPUT);
    }

    for (int i = 0; i < sizeof(RS0_3) ; i++) {
      pinMode(RS0_3[i], OUTPUT);
    }

    for (int i = sizeof(DATA_LINE)- 1; i >= 0 ; i--) {
      pinMode(DATA_LINE[i], OUTPUT);
    }

    // Set the CS1 and CS2B pins for input
    pinMode(CS1_2B[0], OUTPUT);
    pinMode(CS1_2B[1], OUTPUT);

    pinMode(CLOCK, INPUT);
    attachInterrupt(digitalPinToInterrupt(CLOCK), onClockTick, RISING);

    Serial.begin(57600);
}


void resetState() {
  Serial.print("RESET ....");  
  digitalWrite(RESET_DETECT, HIGH);
}

// We change state every second tick (on the TOCK not the TICK);
void onClockTick() {
  if (clockTick) {
    Serial.print("TICK [");
    clockTick = TOCK;    
  } else {
    Serial.print("TOCK [");
    steadyState();
    Serial.print("Current state [");
    Serial.print(current_state);
    Serial.print("] ");
    switch (current_state) {
        case 0:
           resetState();
           current_state++;
           break;
        case 1:
          setRegisterToWriteLatchPortB();
           current_state++;
          break;
        case 2:
          writeToPortB(0x55);
           current_state++;
          break;
        case 3:
          writeToPortB(0xAA);
           current_state++;
          break;
        case 4:
          reportOnState();
           current_state++;
          break;
        case 5:
          Serial.print("Loop!!!");
          current_state = 0;
          break;
     }
     clockTick = TICK;
  }
  reportOnState();
  Serial.println("]");
}

void steadyState() {
  //Ensure reset bit is not set
  digitalWrite(RESET_DETECT, LOW);
  //Enable for write
  digitalWrite(CS1_2B[0], HIGH); //CS1 - high
  digitalWrite(CS1_2B[1], LOW);  //CS2B - low
  //RW_FLAG to read
  digitalWrite(RW_FLAG, LOW);
}

void setRegisterToWriteLatchPortB() {
  digitalWrite(RS0_3[0], LOW);
  digitalWrite(RS0_3[1], HIGH);
  digitalWrite(RS0_3[2], LOW);
  digitalWrite(RS0_3[3], LOW);

  digitalWrite(RW_FLAG, HIGH);

  for (int i = 0; i < sizeof(DATA_LINE); i++) {
    digitalWrite(DATA_LINE[i], HIGH);
  }
  Serial.print("Write 0010 to RS and 0xFF to Data line");
}

void writeToPortB(int valueToWrite) {

  digitalWrite(RW_FLAG, HIGH);

  int val = valueToWrite;

  for (int i = 0; i < sizeof(DATA_LINE); i++) {
    digitalWrite(DATA_LINE[i], (val & 1) > 0 ? HIGH : LOW );
    val = val >> 1;
  }
  Serial.print("Write [");
  Serial.print(valueToWrite);
  Serial.print("] to Data Bus");
  
}


void reportOnState() {
  
    Serial.print(" Data [");

    unsigned int dataLine = 0;
    for (int i = sizeof(DATA_LINE)- 1; i >= 0 ; i--) {
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
    Serial.print(") Buffer B [");

    for (int i = 0; i < sizeof(OUTPUT_B) ; i++) {
      int dataBit = digitalRead(OUTPUT_B[i]) ? 1 : 0;
      Serial.print(dataBit);
    };
    Serial.print("] RS0 - RS3 [");

    for (int i = sizeof(RS0_3) - 1; i >= 0 ; i--) {
      int dataBit = digitalRead(RS0_3[i]) ? 1 : 0;
      Serial.print(dataBit);
    };
    Serial.print("] (CS1=>");
    Serial.print(digitalRead(CS1_2B[0]) ? 1 : 0);
    Serial.print(") (CS2B=>");
    Serial.print(digitalRead(CS1_2B[1]) ? 1 : 0);

    Serial.print(")");
}

void loop() {
}
