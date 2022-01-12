#include <Servo.h>

#define yawAngleChangePin A0
#define pitchAngleChangePin A1
#define laserOutputPin 8
#define pitchAnglePin 9
#define yawAnglePin 10
#define laserInputPin 11

Servo yawServo;
Servo pitchServo;

float yawAngle = 90.0f;
float pitchAngle = 90.0f;

void setup() {
  /*Gör olika förberedelser som krävs för att turreten ska funka. Sätter rätt pinmode 
  på input och output pinsen för lasern och fäster rätt pins till servomotorerna*/
  pinMode(laserInputPin, INPUT_PULLUP);
  pinMode(laserOutputPin, OUTPUT);
  yawServo.attach(yawAnglePin);
  pitchServo.attach(pitchAnglePin);
}

int changeAngleJoystick(int input, float currentAngle) {
  //Tar joystickens utslag och den nuvarande vinkeln på servomotorerna som argument och returnerar den nya vinkeln till servomotorn.
  float angle = currentAngle;
  int joystickAngle = analogRead(input);
  if (joystickAngle < 500 && angle < 180.0f) {
    angle += map(joystickAngle,0,521,1.0f,0.0f);
  } else if (joystickAngle > 550 && angle > 0.0f) {
    angle -= map(joystickAngle,523,1023,0.0f,1.0f);
  }
  /* Beroende på hur långt joysticken är dragen kommer servomotorernas vinkel ändras olika mycket.
  Ju mer joysticken är dragen desto mer kommer servomotorn snurra. Kollar även åt vilket håll servomotorn ska snurra. */

  angle = (angle > 180.0f) ? 180.0f : angle;
  angle = (angle < 0.0f) ? 0.0f : angle;
  /*Det kan hända att de tilldelade värdena för angle kan vara över 180 grader eller 
  under 0 grader eftersom att den innan kan vara 179.9 grader och höjas med 1 vilket blir 180.9.
  Vice versa för under 0 */

  return angle;
}

void loop() {
  yawAngle = changeAngleJoystick(yawAngleChangePin, yawAngle); //Kallar funktionen för att räkna ut den nya vinkeln för yaw vinkeln
  yawServo.write(int(yawAngle)); //Snurrar yaw servomotorn till sin nya vinkel
  pitchAngle = changeAngleJoystick(pitchAngleChangePin, pitchAngle); //Kallar funktionen för att räkna ut den nya vinkeln för pitch vinkeln
  pitchServo.write(int(pitchAngle)); //Snurrar pitch servomotorn till sin nya vinkel

  digitalWrite(laserOutputPin, (digitalRead(laserInputPin) == 1) ? HIGH : LOW); //Om knappen på joysticken är intryckt tänds lasern annars släcks den.

}
