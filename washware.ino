// Arduino Washing Machine Control Code with Enhanced Logic

// Pin Definitions
#define MOTOR_FORWARD_PIN 12   // Motor Forward (A)
#define MOTOR_BACKWARD_PIN 11  // Motor Backward (B)
#define WATER_VALVE_PIN 10     // Water Valve (C)
#define DRUM_RINSE_PIN 9       // Drum & Rinse (D)

#define WATER_LEVEL_PIN 7      // Water Level Selector (M)
#define TIME_SELECTOR_PIN 6    // Time Selector (T)
#define SOAK_PIN 5             // If Soak (S)
#define START_PIN 4            // Start Button

#define DOOR_SENSOR_PIN 8      // Door Sensor

#define WATER_LEVEL_LED 3      // LED for Water Level (M)
#define TIME_SELECTOR_LED 2    // LED for Time Selector (T)
#define SOAK_LED 13             // LED for Soak (S)

// Global Variables
int waterLevelState = 0; // 0: Low, 1: Mid, 2: High
int timeSelectorState = 0; // 0: 15 min, 1: 30 min, 2: 45 min
bool soakEnabled = false; // Soak state

// Setup function
void setup() {
  // Initialize pins
  pinMode(MOTOR_FORWARD_PIN, OUTPUT);
  pinMode(MOTOR_BACKWARD_PIN, OUTPUT);
  pinMode(WATER_VALVE_PIN, OUTPUT);
  pinMode(DRUM_RINSE_PIN, OUTPUT);

  pinMode(WATER_LEVEL_PIN, INPUT);
  pinMode(TIME_SELECTOR_PIN, INPUT);
  pinMode(SOAK_PIN, INPUT);
  pinMode(START_PIN, INPUT);

  pinMode(DOOR_SENSOR_PIN, INPUT);

  pinMode(WATER_LEVEL_LED, OUTPUT);
  pinMode(TIME_SELECTOR_LED, OUTPUT);
  pinMode(SOAK_LED, OUTPUT);

  // Initialize components to OFF state
  digitalWrite(MOTOR_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_BACKWARD_PIN, LOW);
  digitalWrite(WATER_VALVE_PIN, LOW);
  digitalWrite(DRUM_RINSE_PIN, LOW);
}

// Main loop
void loop() {
  Serial.begin(9600);
  // Update inputs
  updateInputs();

  // Wait for the start button and door sensor
  if (digitalRead(START_PIN) == HIGH && digitalRead(DOOR_SENSOR_PIN) == HIGH) {
    startWashingSequence();
  }

  // Add a small delay to debounce inputs
  delay(100);
}

// Update inputs and LEDs
void updateInputs() {
  // Water Level Selector
  if (digitalRead(WATER_LEVEL_PIN) == HIGH) {
    Serial.println("WATER SW!!");
    waterLevelState = (waterLevelState + 1) % 3;
    // updateWaterLevelLED();
    
  }

  // Time Selector
  if (digitalRead(TIME_SELECTOR_PIN) == HIGH) {
    Serial.println("TIME SW!!");
    timeSelectorState = (timeSelectorState + 1) % 3;
    updateTimeSelectorLED();
  }

  // Soak Selector
  if (digitalRead(SOAK_PIN) == HIGH) {
    Serial.println("SOAK SW!!");
    soakEnabled = !soakEnabled;
    updateSoakLED();
  }
}

// Update Water Level LED
void updateWaterLevelLED() {
  switch (waterLevelState) {
    case 0: // Low
      blinkLED(WATER_LEVEL_LED, 1000);
      break;
    case 1: // Mid
      blinkLED(WATER_LEVEL_LED, 250);
      break;
    case 2: // High
      digitalWrite(WATER_LEVEL_LED, HIGH);
      break;
  }
}

// Update Time Selector LED
void updateTimeSelectorLED() {
  digitalWrite(TIME_SELECTOR_LED, HIGH);
  delay(250 * (timeSelectorState + 1));
  digitalWrite(TIME_SELECTOR_LED, LOW);
}

