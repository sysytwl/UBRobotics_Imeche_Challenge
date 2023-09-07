#define IN1 2  // Define the control pin for coil 1
#define IN2 3  // Define the control pin for coil 1
#define IN3 4  // Define the control pin for coil 2
#define IN4 5  // Define the control pin for coil 2

int stepCounter = 0;

void setup() {
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  Serial.begin(9600); // Start serial communication for debugging
}

void loop() {
  moveForward(10);  // Move stepper forward for 10 steps
  delay(1000);
 
  moveBackward(5);  // Move stepper backward for 5 steps
  delay(1000);
 
  stopMotor();      // Stop the motor
  delay(1000);
 
  Serial.println(stepCounter); // Print the current step count
}

void moveForward(int steps) {
  for (int i = 0; i < steps; i++) {
    stepCounter++;
   
    // Step sequence for forward motion
    stepAction(IN1, LOW, IN2, HIGH, IN3, HIGH, IN4, LOW);
    stepAction(IN1, HIGH, IN2, LOW, IN3, HIGH, IN4, LOW);
    stepAction(IN1, HIGH, IN2, LOW, IN3, LOW, IN4, HIGH);
    stepAction(IN1, LOW, IN2, HIGH, IN3, LOW, IN4, HIGH);
  }
}

void moveBackward(int steps) {
  for (int i = 0; i < steps; i++) {
    stepCounter--;
   
    // Step sequence for backward motion
    stepAction(IN1, LOW, IN2, HIGH, IN3, LOW, IN4, HIGH);
    stepAction(IN1, HIGH, IN2, LOW, IN3, LOW, IN4, HIGH);
    stepAction(IN1, HIGH, IN2, LOW, IN3, HIGH, IN4, LOW);
    stepAction(IN1, LOW, IN2, HIGH, IN3, HIGH, IN4, LOW);
  }
}

void stepAction(int pin1, bool state1, int pin2, bool state2, int pin3, bool state3, int pin4, bool state4) {
  digitalWrite(pin1, state1);
  digitalWrite(pin2, state2);
  digitalWrite(pin3, state3);
  digitalWrite(pin4, state4);
  delay(2); // Time delay between steps
}

void stopMotor() {
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
