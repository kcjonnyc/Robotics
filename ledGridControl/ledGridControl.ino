// ledGridControl
// This code controls a grid of 12 x 10 Leds connected to
// the pins on an arduino mega board. 1 Led is on and the Led
// can then be moved across the grid based on 4 button inputs.

const int buttonPin1 = 40; // left
const int buttonPin2 = 41; // right
const int buttonPin3 = 42; // down
const int buttonPin4 = 43; // up

int ledPin[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13};
int transPin[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31};

int buttonState1 = 0;
int buttonState2 = 0;
int buttonState3 = 0;
int buttonState4 = 0;
int led = 0;
int trans = 0;

void setup() {
  Serial.begin(9600);
  // initialize the LED pin as an output:
  for (int c = 0; c <= 11; c++) {
      pinMode(ledPin[c], OUTPUT);
  }
  // initialize the transistor pin as an output:
  for (int c = 0; c <= 9; c++) {
      pinMode(transPin[c], OUTPUT);
  }
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin1, INPUT); 
  pinMode(buttonPin2, INPUT);  
  pinMode(buttonPin3, INPUT); 
  pinMode(buttonPin4, INPUT);
}

// update function to turn on proper led
void update() {
  // turn all leds off and turn specified led on
  for (int c = 0; c <= 11; c++) {
      digitalWrite(ledPin[c], LOW);
  }
  digitalWrite(ledPin[led], HIGH);
  for (int c = 0; c <= 9; c++) {
      digitalWrite(transPin[c], LOW);
  }
  digitalWrite(transPin[trans], HIGH);
  digitalWrite(ledPin[5], HIGH);
  digitalWrite(transPin[3], HIGH);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState1 = digitalRead(buttonPin1);
  delay(10);
  buttonState2 = digitalRead(buttonPin2);
  delay(10);
  buttonState3 = digitalRead(buttonPin3);
  delay(10);
  buttonState4 = digitalRead(buttonPin4);
  delay(10);
  
  if (buttonState1 == HIGH) {
      // need to move through leds left
      led = led - 1;
      if (led <= 0) {
        led = 0;
      }
      update();
      delay(150);
  }
  if (buttonState2 == HIGH) {
    // need to move through leds right
    led = led + 1;
      if (led >= 11) {
        led = 11;
      }
      update();
      delay(150);
  } 
  if (buttonState3 == HIGH) {
      // need to move through leds down
      trans = trans - 1;
      if (trans <= 0) {
        trans = 0;
      }
      update();
      delay(150);
  } 
  if (buttonState4 == HIGH) {
    // need to move through leds up
    trans = trans + 1;
      if (trans >= 9) {
        trans = 9;
      }
      update();
      delay(150);
  }
  Serial.println(led);
}
