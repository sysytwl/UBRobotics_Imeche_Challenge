//pin for highC not working, convinced its a drv8323s problem unique to the IC 🤷‍♂️ investigating on Monday, swapping drivers
//Going to make a new one ✅ Done, works like a charm

const int highA = 2;
const int lowA = 3;
const int highB = 4; 
const int lowB = 5;
const int highC = 6;
const int lowC = 7;

const int ENB = 8;

int i = 1;

void setup() {
  // put your setup code here, to run once:
  pinMode(highA, OUTPUT);
  pinMode(lowA, OUTPUT);
  pinMode(highB, OUTPUT);
  pinMode(lowB, OUTPUT);
  pinMode(highC, OUTPUT);
  pinMode(lowC, OUTPUT);

  pinMode(ENB, OUTPUT);

  digitalWrite(highA, LOW);
  digitalWrite(lowA, LOW);
  digitalWrite(highB, LOW);
  digitalWrite(lowB, LOW);
  digitalWrite(highC, LOW);
  digitalWrite(lowC, LOW);

  digitalWrite(ENB, HIGH);
}
int c =1000000;

void loop() {
  // put your main code here, to run repeatedly:
  //6 SECTOR
  
  Commutate(i);
  i++;
  
  if (i==6) i = 1;
  
  delayMicroseconds(100+c);
  
  if (c>1) c-=10;
}

void Commutate(int sector){
  switch (sector) {
    case 1:
      digitalWrite(highA, HIGH);
      digitalWrite(lowA, LOW);
      digitalWrite(highB, LOW);
      digitalWrite(lowB, HIGH);
      digitalWrite(highC, LOW);
      digitalWrite(lowC, LOW);
      break;

    case 2:
      digitalWrite(highA, HIGH);
      digitalWrite(lowA, LOW);
      digitalWrite(highB, LOW);
      digitalWrite(lowB, LOW);
      digitalWrite(highC, LOW);
      digitalWrite(lowC, HIGH);
      break;

    case 3:
      digitalWrite(highA, LOW);
      digitalWrite(lowA, LOW);
      digitalWrite(highB, HIGH);
      digitalWrite(lowB, LOW);
      digitalWrite(highC, LOW);
      digitalWrite(lowC, HIGH);

      break;

    case 4:
      digitalWrite(highA, LOW);
      digitalWrite(lowA, HIGH);
      digitalWrite(highB, HIGH);
      digitalWrite(lowB, LOW);
      digitalWrite(highC, LOW);
      digitalWrite(lowC, LOW);

      break;

    case 5:
      digitalWrite(highA, LOW);
      digitalWrite(lowA, HIGH);
      digitalWrite(highB, LOW);
      digitalWrite(lowB, LOW);
      digitalWrite(highC, HIGH);
      digitalWrite(lowC, LOW);

      break;

    case 6:
      digitalWrite(highA, LOW);
      digitalWrite(lowA, LOW);
      digitalWrite(highB, LOW);
      digitalWrite(lowB, HIGH);
      digitalWrite(highC, HIGH);
      digitalWrite(lowC, LOW);

      break;

    default:
      break;
  }
}
