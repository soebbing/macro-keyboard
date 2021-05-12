/*******************************************************************
   A multi-mode Macro keyboard with Arduino Pro Micro using row column matrix-ing.
   This is example sketch to understand key layout, assignments, and command protocols.
   This example is slightly more robust than the larger size MacroKeyboard (https://www.youtube.com/watch?v=acJ6gufBN_A.
   Each mode (profile/application) has a unique definition for each encoder, including mouse control and a display LCD
   to descibes the modes or key assignements.
   Some mouse movement get their value from analogRead(pin 0) so make sure something is connected.
   Don't forget to solder all the diode and encoder solder jumpers on the backside!!
  (c) 2020 Ryan Bates
  Last time I touched this: Oct 23 2020
  web: www.retrobuiltgames.com
  youtube: https://www.youtube.com/c/ryanbatesrbg
  twitter: @retrobuiltgames

  Ryan's Pro Tips:
  =============== Keyboard Control================================================================================
  Keyboard.write();   Sends a keystroke to a connected computer.
                    This is similar to pressing and releasing a key on your keyboard.
                    Will send a shift command if applicable. Example: Keyboard.write('K') will
                    automatically do SHIFT + k.
                    Can also accept ASCII code like this:
                    //Keyboard.write(32); // This is space bar (in decimal)
                    Helpful list of ASCII + decimal keys http://www.asciitable.com/


  Keyboard.press();   Best for holding down a key with multi-key commands; like copy/ paste
                    This example is [ctrl] + [shift] + [e]
                      //Keyboard.press(KEY_LEFT_CTRL);
                      //Keyboard.press(KEY_LEFT_SHIFT);
                      //Keyboard.press('e');
                      //delay(100); Keyboard.releaseAll();


  Keyboard.print();   Sends a keystroke(s)
                    Keyboard.print("stop using aim bot"); // types this in as a char or int! (not a string)!


  Keyboard.println(); Sends a keystroke followed by a newline (carriage return)
                     Very practical if you want to type a password and login in one button press!

  SOMETIMES, applications are coded to recognize Keyboard.press() and not Keyboard.write() and vice versa.
  You might have to experiment.

  =============== Mouse Control================================================================================
  Mouse.move(x, y, wheel);  Moves the mouse and or scroll wheel up/down left/right.
                          Range is -128 to +127. units are pixels
                          -number = left or down
                          +number = right or up

  Mouse.press(b);         Presses the mouse button (still need to call release). Replace "b" with:
                        MOUSE_LEFT   //Left Mouse button
                        MOUSE_RIGHT  //Right Mouse button
                        MOUSE_MIDDLE //Middle mouse button
                        MOUSE_ALL    //All three mouse buttons

  Mouse.release(b);       Releases the mouse button.

  Mouse.click(b);         A quick press and release.

*******************************************************************/
// ----------------------------
// Standard Libraries
// ----------------------------
#include <Wire.h>
#include "Keyboard.h"
// Library with a lot of the HID definitions and methods
// Can be useful to take a look at it see whats available
// https://github.com/arduino-libraries/Keyboard/blob/master/src/Keyboard.h


// Variables that will change:
int modePushCounter = 0;     // counter for the number of button presses
int buttonState = 0;         // current state of the button
int lastButtonState = 0;     // previous state of the button

String password;             // string for rando password generator, its a global variable because i might do something fancy with it?

const int ModeButton = A0;    // the pin that the Modebutton is attached to
const int pot = A1;           // pot for adjusting attract mode demoTime or mouseMouse pixel value
const int Mode1 = A2;
const int Mode2 = A3;         //Mode status LEDs

#include <Keypad.h>

//Define the rols column matrix roots
const byte ROWS = 2; //two rows
const byte COLS = 4; //four columns

char keys[ROWS][COLS] = {
  {'1', '2', '3', '4'},  //  the keyboard hardware is a 2x4 grid...
  {'5', '6', '7', '8'},

};

byte rowPins[ROWS] = {4, 5}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {6, 7, 8, 9 }; //connect to the column pinouts of the keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  randomSeed(analogRead(0));           // is this a bad idea? it's called before assigning the button as pull up... the switch is open at rest..
  pinMode(ModeButton, INPUT_PULLUP);  // initialize the button pin as a input:
  pinMode(Mode1, OUTPUT); digitalWrite(Mode1, LOW);
  pinMode(Mode2, OUTPUT); digitalWrite(Mode2, LOW);

  Keyboard.begin();
}

