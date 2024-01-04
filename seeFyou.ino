#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// Arduino pins
const int output_photodiode = A0;
const int button_input_blank = A2;
const int button_input_read = A3;
int blankReading = analogRead(output_photodiode);
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16charsand2line display

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  //analogReference(EXTERNAL);
  pinMode(button_input_blank, INPUT_PULLUP);
  pinMode(button_input_read, INPUT_PULLUP);

  // Set welcome screen
  lcd.init(); // initialize the lcd
  lcd.backlight(); // open the backlight
  lcd.setCursor(2, 0); 
  lcd.print("SemiQuant.com"); // Print a message to the LCD
  lcd.setCursor(2, 1); // set the cursor to column 2, line 1
  lcd.print("count4you"); // Print a message to the LCD.

  delay(2000);

  // Show Blank message
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blank reading:");
  lcd.setCursor(0, 1);
  lcd.print("Click Blank");
  
  // Wait for button_input_blank to be pressed
  while (digitalRead(button_input_blank) == LOW) {
    // Wait for the button to be clicked
  }

  // Get and display Blank reading
  Serial.println(blankReading);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blank reading: ");
  lcd.setCursor(0, 1);
  lcd.print("Blank read");
  delay(1000);

  // Ask for Read
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OD reading: ");
  lcd.setCursor(0, 1);
  lcd.print("Click Read");
}

void loop() {
  int buttonState = 0;
  int buttonState2 = 0;
  buttonState = digitalRead(button_input_blank);
  buttonState2 = digitalRead(button_input_read);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == LOW) {
    blankReading = 0;
    int reads = 0;
    lcd.clear();
    lcd.print("Blank raw:");

    while (reads < 10) {
      blankReading += analogRead(output_photodiode);
      reads++;
      delay(200);
    }

    blankReading /= reads;
    lcd.setCursor(0, 1);
    lcd.print(blankReading);
  };

  if (buttonState2 == LOW) {
    int value = 0;
    int reads = 0;
    lcd.clear();
    lcd.print("OD reading:");

    while (reads < 10) {
      value += analogRead(output_photodiode);
      reads++;
      delay(200);
    }

    float od = -1.0 * log10(float(value) / float(blankReading));
    lcd.print("OD reading:");
    lcd.setCursor(0, 1);
    lcd.print(od);
  };

  delay(500);
}

