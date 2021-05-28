/*
 * Defuse the Bomb!
 * 
 * Goal: "Cut" the specific colored wires in the correct sequence to stop the bomb from detonating!
 * 
 * Cutting a wire outside the correct sequence speeds up the clock.
 * 
 * If a wire within the correct sequence is cut out of order, or the time expires before the sequence is completed correctly,
 *   the bomb will go off (BOOM pin goes HIGH).
 * 
 * If each wire in the sequence is cut in the correct order, the bomb is defused.
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

#define BEEP    9     // Audible 'beep' for each clock tick

#define BOOM    10    // Goes HIGH if Bomb has gone off

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
unsigned long interval = 250000;     // seconds between Timer hitting callback
volatile unsigned int step = 4;      // how many intervals to wait across callbacks to decrement remaining (clock countdown rate)
volatile int step_counter = 0;
int ledState = LOW;

// Puzzle
volatile unsigned long remaining = 300;                     // seconds left until bomb goes off  (needs volatile since it's shared with interrupt)
const int correct_wires[] = { ORANGE, BLUE, YELLOW };       // This is the correct removal sequence to defuse the bomb
const int num_correct = 3;
const int other_wires[] = { RED, GREEN, BROWN, PURPLE };    // These wires do not cause bomb to go off, but each speeds up timer a bit more
const int num_other = 4;
int next_to_cut = 0;                                        // Which wire in the sequence should be cut next?
volatile bool boom = false;                                          // Should be bomb go off?
volatile bool active = true;                                         // Is the bomb still active?

void blue_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Blue");
    check_cut_wire(BLUE);
  }
}

void red_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Red");
    check_cut_wire(RED);
  }
}

void green_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Green");
    check_cut_wire(GREEN);
  }
}

void brown_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Brown");
    check_cut_wire(BROWN);
  }
}

void yellow_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Yellow");
    check_cut_wire(YELLOW);
  }
}

void orange_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Orange");
    check_cut_wire(ORANGE);
  }
}

void purple_cut(const int state) {
  if (state == LOW) {
    show_cut_wire("Purple");
    check_cut_wire(PURPLE);
  }
}

// Called on each interval
void check_time(void) {
  step_counter++;
  
  // "Heartbeat"
  ledState = !ledState;
  digitalWrite(LED_BUILTIN, ledState);
  digitalWrite(BEEP, LOW);    // beep is normally off, we only 'beep' when actually changing a number
                
  // Count Down
  if (active && !boom) {
    if (remaining > 0 && (step_counter >= step)) {  // Only decrease time when we're on a 'step'
      remaining -= 1;
      step_counter = 0;
      digitalWrite(BEEP, HIGH);
    }
  }
}

void show_cut_wire(const char* color) {
  lcd.setCursor(1, 5);
  lcd.print(blank);
  lcd.setCursor(1, 5);  
  lcd.print(color);
  lcd.print(" was cut!");
}

// Key logic for the puzzle.
void check_cut_wire(const int wire) {
  if ( wire != correct_wires[next_to_cut] ) {
    // did we cut a wire that speeds up the clock?
    for ( int w=0; w < num_other; w++ ) {
      if ( wire == other_wires[w] ) {
        // Speed up the clock a bit 
        noInterrupts();
        step -= 1;
        interrupts();
        return;
      }
    }
    // If you get here, we know a 'correct' wire was cut out of sequence...bomb will go off!
    boom = true;
    return;
  } else {
    // They cut correct wire...
    next_to_cut++;
    if ( next_to_cut == num_correct ) {
      // Bomb defused!
      active = false;
    }
    return;
  }
}


void setup() { 
  for (int p=2; p < 9; p++) {   // DEPENDS ON "WIRE" PINS BEING SEQUENTIALLY DEFINED ABOVE!
    pinMode(p, INPUT);
  }

  // Timer 'heartbeat'
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BEEP, OUTPUT);

  // Trigger pin- goes low if bomb has gone off
  pinMode(BOOM, OUTPUT);
  digitalWrite(BOOM, HIGH);
    
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

  Timer1.initialize(interval);
  Timer1.attachInterrupt(check_time);

}

void loop() {
  if ( active ) {
    if ( !boom ) {
      blue_wire.update();
      red_wire.update();
      green_wire.update();
      brown_wire.update();
      yellow_wire.update();
      orange_wire.update();
      purple_wire.update();

      // Need to make a 'local' copy of remaining variable since it's updated by the interrupt
      unsigned long local_remaining;
      noInterrupts();
      local_remaining = remaining;
      interrupts();
    
      // Convert seconds count to a displayable time
      int display_min = local_remaining / 60;
      int display_sec = local_remaining % 60;
    
      // Write the displayable time to the 7-segment display
      matrix.writeDigitNum(0, (display_min / 10) % 10);
      matrix.writeDigitNum(1, display_min % 10);
      matrix.drawColon(true);
      matrix.writeDigitNum(3, (display_sec / 10) % 10);
      matrix.writeDigitNum(4, display_sec % 10);
      matrix.writeDisplay();

      // Did we time out?
      if (local_remaining <= 0) {
        boom = true;
      }
    } else {
      lcd.setCursor(0,2);
      lcd.print("  BOOM!"); 
      digitalWrite(BOOM, LOW);
    }
  } else {
      lcd.setCursor(0,2);
      lcd.print("  DISARMED"); 
  }
}
