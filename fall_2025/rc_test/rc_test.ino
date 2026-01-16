#define CH1 5
#define CH2 6
#define CH3 7

void setup() {
  Serial.begin(115200);
  pinMode(CH1, INPUT); //ch1
  pinMode(CH2, INPUT); //ch2
  pinMode(CH3, INPUT); //ch3
}

int readDrive(int ch) {
  unsigned long reading = pulseIn(ch,HIGH,40000); // rc pulse sent every 20 ms ig?
  return constrain(map(reading, 1000, 2000, 100, -100),-100,100); //scales reading to throttle percent
}
int readThrottle(int ch) {
  unsigned long reading = pulseIn(ch,HIGH,40000); // rc pulse sent every 20 ms ig?
  return constrain(map(reading, 1000, 2000, 0, 100),0,100); //scales reading to throttle percent
}

void loop() {
  Serial.println( String(readDrive(CH1)) + " " + String(readDrive(CH2)) + " " + String(readThrottle(CH3)) );
  //Serial.println( String(readChannel(CH1)));
  delay(1000);
}





// #define CH1 7
// #define CH2 1
// #define CH3 0

// // Read the number of a given channel and convert to the range provided.
// // If the channel is off, return the default value
// int readChannel(int channelInput, int minLimit, int maxLimit, int defaultValue){
//   int ch = pulseIn(channelInput, HIGH, 30000);
//   if (ch < 100) return defaultValue;
//   return map(ch, 1000, 2000, minLimit, maxLimit);
// }

// // Red the channel and return a boolean value
// bool redSwitch(byte channelInput, bool defaultValue){
//   int intDefaultValue = (defaultValue)? 100: 0;
//   int ch = readChannel(channelInput, 0, 100, intDefaultValue);
//   return (ch > 50);
// }

// void setup(){
//   Serial.begin(115200);
//   pinMode(CH1, INPUT);
//   pinMode(CH2, INPUT);
//   pinMode(CH3, INPUT);
//   }

// int ch1Value, ch2Value, ch3Value;

// void loop() {
//   ch1Value = readChannel(CH1, -100, 100, 0);
//   ch2Value = readChannel(CH2, -100, 100, 0);
//   ch3Value = readChannel(CH3, -100, 100, -100);
  
//   Serial.print("Ch1:");
//   Serial.println(ch1Value, "\n");
//   Serial.print(" Ch2:");
//   Serial.println(ch2Value,"\n");
//   Serial.print(" Ch3:");
//   Serial.println(ch3Value,"\n");
//   delay(500);
// }