void loop() {
  char key = keypad.getKey();
  checkModeButton();

  switch (modePushCounter) { // switch between keyboard configurations:
    case 0:    //  Application Alpha or MODE 0 ========================================================================
      digitalWrite(Mode1, LOW);
      digitalWrite(Mode2, LOW); //indicate what mode is loaded

      if (key) {
        Serial.println(key);
        switch (key) {
          case '1': // Open kitty terminal
            Keyboard.press(KEY_LEFT_GUI);
            delay(50);
            Keyboard.print("/");
            Keyboard.release(KEY_LEFT_GUI);
            
            delay(100);
            Keyboard.print("kitty");
            delay(50);
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print("1");
            break;

          case '2': // Mute/Unmute in Meet
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('d');
            break;

          case '3':
            break;

          case '4': // Move window workspace up
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.print(KEY_PAGE_UP);
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release(KEY_LEFT_SHIFT);
            break;

          case '5': // Lock screen
            Keyboard.press(KEY_LEFT_GUI);
            delay(20);
            Keyboard.release(KEY_LEFT_GUI);
            delay(150);
            Keyboard.println("lock");
            break;

          case '6': // Copy
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('c');
            break;

          case '7': // Paste
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('v');                         // paste
            break;

          case '8': // Move window one workspace down
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.press(KEY_PAGE_DOWN);
            Keyboard.release(KEY_PAGE_DOWN);
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release(KEY_LEFT_SHIFT);
            break;
        }
        delay(50);
        Keyboard.releaseAll(); // this releases the buttons
      }
      break;

    case 1:    //  Arduino shortcuts or MODE 1 ========================================================================
      digitalWrite(Mode1, HIGH);
      digitalWrite(Mode2, LOW);

      if (key) {
        Serial.println(key);
        switch (key) {
          case '1':
            Keyboard.write(32);                           //Space Bar
            break;

          case '2':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('r');                          //Verify
            break;

          case '3':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('u');                          //Upload
            break;

          case '4':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.press(KEY_LEFT_SHIFT);
            Keyboard.print('m');                          //Serial Monitor
            break;

          case '5':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('c');                          //Copy
            break;

          case '6':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('v');                          //Paste
            break;

          case '7':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('/');                          //Comment / Uncomment
            break;

          case '8':
            Keyboard.press(KEY_LEFT_CTRL);
            Keyboard.print('f');                          //Find
            break;
        }

        delay(50);
        Keyboard.releaseAll(); // this releases the buttons
      }
      break;

    case 2:    // Application Delta or MODE 2 ========================================================================

      digitalWrite(Mode1, LOW);
      digitalWrite(Mode2, HIGH);

      if (key) {
        switch (key) {
          case '1':
            Keyboard.println("Delta key1");
            break;
          
          case '2':
            Keyboard.println("Delta key2");
            break;

          case '3':
            Keyboard.println("Delta key3");
            break;

          case '4':
            Keyboard.println("Delta key4");
            break;

          case '5':
            Keyboard.print("admin_1");
            Keyboard.write(9); //tab key
            Keyboard.println("password_1");
            break;

          case '6':
            Keyboard.print("admin_2");
            Keyboard.write(9); //tab key
            Keyboard.println("password_2");
            break;

          case '7':
            Keyboard.print("admin_3");
            Keyboard.write(9); //tab key
            Keyboard.println("password_3");
            break;

          case '8':
            Keyboard.print("admin_4");
            Keyboard.write(9); //tab key
            Keyboard.println("password_4");
            break;

        }
        delay(50);
        Keyboard.releaseAll(); // this releases the buttons
      }
      break;

    case 3:    // Application Delta or MODE 3 ========================================================================
      digitalWrite(Mode1, HIGH);
      digitalWrite(Mode2, HIGH);

      if (key) {
        //Serial.println(key);
        switch (key) {
          case '1':
            Keyboard.println("Hi.   Hopefully this example code makes sense.");
            break;

          case '2': //macro that opens chrome and a random wiki page for learning.
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release('r');
            delay(50); //give your system time to catch up with these android-speed keyboard presses
            Keyboard.println("chrome"); delay(500);
            Keyboard.println("https://en.wikipedia.org/wiki/Special:Random");
            break;

          case '3': Keyboard.press(KEY_LEFT_GUI); delay(50);
            Keyboard.release(KEY_LEFT_GUI); delay(400);
            Keyboard.println("mag"); delay(200);
            break;

          case '4': Keyboard.write(32); break; //smashing that space bar
          case '5': //macro that opens Chrome & Rick Rolls you like a chump
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release('r');
            delay(50); //give your system time to catch up with these android-speed keyboard presses
            Keyboard.println("chrome");
            delay(500);
            Keyboard.println("https://www.youtube.com/watch?v=dQw4w9WgXcQ");
            break;

          case '6':  //macro that opens Chrome and goes to my youtube channel!
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release('r');
            delay(50); //give your system time to catch up with these android-speed keyboard presses
            Keyboard.println("chrome");
            delay(500);
            Keyboard.println("https://www.youtube.com/c/ryanbatesrbg");
            break;

          case '7': //macro example!!! Windows_Key+R = Run then type "mspaint" and press enter. Opens MS Paint
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release('r');
            delay(50); //give your system time to catch up with these android-speed keyboard presses
            Keyboard.println("mspaint");
            break;

          case '8': //macro example!!! Windows_Key+R = Run then type "calc" and press enter. Opens MS Calculator
            Keyboard.press(KEY_LEFT_GUI);
            Keyboard.press('r');
            Keyboard.release(KEY_LEFT_GUI);
            Keyboard.release('r');
            delay(50); //give your system time to catch up with these android-speed keyboard presses
            Keyboard.println("calc");
            break;
        }

        delay(50);
        Keyboard.releaseAll(); // this releases the buttons
      }
  }
}

void checkModeButton() {
  buttonState = digitalRead(ModeButton);
  if (buttonState != lastButtonState) { // compare the buttonState to its previous state
    if (buttonState == LOW) { // if the state has changed, increment the counter
      // if the current state is LOW then the button cycled:
      modePushCounter++;
    }
    delay(50); // Delay a little bit to avoid bouncing
  }

  lastButtonState = buttonState; // save the current state as the last state, for next time through the loop
  if (modePushCounter > 3) { //reset counter after 4 presses (remember we start counting at 0) change this number to add more modes
    modePushCounter = 0;
  }
}
