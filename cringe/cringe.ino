//pin for highC not working, conviced its a drv8323 problem unique to the IC
//Going to make a new one

const int highA = 14;
const int lowA = 27;
const int highB = 26; 
const int lowB = 25;
const int highC = 35; //35
const int lowC = 32;

const int ENB = 13;

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

void loop() {
  // put your main code here, to run repeatedly:
  //6 SECTOR
  if (i != 7){
    Commutate(i);
    i++;
  } else {
    i = 1;
   } 

   delay(1);
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
