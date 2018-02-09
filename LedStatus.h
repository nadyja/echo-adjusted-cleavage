int ledPin  = D7;

class LedStatus {
    public:
        LedStatus(int test) {
            pinMode(ledPin, OUTPUT); 
        }
        void Setup() {
        }
        void Update(bool isPresent) {
            if(isPresent) {
                digitalWrite(ledPin, HIGH);
            } else {
                digitalWrite(ledPin, LOW);
            }
        } 
};
