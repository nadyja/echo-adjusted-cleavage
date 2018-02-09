#include "Sonar.h"
#include "ServoWinder.h"
#include "LedStatus.h"

Sonar sonar(1);
ServoWinder servo(1);
LedStatus led(1);

bool sonarState = false;
int brewCoffee(String command);

void setup() {
    sonar.Setup();
    servo.Setup();
    led.Setup();
    Particle.variable("sonarState", sonarState);
}

void loop() {
    sonarState = sonar.Update(sonarState);
    led.Update(sonarState);
    servo.Update(sonarState);
    delay(10);
}

