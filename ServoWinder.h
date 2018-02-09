#define WINDING_UP 180
#define WINDING_DOWN 0
#define IDLE 90

int servoDirectionPin       = D0;

long servoStepDuration      = 100;  // ms
long servoWindingDuration   = 2000; // ms

int motorDirection          = IDLE;
unsigned long currentSteps  = 0;
bool autoRun = true;
Servo myservo; 

class ServoWinder {
    unsigned long stepsThresholdUp;
    unsigned long stepsThresholdDown;
    unsigned long previousMillis;
    
    private:
        bool isUpReached() {
            return currentSteps >= stepsThresholdUp;
        }
        bool isDownReached() {
            return currentSteps <= stepsThresholdDown;
        }
        int getDirection(bool isPresent) {
            if(isPresent && !isUpReached()) {
               return WINDING_UP;
            } else if(!isPresent && !isDownReached()) {
                return WINDING_DOWN;
            } else {
                return IDLE;
            }
        }
        void setMotor(int direction) {
            myservo.write(direction);
            if(direction == WINDING_UP) currentSteps += servoStepDuration;
            if(direction == WINDING_DOWN) currentSteps -= servoStepDuration;
        }
    public:
        ServoWinder(int seed) {
            stepsThresholdUp = servoWindingDuration;
            stepsThresholdDown = 0;
            currentSteps = 0;
            previousMillis = 0;
            motorDirection = IDLE;

            myservo.attach(servoDirectionPin);
            Particle.function("setWinding", &ServoWinder::setWinding, this);
            Particle.function("setMotor", &ServoWinder::overrideMotor, this);
        }
        int setWinding(String command) {
            if(command.toInt()) {
                servoWindingDuration = command.toInt();
                return 1;
            }
            else return -1;
        }
        int overrideMotor(String command) {
            if (command == "stop") {
                autoRun = false;
                setMotor(IDLE);
                return 1;
            } else if (command == "up") {
                autoRun = false;
                setMotor(WINDING_UP);
                return 1;
            } else if (command == "down") {
                autoRun = false;
                setMotor(WINDING_DOWN);
                return 1;
            } else {
                autoRun = true;
                return -1;
            }
        }
        void Setup() {
            Particle.variable("direction", motorDirection);
            Particle.variable("steps", currentSteps);
        }
        void Update(bool isPresent) {
            unsigned long currentMillis = millis();
            if (currentMillis - previousMillis > servoStepDuration && autoRun) {
                motorDirection = getDirection(isPresent);
                setMotor(motorDirection);
                previousMillis = currentMillis;
            }
        }
};
