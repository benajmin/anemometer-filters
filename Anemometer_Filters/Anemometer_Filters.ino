// Constants for exponential filter
float expWeightingFactor = 0.3;

int expValue = 0;
bool expFirstValueFlag = true;
int exponentialFilter(int reading){
  //first value
  if (expFirstValueFlag = true){
    expValue = reading;
    expFirstValueFlag = false;
  }else{
    expValue = expWeightingFactor*reading + (1 - expWeightingFactor)*expValue;
  }
  return expValue;
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Start");
  //So we'll have the same data each time
  randomSeed(4);
}

int x = 0;
void loop() {
  // put your main code here, to run repeatedly:
  int reading = getData();

  Serial.println(exponentialFilter(reading));
  delay(100);
  x+=1;

  if (x == 3000){
    Serial.println("5 minutes");
  }
}