// Update Soak LED
void updateSoakLED() {
  digitalWrite(SOAK_LED, soakEnabled ? HIGH : LOW);
}

// Start the washing sequence
void startWashingSequence() {
  // Ensure all components are OFF
  resetComponents();

  // Water Filling
  fillWater();

  // Soaking
  if (soakEnabled) {
    soak();
  }

  // Main Washing Cycle
  mainWashingCycle();

  // Final Adjustments
  finalCycleAdjustments();

  // End program
  resetComponents();
}

// Reset all components to OFF
void resetComponents() {
  digitalWrite(MOTOR_FORWARD_PIN, LOW);
  digitalWrite(MOTOR_BACKWARD_PIN, LOW);
  digitalWrite(WATER_VALVE_PIN, LOW);
  digitalWrite(DRUM_RINSE_PIN, LOW);
}

// Fill water based on water level
void fillWater() {
  int fillTime = 0;
  switch (waterLevelState) {
    case 0: fillTime = 120000; break; // Low
    case 1: fillTime = 300000; break; // Mid
    case 2: fillTime = 600000; break; // High
  }

  digitalWrite(WATER_VALVE_PIN, HIGH);
  delay(fillTime);
  digitalWrite(WATER_VALVE_PIN, LOW);
}

// Soaking process
void soak() {
  delay(1200000); // Wait for 20 minutes
}

// Main washing cycle
void mainWashingCycle() {
  int totalWashTime = (timeSelectorState + 1) * 900000; // 15, 30, 45 minutes
  int cycleCount = 2;
  int cycleTime = totalWashTime / cycleCount;

  for (int i = 0; i < cycleCount; i++) {
    washStage(cycleTime);
    rinseStage(i == (cycleCount - 1)); // Final rinse if last cycle
  }
}

// Washing stage
void washStage(int duration) {
  unsigned long startTime = millis();
  while (millis() - startTime < duration) {
    digitalWrite(MOTOR_FORWARD_PIN, HIGH);
    delay(120000);
    digitalWrite(MOTOR_FORWARD_PIN, LOW);
    delay(30000);

    digitalWrite(MOTOR_BACKWARD_PIN, HIGH);
    delay(120000);
    digitalWrite(MOTOR_BACKWARD_PIN, LOW);
    delay(30000);
  }
}

// Rinse stage
void rinseStage(bool finalCycle) {
  digitalWrite(DRUM_RINSE_PIN, HIGH);
  delay(900000); // Rinse for 15 minutes
  resetComponents();
  delay(5000);

  digitalWrite(MOTOR_BACKWARD_PIN, HIGH);
  digitalWrite(DRUM_RINSE_PIN, HIGH);
  delay(finalCycle ? 300000 : 30000); // Adjust timing for final cycle
  resetComponents();
}

// Final cycle adjustments
void finalCycleAdjustments() {
  int fillTime = 0;
  switch (waterLevelState) {
    case 0: fillTime = 60000; break; // Low
    case 1: fillTime = 120000; break; // Mid
    case 2: fillTime = 180000; break; // High
  }

  digitalWrite(WATER_VALVE_PIN, HIGH);
  delay(fillTime);
  digitalWrite(WATER_VALVE_PIN, LOW);

  rinseStage(true);
}

// Blink LED function
void blinkLED(int pin, int interval) {
  digitalWrite(pin, HIGH);
  delay(interval);
  digitalWrite(pin, LOW);
  delay(interval);
}

// Debugging Section (commented out)

void debugInputsAndOutputs() {
  Serial.begin(9600);
  // Serial.print("Water Level State: "); Serial.println(waterLevelState);
  // Serial.print("Time Selector State: "); Serial.println(timeSelectorState);
  // Serial.print("Soak Enabled: "); Serial.println(soakEnabled);
  // Serial.print("Door Sensor: "); Serial.println(digitalRead(DOOR_SENSOR_PIN));
}

