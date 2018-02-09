#define EMPTY 1
#define ENTERING 2
#define PRESENT 3
#define LEAVING 4

int sonarTriggerPin         = D1;
int sonarEchoPin            = D2;
long sonarPingInterval      = 200;  // ms
long sonarHoverDelay        = 1000; // ms
long sonarDistanceThreshold = 500;  // mm
bool sonarEnabled = true;
bool isDetected = false;
    
class Sonar {
    int sonarState;
    unsigned long previousHoverMillis;
    unsigned long previousPingMillis;

    private:
        long getDistance_mm() {
            digitalWrite(sonarTriggerPin, HIGH);
            delayMicroseconds(50);
            digitalWrite(sonarTriggerPin, LOW);
            long Time_Echo_us = pulseIn(sonarEchoPin, HIGH);
            if((Time_Echo_us < 60000) && (Time_Echo_us > 10)) {
              long distance_mm = (Time_Echo_us*34/100)/2;
              return distance_mm;
            }
            return 600;
        }
    public:
        Sonar(int a) {
            sonarState = EMPTY; 
            isDetected = false;
            previousHoverMillis = 0;
            previousPingMillis = 0;
            
            pinMode(sonarTriggerPin, OUTPUT); 
            pinMode(sonarEchoPin, INPUT); 
            Particle.function("setDistance", &Sonar::setDistance, this);
            Particle.function("toggleSwitch", &Sonar::toggleSwitch, this);
        }
        int setDistance(String command) {
            if(command.toInt()) {
                sonarDistanceThreshold = command.toInt();
                return 1;
            }
            else return -1;
        }
        int toggleSwitch(String command) {
            if(command == "on") {
                sonarEnabled = true;
                return 1;
            } else {
                sonarEnabled = false;
                return -1;
            }
        }
        void Setup() {
            Particle.variable("isDetected", isDetected);
        }
        bool Update(bool& state) {
            unsigned long currentMillis = millis();
            if (currentMillis - previousPingMillis > sonarPingInterval) {
                isDetected = getDistance_mm() < sonarDistanceThreshold;
                
                if (isDetected) {
                    if (sonarState == EMPTY) sonarState = ENTERING;
                    if (sonarState == LEAVING) sonarState = PRESENT;
                } else {
                    if (sonarState == PRESENT) sonarState = LEAVING;
                    if (sonarState == ENTERING) sonarState = EMPTY;
                }
                previousPingMillis = currentMillis;
            }
            if (currentMillis - previousHoverMillis > sonarHoverDelay) {
                if (sonarState == ENTERING && isDetected) sonarState = PRESENT;
                if (sonarState == LEAVING && !isDetected) sonarState = EMPTY;
                previousHoverMillis = currentMillis;
            }
            return (sonarState == PRESENT || sonarState == LEAVING);
         }
};
