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
 * 
 */
#include <ButtonDebounce.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_GFX.h>
#include <Adafruit_LEDBackpack.h>
#include <TimerOne.h>

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
unsigned long interval = 1;       // seconds
unsigned long remaining = 45;     // seconds
int ledState = LOW;

// Puzzle
const int sequence[] = { ORANGE, BLUE, YELLOW };    // This is the correct removal sequence to defuse the bomb
const int num_wires = 3;
int next_to_cut = 0;                              // Which wire in the sequence should be cut next?

void show_cut_wire(const char* color) {
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

void check_time(void) {
  // "Heartbeat"
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  
  // Count Down
  if (remaining > 0) {
    remaining -= interval;
  }
}


void setup() { 
  for (int p=2; p < 9; p++) {   // DEPENDS ON "WIRE" PINS BEING SEQUENTIALLY DEFINED ABOVE!
    pinMode(p, INPUT);
  }

  // Timer1 'heartbeat'
  pinMode(LED_BUILTIN, OUTPUT);
    
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

  Timer1.initialize(interval*1000000);  // needs microseconds
  Timer1.attachInterrupt(check_time);

}

void loop() {
  blue_wire.update();
  red_wire.update();
  green_wire.update();
  brown_wire.update();
  yellow_wire.update();
  orange_wire.update();
  purple_wire.update();

  // Convert seconds count to a displayable time
  int display_min = remaining / 60;
  int display_sec = remaining % 60;

  // Write the displayable time to the 7-segment display
  matrix.writeDigitNum(0, (display_min / 10) % 10);
  matrix.writeDigitNum(1, display_min % 10);
  matrix.drawColon(true);
  matrix.writeDigitNum(3, (display_sec / 10) % 10);
  matrix.writeDigitNum(4, display_sec % 10);
  matrix.writeDisplay();
  
  if (remaining <= 0) {
      lcd.setCursor(0,2);
      lcd.print("  BOOM!"); 
  }
  delay(100);
}
