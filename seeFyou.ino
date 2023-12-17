#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <math.h>

// Arduino pins
const int output_photodiode = A0;
const int button_input_blank = A3;
const int button_input_read = A2;
LiquidCrystal_I2C lcd(0x27, 16, 2); // set the LCD address to 0x27 for a 16charsand2line display

void setup() {
  Serial.begin(9600);
  Serial.println("Starting");
  analogReference(EXTERNAL);
  pinMode(button_input_blank, INPUT_PULLUP);

  // Set welcome screen
  lcd.init(); // initialize the lcd
  lcd.backlight(); // open the backlight
  lcd.setCursor(2, 0); // set the cursor to column 3, line 0
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
  while (digitalRead(button_input_blank) == HIGH) {
    // Wait for the button to be clicked
  }
  // Get and display Blank reading
  int blankReading = analogRead(output_photodiode);
  Serial.println(blankReading);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Blank reading: ");
  lcd.setCursor(0, 1);
  lcd.print(blankReading);
  delay(2000);

  // Ask for Read
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("OD reading: ");
  lcd.setCursor(0, 1);
  lcd.print("Click Read");

  // Wait for button_input_read click
  waitForButtonClick(blankReading);
}

void loop() {
  // Empty loop, everything is handled in setup
}

void waitForButtonClick(int blankReading) {
  int ref_counts = 0; // Assuming ref_counts needs to be initialized here
  while (true) {
    // Wait for either button_input_read or button_input_blank to be clicked
    while (digitalRead(button_input_blank) == HIGH && digitalRead(button_input_read) == HIGH) {
      // Wait for a button to be clicked
    }

    if (digitalRead(button_input_blank) == LOW) {
      // Button_input_blank is pressed, perform actions for blank
      int reads = 0;
      int value = 0;

      while (reads < 10) {
          value += analogRead(output_photodiode);
          reads++;
          delay(200);
      }

      // Now 'value' contains the average of 10 analogReads
      value /= reads;

      float od = -1.0 * log10(float(value - blankReading) / float(ref_counts - blankReading));
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Blank reading:");
      lcd.setCursor(0, 1);
      lcd.print(value);
    } else if (digitalRead(button_input_read) == LOW) {
      // Button_input_read is pressed, perform actions for reading
      int value = analogRead(output_photodiode);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("OD reading:");
      lcd.setCursor(0, 1);
      lcd.print(value);
    }

    // Wait for the button to be released
    while (digitalRead(button_input_blank) == LOW || digitalRead(button_input_read) == LOW) {
      // Wait for the button to be released
    }

    delay(200); // Debounce delay
  }
}
