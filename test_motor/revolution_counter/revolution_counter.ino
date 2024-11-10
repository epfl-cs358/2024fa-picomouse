
volatile int count1 = 0; 
volatile int count2 = 0; 


const int encoderPin1 = 2;  // rev counter pin motor 1
const int encoderPin2 = 3;  // rev counter pin motor 2






void setup() {

  attachInterrupt(digitalPinToInterrupt(encoderPin1), countEncoder1, RISING);
  attachInterrupt(digitalPinToInterrupt(encoderPin2), countEncoder2, RISING);
  
  Serial.begin(9600);
}

void loop() {
}


//Attention, Serial.print is slow ---> moving the motor too fast will not count all the revolution. 
// --> dont't use Serial is you want to do a lot of rpm. 

void countEncoder1() {
  count1++;
  Serial.print("Motor 1: ");
  Serial.println(count1);
}


void countEncoder2() {
  count2++;
  Serial.print("Motor 2: ");
  Serial.println(count2);
}
