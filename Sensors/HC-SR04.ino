const int trigPin = 9;  
const int echoPin = 10; 

long duration;
int distance;

void setup() {
  Serial.begin(9600);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
}

void loop() {  
  Serial.print("Jarak : ");
  Serial.print(Sensor());
  Serial.println(" cm")
}

float Sensor(){
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
   
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);
        
    duration = pulseIn(echoPin, HIGH);  
    distance = duration * 0.0344 / 2;
    
    return distance;
}