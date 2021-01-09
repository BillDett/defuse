/*
 * Defuse the Bomb!
 * 
 * "Cut" the specific colored wires in the correct sequence to stop the bomb from detonating!
 * 
 * Cutting a wire outside the sequence speeds up the clock.
 * 
 * If a wire within the sequence is cut out of order, the bomb will go off.
 * 
 * If each wire in the sequence is cut in the correct order, the bomb is defused.
 * 
 * If the time expires before the sequence is completed correctly, the bomb will go off.
 *
 *TODO:
 * Game logic to check the sequence
 * Timing logic & clock display
 * 
 */
#include <ButtonDebounce.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

// "Wire" Pins
#define BLUE    2
#define RED     3
#define GREEN   4
#define BROWN   5
#define YELLOW  6
#define ORANGE  7
#define PURPLE  8

ButtonDebounce blue_wire(BLUE, 250);
ButtonDebounce red_wire(RED, 250);
ButtonDebounce green_wire(GREEN, 250);
ButtonDebounce brown_wire(BROWN, 250);
ButtonDebounce yellow_wire(YELLOW, 250);
ButtonDebounce orange_wire(ORANGE, 250);
ButtonDebounce purple_wire(PURPLE, 250);

// LCD Stuff
LiquidCrystal_I2C lcd(0x27, 20, 4);   // SDA -> A4, SCL -> A5
const char* blank = "                 ";

Adafruit_7segment matrix = Adafruit_7segment();

// Timer
//const int duration = 3000;    // ms
int remaining = 10000;
int start_ms;

// Puzzle
const int sequence[] = { ORANGE, BLUE, YELLOW };    // This is the correct removal sequence to defuse the bomb
const int num_wires = 3;
int next_to_cut = 0;                              // Which wire in the sequence should be cut next?

void show_cut_wire(char* color) {
  lcd.setCursor(1, 5);
  lcd.print(blank);
  lcd.setCursor(1, 5);  
  lcd.print(color);
  lcd.print(" was cut!");
}

void blue_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Blue");
  }
}

void red_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Red");  }
}

void green_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Green");
  }
}

void brown_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Brown");
  }
}

void yellow_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Yellow");
  }
}

void orange_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Orange");
  }
}

void purple_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Purple");
  }
}

void check_time() {
  if (remaining > 0) {
    int elapsed = millis() - start_ms;
    remaining -= elapsed;
    //lcd.setCursor(2,2);
    //lcd.print(remaining);
    matrix.print(remaining/1000);
    matrix.writeDisplay();
    if ( remaining <= 0 ) {
      lcd.print("  BOOM!"); 
    }
  }
}


void setup() { 
  for (int p=2; p < 9; p++) {   // DEPENDS ON "WIRE" PINS BEING SEQUENTIALLY DEFINED ABOVE!
    pinMode(p, INPUT);
  }
  blue_wire.setCallback(blue_cut);
  red_wire.setCallback(red_cut);
  green_wire.setCallback(green_cut);
  brown_wire.setCallback(brown_cut);
  yellow_wire.setCallback(yellow_cut);
  orange_wire.setCallback(orange_cut);
  purple_wire.setCallback(purple_cut);      

  lcd.init();
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Welcome To The Bomb!");

  matrix.begin(0x70);

  start_ms = millis();
}

void loop() {
  blue_wire.update();
  red_wire.update();
  green_wire.update();
  brown_wire.update();
  yellow_wire.update();
  orange_wire.update();
  purple_wire.update();
  check_time();
  delay(1000); // THIS IS BAD DESIGN...USE INTERRUPTS FOR TIMER?
}
