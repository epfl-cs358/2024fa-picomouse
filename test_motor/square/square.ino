//Motor
const int IN1 = 5;   //  moteur 1
const int IN2 = 6;   //  moteur 1
const int IN3 = 9;   //  moteur 2
const int IN4 = 10;  //  moteur 2


const int wheelToMotorRatio = 212; //212 motor revolution for 1 wheel revoluion
const float wheelDiameter = 37.0;
const int wheelDistance = 85;
const double mouseRadius =  wheelDistance / 2.0; 
const float wheelCircumference = wheelDiameter * M_PI;


const int encoderPin1 = 3;  //Pin Rev counter motor 1
const int encoderPin2 = 2;  ///Pin Rev counter motor 2


volatile long int count1 = 0;  // counter motor 1
volatile long int count2 = 0;  // counter motor 2


int targetTurns1 = 0;
int targetTurns2 = 0;
bool motor1Running = false;
bool motor2Running = false;

void setup() {
	//motor
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);

  // revolution counter
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);

  //Interruptions
  attachInterrupt(digitalPinToInterrupt(encoderPin1), countEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), countEncoder2, RISING);

  Serial.begin(9600);
}

void loop() {
  delay(1000);
  moveSquare(100, 50); 
  delay(5000);        

  //if (!motor1Running) {
  //   rotateWheel1(1, 100, true);
  // }
  /**
  if (!motor2Running) {
     rotateWheel2(3, 200, true);
   }

  // 
  // if (motor1Running && count1 >= targetTurns1) {
  //   stopMotor1();
  //   motor1Running = false;
  //   Serial.println("Moteur 1 a atteint le nombre de tours de roue demandé.");
  //   delay(4000);
  // }

  if (motor2Running && count2 >= targetTurns2) {
    stopMotor2();
    motor2Running = false;
    Serial.println("Moteur 2 a atteint le nombre de tours de roue demandé.");
    delay(1000);
  }**/
}


void rotateWheel1(int speed, bool direction) {
  count1 = 0;  
  motor1Running = true;

  if (direction) {
    analogWrite(IN1, speed);  
    digitalWrite(IN2, LOW);   
  } else {
    analogWrite(IN2, speed);  
    digitalWrite(IN1, LOW);   
  }
}


void rotateWheel2(int speed, bool direction) {
  count2 = 0;  
  motor2Running = true;

  if (direction) {
    analogWrite(IN3, speed);  
    digitalWrite(IN4, LOW);   
  } else {
    analogWrite(IN4, speed);  
    digitalWrite(IN3, LOW);  
  }
}


void stopMotor1() {
  analogWrite(IN2, 80);  
  digitalWrite(IN1, LOW);  
  delay(25);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  motor1Running = false;
  count1 = 0; 
}


void stopMotor2() {
  analogWrite(IN4, 80);  
  digitalWrite(IN3, LOW);  
  delay(25);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  motor2Running = false;
  count2 = 0;
}



void countEncoder1() {
  count1++;
}


void countEncoder2() {
  count2++;  
}



int calculateTurnsForDistance(float distanceCm) {
  float distanceMm = distanceCm * 10;  // Conversion cm -> mm
  return distanceMm / wheelCircumference * wheelToMotorRatio;
}


void moveSquare(int speed, int size) {
  int turnsForSide = calculateTurnsForDistance(size); //nbr turn for one side of squa
  Serial.println(turnsForSide);
  for (int i = 0; i < 4; i++) {
    rotateWheel1(speed, true);  // Moteur 1
    rotateWheel2(speed, true);  // Moteur 2

    while (motor1Running || motor2Running) {
      if (count1 >= turnsForSide) { stopMotor1(); }
      if (count2 >= turnsForSide) { stopMotor2(); }
    }
  
    // Rotation de 90
    rotateWheel2(speed, true);
    const double perim =  wheelDistance * M_PI / 2 ;
    const int rotate90 = perim / wheelCircumference * wheelToMotorRatio;
    Serial.println(rotate90);
    
    while (motor2Running) {
      //if (count1 >= rotate90) {  stopMotor1(); }
      if (count2 >= rotate90) {  stopMotor2(); } 
    }
  }
}
