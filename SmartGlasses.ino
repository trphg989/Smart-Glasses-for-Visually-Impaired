/*
  Smart Glasses - Buzzer Prototype
  Board: Arduino Nano (ATmega328P)
  Sensors: HC-SR04, GUVA-S12SD analog UV module
  Output: 5V passive buzzer

  Connections:
    HC-SR04: VCC->5V, GND->GND, TRIG->D2, ECHO->D3
    GUVA-S12SD: VCC->supported supply voltage, GND->GND, OUT->A0
    Passive buzzer (+)->D4, (-)->GND (only if buzzer current is low)

  IMPORTANT: UV_HIGH_ADC and UV_VERY_HIGH_ADC are uncalibrated
  example ADC values, NOT UV Index thresholds. Calibrate before use.
  This is a prototype, NOT a replacement for a mobility cane.
*/

#define TRIG_PIN 2
#define ECHO_PIN 3
#define BUZZER_PIN 4
#define UV_PIN A0

// Obstacle thresholds, in centimeters
const int DIST_DANGER_CM = 30;
const int DIST_NEAR_CM = 50;
const int DIST_MAX_CM = 100;

// TEST ONLY: calibrate these for the specific GUVA-S12SD module
const int UV_HIGH_ADC = 100;
const int UV_VERY_HIGH_ADC = 200;

const unsigned long SENSOR_PERIOD_MS = 120;
const unsigned long SERIAL_PERIOD_MS = 800;
const unsigned long UV_REPEAT_MS = 30000;
const unsigned long UV_ON_MS = 110;
const unsigned long UV_OFF_MS = 140;

int distanceCm = -1;
int uvRaw = 0;
int obstacleLevel = 0;
int previousObstacleLevel = 0;
int uvLevel = 0;
int previousUVLevel = 0;

unsigned long lastSensorAt = 0;
unsigned long lastSerialAt = 0;
unsigned long obstacleCycleAt = 0;
unsigned long lastUVAlertAt = 0;
bool uvAlertEverPlayed = false;

// UV sound sequence is an asynchronous state machine
bool uvPlaying = false;
int uvBeepCount = 0;
int uvBeepIndex = 0;
bool uvToneOn = false;
unsigned long uvStepAt = 0;

int readDistanceCm() {
  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG_PIN, LOW);

  unsigned long duration = pulseIn(ECHO_PIN, HIGH, 25000UL);
  if (duration == 0) return -1; // Timeout / invalid reading

  int cm = (int)(duration * 0.0343f / 2.0f);
  if (cm < 2 || cm > 400) return -1;
  return cm;
}

int readUVRaw() {
  long sum = 0;
  for (byte i = 0; i < 8; i++) {
    sum += analogRead(UV_PIN);
  }
  return (int)(sum / 8);
}

int classifyObstacle(int cm) {
  if (cm < 0 || cm > DIST_MAX_CM) return 0;
  if (cm < DIST_DANGER_CM) return 3;
  if (cm < DIST_NEAR_CM) return 2;
  return 1;
}

int classifyUV(int reading) {
  if (reading >= UV_VERY_HIGH_ADC) return 2;
  if (reading >= UV_HIGH_ADC) return 1;
  return 0;
}

void stopUVSound() {
  uvPlaying = false;
  uvToneOn = false;
  noTone(BUZZER_PIN);
}

void beginUVSound(unsigned long now) {
  uvPlaying = true;
  uvBeepCount = (uvLevel == 2) ? 3 : 2;
  uvBeepIndex = 0;
  uvToneOn = true;
  uvStepAt = now;
  lastUVAlertAt = now;
  uvAlertEverPlayed = true;
  tone(BUZZER_PIN, 3000);
}

void updateUVSound(unsigned long now) {
  if (!uvPlaying) return;

  if (uvToneOn && now - uvStepAt >= UV_ON_MS) {
    noTone(BUZZER_PIN);
    uvToneOn = false;
    uvStepAt = now;
    ++uvBeepIndex;
    if (uvBeepIndex >= uvBeepCount) {
      uvPlaying = false;
    }
  } else if (!uvToneOn && now - uvStepAt >= UV_OFF_MS) {
    tone(BUZZER_PIN, 3000);
    uvToneOn = true;
    uvStepAt = now;
  }
}

void updateObstacleSound(unsigned long now) {
  // Higher danger => faster beeps. No blocking delay().
  unsigned long period;
  unsigned long onTime;
  unsigned int frequency;

  if (obstacleLevel == 3) {
    period = 180; onTime = 110; frequency = 2500;
  } else if (obstacleLevel == 2) {
    period = 400; onTime = 100; frequency = 1900;
  } else {
    period = 900; onTime = 100; frequency = 1300;
  }

  unsigned long phase = (now - obstacleCycleAt) % period;
  if (phase < onTime) {
    tone(BUZZER_PIN, frequency);
  } else {
    noTone(BUZZER_PIN);
  }
}

void setup() {
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);
  pinMode(UV_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  digitalWrite(TRIG_PIN, LOW);
  noTone(BUZZER_PIN);

  Serial.begin(9600);
  Serial.println(F("Smart Glasses - buzzer version"));
  Serial.println(F("UV ADC thresholds are TEST VALUES only"));
}

void loop() {
  unsigned long now = millis();

  if (now - lastSensorAt >= SENSOR_PERIOD_MS) {
    lastSensorAt = now;
    distanceCm = readDistanceCm();
    uvRaw = readUVRaw();

    int newObstacle = classifyObstacle(distanceCm);
    if (newObstacle != obstacleLevel) {
      obstacleCycleAt = millis(); // restart beep immediately on zone change
    }
    obstacleLevel = newObstacle;
    uvLevel = classifyUV(uvRaw);
  }

  now = millis();
  if (obstacleLevel == 0 && previousObstacleLevel > 0) {
    noTone(BUZZER_PIN); // stop the last obstacle beep
  }
  if (obstacleLevel > 0) {
    if (uvPlaying) stopUVSound(); // obstacle always has priority
    updateObstacleSound(now);
  } else {
    // Record a new UV warning when reaching a higher risk zone.
    // Repeat only occasionally while the level stays elevated.
    bool uvIncreased = uvLevel > previousUVLevel;
    bool uvRepeatDue = !uvAlertEverPlayed ||
                       (now - lastUVAlertAt >= UV_REPEAT_MS);

    if (uvLevel == 0) {
      if (uvPlaying) stopUVSound();
      noTone(BUZZER_PIN);
      uvAlertEverPlayed = false;
    } else {
      if (!uvPlaying && (uvIncreased || uvRepeatDue)) {
        beginUVSound(now);
      }
      updateUVSound(now);
    }
  }
  previousUVLevel = uvLevel;
  previousObstacleLevel = obstacleLevel;

  if (now - lastSerialAt >= SERIAL_PERIOD_MS) {
    lastSerialAt = now;
    Serial.print(F("Distance: "));
    Serial.print(distanceCm);
    Serial.print(F(" cm | UV ADC: "));
    Serial.print(uvRaw);
    Serial.print(F(" | Obstacle level: "));
    Serial.print(obstacleLevel);
    Serial.print(F(" | UV level (uncalibrated): "));
    Serial.println(uvLevel);
  }
}
