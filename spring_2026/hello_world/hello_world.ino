
//In oreder to get serial output, Go to Tools > USB CDC on boot: "Enabled"
//Also just press Serial monitor so you actually have the window
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello World!");
}

void loop() {
  // put your main code here, to run repeatedly:
}
