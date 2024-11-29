#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Initialize the LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define keypad size
const byte ROWS = 4; // 4 rows
const byte COLS = 4; // 4 columns

// Define the symbols on the buttons of the keypad
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

// Connect keypad ROW0, ROW1, ROW2, ROW3 to Arduino pins 6, 7, 8, 9
// Connect keypad COL0, COL1, COL2, COL3 to Arduino pins 10, 11, 12, 13
byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {10, 11, 12, 13};

// Create the Keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

void setup() {
  // Initialize the LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();

  // Display the welcome message
  lcd.setCursor(0, 0);
  lcd.print("Saisir votre ID:");
}

void loop() {
  char key = keypad.getKey();

  if (key) { // If a key is pressed
    static String inputID = ""; // Store the entered ID

    if (key == '#') { 
      // When '#' is pressed, consider it as "Enter"
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID Saisi: ");
      lcd.setCursor(0, 1);
      lcd.print(inputID); // Display the entered ID
      delay(3000);        // Pause to show the message
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saisir votre ID:");
      inputID = "";       // Clear the ID for the next input
    } else if (key == '*') { 
      // When '*' is pressed, clear the current input
      inputID = "";
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the second line
      lcd.setCursor(0, 1);
    } else {
      // Append the key to the input
      inputID += key;
      lcd.setCursor(0, 1);
      lcd.print(inputID); // Display the current input
    }
  }
}
