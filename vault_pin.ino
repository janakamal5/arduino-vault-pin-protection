#include <Wire.h>//To able communicate with the Arduino
#include <LiquidCrystal_I2C.h>//To communnicate with the LCD

LiquidCrystal_I2C lcd(0x27, 16, 2);//LCD address that we are using

const int BTN_PIN[4] = {2, 3, 4, 5};//Button Pins to able to enter 4-digit PIN code
const int BTN_ENTER = 6;//Enter Pin
const int LED_GREEN = 12;//Green LED Pin
const int LED_RED   = 11;//Red LED Pin
const int BUZZER    = 10;//Buzzer pin

const int SECRET_PIN[4] = {7, 1, 5, 9};//My Last digit of my ID as the PIN code :)

int           pinDigits[4]         = {0, 0, 0, 0};//and the beginning of the PIN code will start like that
int           wrongAttempts         = 0;//counter number of wrong PIN entered
unsigned long lastIncrementTime[4] = {0, 0, 0, 0};// increament by 1 while pressing the button for each
const unsigned long HOLD_DELAY      = 400;// when I still pressing the buttons increases by 1 for every 400 msec
bool lastButtonState[4] = {HIGH, HIGH, HIGH, HIGH};//High == not pressed LOW == pressed
bool enterCurrent        = HIGH;
bool enterPrevious       = HIGH;


void playMarioDeathTune() {//Buzzer sound 
  int notes[][2] = {//the notes as we just can copy the music and paste it from Youtube we try to make our own music to create the perfect pitch
    {523, 350},//{Frequency,Duration}
    {0,0},
    {0,150},
    {0,0},
    {392, 350},
    {0,   150},
    {330, 500},
    {0,   200},
    
    
    
  };
  int len = sizeof(notes) / sizeof(notes[0]);

  for (int i = 0; i < len; i++) {// It loops through for every note 
    if (notes[i][0] == 0) {
      noTone(BUZZER);
    } else {
      tone(BUZZER, notes[i][0]);//Generates the sound based the frequency provided
    }
    delay(notes[i][1]);//Plays the note based on the duration 
  }
  noTone(BUZZER);//Stops the buzzer
}

void setup() {//Begins when the arduino intiated 
  Wire.begin();//Starts communicating with I2C 
  lcd.init();//The LCD start
  lcd.backlight();// turns the blacklight 

  for (int i = 0; i < 4; i++) {
    pinMode(BTN_PIN[i], INPUT_PULLUP);// the buttons as inputs 
  }
  pinMode(BTN_ENTER, INPUT_PULLUP);//Enter button as input
  pinMode(LED_GREEN, OUTPUT);//Green LED as output
  pinMode(LED_RED,   OUTPUT);//Red LED as output
  pinMode(BUZZER,    OUTPUT);//Buzzer as output 

  digitalWrite(LED_GREEN, LOW);//To make the Green LED off when the Arduino starts
  digitalWrite(LED_RED,   LOW);//To make the Red LED off when the Arduino starts
  noTone(BUZZER);//To make the Buzzer off when the Arduino starts

  lcd.clear();//To intiate the LCD 
  lcd.setCursor(0, 0);//move the Cursor to specific position (Colomn,Row)
  lcd.print("Vault System");//The welcome Message
  lcd.setCursor(0, 1);//under the welcome message 
  lcd.print("Enter PIN");//Will show this too

  delay(1500);//opens after 1.5 sec
  showPIN();//Show PIN means under "Enter PIN" 0000 on the LCD
}

void loop() {//
  unsigned long currentTime = millis();//returns the number on the Arduino for every milliseconds

  for (int i = 0; i < 4; i++) {//Prints all 4 digits 
    bool currentState = digitalRead(BTN_PIN[i]);// Read the current state of the button (HIGH = released, LOW = pressed)

    if (currentState == LOW && lastButtonState[i] == HIGH) {//Sees if a new button press
      pinDigits[i]++;//Increase the PIN digits by 1 
      if (pinDigits[i] > 9) pinDigits[i] = 0;//if a digit == 9 then the next press is 0
      showPIN();//Shows in the LCD screen
      lastIncrementTime[i] = currentTime;//Stores the time when the last digit was increased
      delay(50);//Small debounce delay to stop any unwanted presses
    } else if (currentState == LOW && lastButtonState[i] == LOW) {//Checks if the button is still being held down
      if (currentTime - lastIncrementTime[i] >= HOLD_DELAY) {//Holding the button for every 400ms
        pinDigits[i]++;//increaments the digits 
        if (pinDigits[i] > 9) pinDigits[i] = 0;
        showPIN();
        lastIncrementTime[i] = currentTime;//Rest the timer for the next auto-increment
      }
    }
    lastButtonState[i] = currentState;//Save the current button state for the next loop
  }

  enterCurrent = digitalRead(BTN_ENTER);//Reads the Enter button state 
  if (enterCurrent == LOW && enterPrevious == HIGH) {// to dectect the new press
    checkPIN(); //to check the entered PIN matches the secret PIN 
  }
  enterPrevious = enterCurrent;// saves the Enter button state for the next loop 
}

void showPIN() {
  lcd.clear();
  lcd.setCursor(0, 0);//The first sentence 
  lcd.print("Enter PIN:");//this one 
  lcd.setCursor(4, 1);// the 4th row is my PIN 
  for (int i = 0; i < 4; i++) {
    lcd.print(pinDigits[i]);// when I reach the 4th digit
    lcd.print(" ");
  }
}

void checkPIN() {
  bool correct = true;
  for (int i = 0; i < 4; i++) {
    if (pinDigits[i] != SECRET_PIN[i]) correct = false;// the input pin not equal to 
  }

  if (correct) {// if the PIN is correct 
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("PIN Accepted");// Return this 
    lcd.setCursor(0, 1); lcd.print("Vault Opened");//after it this 
    digitalWrite(LED_GREEN, HIGH);// then the green LED light up
    digitalWrite(LED_RED,   LOW);// and Red LED closes
    noTone(BUZZER);// No buzzer sound
    while (true);// if the PIN is correct
  }

  wrongAttempts++;// Increase by 1 if my my PIN is inncorrect

  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("Wrong PIN");//if my PIN is inncorrect then output this at first
  lcd.setCursor(0, 1); lcd.print("Try Again");// after it this

  for (int i = 0; i < 3; i++) {// if less than 3 trials 
    digitalWrite(LED_RED, HIGH); delay(250);//the red LED opens
    digitalWrite(LED_RED, LOW);  delay(250);//and closes as if it is blinking 
  }

  if (wrongAttempts >= 3) {// more than or equal 3 trials 
    lcd.clear();
    lcd.setCursor(0, 0); lcd.print("!!! ALARM !!!");//outputs as my first sentence 
    lcd.setCursor(0, 1); lcd.print("System Locked");// then this after it 
    digitalWrite(LED_RED, HIGH);// the Red LED keeps lighten up
    playMarioDeathTune();// and buzzer sound begins
    while (true);// if the condition still is truee
  }

  for (int i = 0; i < 4; i++) pinDigits[i] = 0;//Rest all PINs to 0 
  delay(1000);// wait for 1000 msec
  showPIN();//output on the screen
}
