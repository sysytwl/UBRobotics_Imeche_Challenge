const int out = 15;
const int s2 = 2;
const int s3 = 4;


void setup() {  
  Serial.begin(115200); 
  pinMode(s2, OUTPUT);  
  pinMode(s3, OUTPUT);  
  pinMode(out, INPUT);  
}

void loop() {  
  color();
  delay(500);
}

void color() {    
  //red
  digitalWrite(s2, LOW);  
  digitalWrite(s3, LOW);   
  int red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //blue
  digitalWrite(s2, LOW);
  digitalWrite(s3, HIGH); 
  int blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //green
  digitalWrite(s2, HIGH);
  digitalWrite(s3, HIGH);   
  int green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  //white
  //digitalWrite(s2, HIGH);
  //digitalWrite(s3, LOW);  
  //white = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  
  Serial.print("RGB:"); Serial.print(red); Serial.print(","); Serial.print(green); Serial.print(","); Serial.println(blue);
}
