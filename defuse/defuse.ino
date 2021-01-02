/*
 * Defuse the Bomb!
 * 
 * "Cut" the correctly colored wires in sequence to stop the bomb from detonating!
 * 
 * Each wire is handled like a pushbutton & we have a simple state machine to watch
 * the progress.
 * 
 */
#include <ButtonDebounce.h>
#include <LiquidCrystal_I2C.h>

ButtonDebounce blue_wire(2, 250);
ButtonDebounce red_wire(3, 250);
ButtonDebounce green_wire(4, 250);
ButtonDebounce brown_wire(5, 250);
ButtonDebounce yellow_wire(6, 250);
ButtonDebounce orange_wire(7, 250);
ButtonDebounce purple_wire(8, 250);

LiquidCrystal_I2C lcd(0x27, 20, 4);   // SDA -> A4, SCL -> A5
const char* blank = "                    ";

void show_pulled_wire(char* color) {
  lcd.setCursor(1, 5);
  lcd.print(color);
  lcd.print(" was pulled!");
}

void blue_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Blue");
  }
}

void red_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Red");  }
}

void green_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Green");
  }
}

void brown_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Brown");
  }
}

void yellow_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Yellow");
  }
}

void orange_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Orange");
  }
}

void purple_pulled(const int state) {
  if (state == LOW) {
    show_pulled_wire("Purple");
  }
}


void setup() { 
  for (int p=2; p < 9; p++) {
    pinMode(p, INPUT);
  }
  blue_wire.setCallback(blue_pulled);
  red_wire.setCallback(red_pulled);
  green_wire.setCallback(green_pulled);
  brown_wire.setCallback(brown_pulled);
  yellow_wire.setCallback(yellow_pulled);
  orange_wire.setCallback(orange_pulled);
  purple_wire.setCallback(purple_pulled);      

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome To The Bomb!");
}

void loop() {
  blue_wire.update();
  red_wire.update();
  green_wire.update();
  brown_wire.update();
  yellow_wire.update();
  orange_wire.update();
  purple_wire.update();
}
