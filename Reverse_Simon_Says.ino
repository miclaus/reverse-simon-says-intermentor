/**
 * Reverse Simon Says
 * 
 * @author Iosif Miclaus
 * @author Michael Navratil
 */

// Buttons
const int top_button = 7;
const int left_button = 2;
const int right_button = 4;
// LEDs
const int main_LED = 10;
const int top_LED = 13;
const int left_LED = 8;
const int right_LED = 12;

// other constants
const int min_main_LED = 15;
const int max_main_LED = 255;
const long interval_main_LED = 25; // ms
const int fade_interval = 5; // ms
const int required_action_millis = 2600; // ms
const int action_required_action_millis = 240; // ms
const int cooldown_period_millis = 1000; // ms

// Button states
int top_button_state = 0;
int left_button_state = 0;
int right_button_state = 0;

// other
unsigned long prev_millis = 0;
int prev_fade_value = min_main_LED;
int fade_modifier = 1;
boolean is_serial_busy = false;
boolean tripple_action = false;
unsigned long init_new_game_millis = 0;
boolean new_game_is_initiating = false;
unsigned long cooldown_period_start_millis = 0;

// actions
boolean sending_next_guess = false;
int last_sending_next_guess_millis = 0;


void setup() {
  pinMode(top_button, INPUT);
  pinMode(left_button, INPUT);
  pinMode(right_button, INPUT);

  pinMode(main_LED, OUTPUT);
  pinMode(top_LED, OUTPUT);
  pinMode(left_LED, OUTPUT);
  pinMode(right_LED, OUTPUT);

  sync_blinks(4);

  Serial.begin(9600);
}


void loop() {
  unsigned long curr_millis = millis();
  if ( (unsigned long)(curr_millis - prev_millis) >= interval_main_LED ) {
    prev_millis = curr_millis;
    
    int fade_value = prev_fade_value + (fade_interval * fade_modifier);
    prev_fade_value = fade_value;

    if (fade_value >= max_main_LED || fade_value <= min_main_LED) {
      fade_modifier *= -1;      
    }
    
    analogWrite(main_LED, fade_value);
  }


  // algorithm

  top_button_state = digitalRead(top_button);
  left_button_state = digitalRead(left_button);
  right_button_state = digitalRead(right_button);
    
  tripple_action = ( 
       top_button_state == HIGH 
    && left_button_state == HIGH 
    && right_button_state == HIGH 
  );

  unsigned long curr_new_game_millis = millis();
  
  if (tripple_action) {
    display_actions(top_button_state, left_button_state, right_button_state);
    
    if (new_game_is_initiating == false) {
      new_game_is_initiating = true;
      init_new_game_millis = curr_new_game_millis;
    } else {
      if ( (unsigned long)(curr_new_game_millis - init_new_game_millis) 
        >= required_action_millis
      ) {
        new_game_is_initiating = false;
        Serial.println("N");
        sync_blinks(3);
        cooldown_period_start_millis = millis();
      }
    }
  } else {
    if ( (unsigned long)(curr_new_game_millis - cooldown_period_start_millis)
      >= cooldown_period_millis
    ) {
      display_actions(top_button_state, left_button_state, right_button_state);

      new_game_is_initiating = false;
    }
  }
}


void sync_blinks(int count) {
   for(int c = 0; c < count; c++) {
    analogWrite(main_LED, max_main_LED);
    delay(20);

    if (c != count) {
      analogWrite(main_LED, 0);
      delay(80);
    }
  }
}


void display_actions(
  int top_button_state, 
  int left_button_state, 
  int right_button_state
) {
  unsigned long curr_sending_next_guess_millis = millis();

  if ( tripple_action ) {
    digitalWrite(top_LED, HIGH);
    digitalWrite(left_LED, HIGH);
    digitalWrite(right_LED, HIGH);
  }
  
  if ( top_button_state == HIGH
    && left_button_state == LOW 
    && right_button_state == LOW
  ) {
    digitalWrite(top_LED, HIGH);

    if ( ! tripple_action ) {
      if (sending_next_guess == false) {
        sending_next_guess = true;
        last_sending_next_guess_millis = curr_sending_next_guess_millis;
      } else {
        if ( (unsigned long)(curr_sending_next_guess_millis - last_sending_next_guess_millis)
          >= action_required_action_millis
        ) {
          sending_next_guess = false;
          Serial.println("T");
        }
      }
    }
  } else if ( 
       top_button_state == LOW 
    && left_button_state == HIGH 
    && right_button_state == LOW
  ) {
    digitalWrite(left_LED, HIGH);
    
    if ( ! tripple_action ) {
      if (sending_next_guess == false) {
        sending_next_guess = true;
        last_sending_next_guess_millis = curr_sending_next_guess_millis;
      } else {
        if ( (unsigned long)(curr_sending_next_guess_millis - last_sending_next_guess_millis)
          >= action_required_action_millis
        ) {
          sending_next_guess = false;
          Serial.println("L");
        }
      }
    }
  } else if ( 
       top_button_state == LOW
    && left_button_state == LOW 
    && right_button_state == HIGH
  ) {
    digitalWrite(right_LED, HIGH);

    if ( ! tripple_action ) {
      if (sending_next_guess == false) {
        sending_next_guess = true;
        last_sending_next_guess_millis = curr_sending_next_guess_millis;
      } else {
        if ( (unsigned long)(curr_sending_next_guess_millis - last_sending_next_guess_millis)
          >= action_required_action_millis
        ) {
          sending_next_guess = false;
          Serial.println("R");
        }
      }
    }
  } else {
    digitalWrite(top_LED, LOW);
    digitalWrite(left_LED, LOW);
    digitalWrite(right_LED, LOW);
  }
}
