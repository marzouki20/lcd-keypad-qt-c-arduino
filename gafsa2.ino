#include <LiquidCrystal_I2C.h>
#include <Keypad.h>

// Initialize LCD
LiquidCrystal_I2C lcd(0x27, 16, 2);

// Define keypad configuration
const byte ROWS = 4;
const byte COLS = 4;
char keys[ROWS][COLS] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};
byte rowPins[ROWS] = {6, 7, 8, 9};
byte colPins[COLS] = {10, 11, 12, 13};

// Create keypad object
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

// Variables to store input data
String inputID = "";   // Buffer for the input ID
String inputPass = ""; // Buffer for the input password
int cursorY = 0;       // Variable for controlling LCD cursor Y position

void setup() {
  // Initialize LCD
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Saisir votre ID:");

  // Initialize Serial communication
  Serial.begin(115200); // Ensure this matches the baud rate in Qt
}

void loop() {
  // Check if data is available from Serial (e.g., received from Qt or other source)
  if (Serial.available() > 0) {
    String incomingChar = Serial.readString();  // Read incoming serial character

    // If the received data indicates success (true), display "Hello Admin"
    if (incomingChar == "true") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Hello Admin");
      delay(1000);  // Display for 2 seconds
    }
    // If password is incorrect, display "Password Incorrect" and stay in the password entry stage
    else if (incomingChar == "Pass") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Password Incorrect");
      delay(1000);  // Display for 2 seconds

      // Keep showing the password prompt without resetting the ID
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Saisir votre Pass:");
      cursorY = 1;
    }
    // If ID is incorrect, display "ID Incorrect"
    else if (incomingChar == "ID") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("ID Incorrect");
      delay(1000);  // Display for 2 seconds
    }
    // If there's any error, display "Error"
    else if (incomingChar == "Error") {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Error in Validation");
      delay(2000);  // Display for 2 seconds
    }
  }

  // Read a key from the keypad
  char key = keypad.getKey();

  // Check if any key is pressed
  if (key) {
    if (key == '#') {  // If # is pressed
      if (cursorY == 0) {  // If we are at the ID entry stage
        // Show prompt for password
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Saisir votre Pass:");
        cursorY = 1; // Move to password input phase
      } 
      else if (cursorY == 1) {  // If we are at the password entry stage
        // Combine ID and password and send over serial
        String combinedData = inputID + ":" + inputPass;
        Serial.println(combinedData);  // Send ID and password in the format "id:password"

        // Reset the input buffers and go back to ID entry stage
        inputPass = "";  // Don't clear the inputID
        cursorY = 0;

        delay(2000); // Wait for 2 seconds before showing the ID prompt again
      }
    } 
    else if (key == '*') {  // If * is pressed, clear inputs
      inputID = "";
      inputPass = "";
      lcd.setCursor(0, 1);
      lcd.print("                "); // Clear the display line
    } 
    else if (key == 'D') {  // If D is pressed, restart the process
      restartInput();  // Restart the input process
    }
    else {
      // Append the key to the input (ID or password)
      if (cursorY == 0) {
        inputID += key;  // Input ID
        lcd.setCursor(0, 1);
        lcd.print(inputID);  // Display the entered ID
      } 
      else if (cursorY == 1) {
        inputPass += key;  // Input password
        lcd.setCursor(0, 1);
        lcd.print(inputPass);  // Display the entered password
      }
    }
  }
}

// Function to restart the input process
void restartInput() {
  // Clear the screen and show the ID input prompt again
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Saisir votre ID:");
  
  // Reset buffers and cursor position
  inputID = "";
  inputPass = "";
  cursorY = 0;
}
