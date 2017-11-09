// Time interval between readings
const timeInterval = 100;

// Constants for exponential filter
const float expWeightingFactor = 0.07;

int expValue = 0;
bool expFirstValueFlag = true;
int exponentialFilter(int reading){
    //first value
    if (expFirstValueFlag == true){
        expValue = reading;
        expFirstValueFlag = false;
    }else{
        //deal with vaue wrapping
        if (reading < 90 && expValue > 270){
            expValue -= 360;
        }else if (reading > 270 && expValue < 90){
            expValue += 360;
        }
    expValue = expWeightingFactor*reading + (1 - expWeightingFactor)*expValue;
    }
    
    return (expValue + 360) % 360;
}


// Constants for average filter
const int averageFactor = 50;

int avgValue[averageFactor+1];
float lastAvg = 0;
bool avgFirstValueFlag = true;
int avgWrapValue = 0;
int averageFilter(int reading){
    //first value
    if (avgFirstValueFlag == true){
        for (int i = 0; i <= averageFactor; i++){
            avgValue[i] = reading;
        }
        lastAvg = reading;
        avgFirstValueFlag = false;
    }else{
        //shift values
        for (int i = averageFactor; i > 0; i--){
            avgValue[i] = avgValue[i-1];
        }
        avgValue[0] = reading;
    }

    //deal with wrapping
    if (reading < 90 && lastAvg > 270){
        lastAvg -= 360;
    }else if (reading > 270 && lastAvg < 90){
        lastAvg += 360;
    }

    //keep track of whether reading has wrapped further than tail of average
    if (reading < 90 && avgValue[1] > 270){
        avgWrapValue -= 1;
    }else if (reading > 270 && avgValue[1] < 90){
        avgWrapValue += 1;
    }

    //calculate average
    lastAvg += (float)reading/averageFactor - ((float)avgValue[averageFactor]+360*avgWrapValue)/averageFactor;

    //track tail wrap
    if (avgValue[averageFactor-1] < 90 && avgValue[averageFactor] > 270){
        avgWrapValue += 1;
    }else if (avgValue[averageFactor-1] > 270 && avgValue[averageFactor] < 90){
        avgWrapValue -= 1;
    }

    return ((int) lastAvg + 360) % 360;
}


// Constants for triangular filter
const int triFactor = 20;

int triValue[triFactor+1];
bool triFirstValueFlag = true;
long triDenominator = triFactor*(triFactor+1)/2;
long triSum, triNumerator;
int triWrapValue = 0;
int triangularFilter(int reading){
    //first value
    if (triFirstValueFlag == true){
        for (int i = 0; i <= triFactor; i++){
            triValue[i] = reading;
        }
        triFirstValueFlag = false;
        triSum = reading*triFactor;
        triNumerator = reading*triDenominator;
    }else{
        //shift values
        for (int i = triFactor; i > 0; i--){
            triValue[i] = triValue[i-1];
        }
        triValue[0] = reading;
    }

    //keep track of whether reading has wrapped further than tail of average
    if (reading < 90 && triValue[1] > 270){
        triWrapValue -= 1;
        triNumerator -= 360*triDenominator;
        triSum -= 360*triFactor;
    }else if (reading > 270 && triValue[1] < 90){
        triWrapValue += 1;
        triNumerator += 360*triDenominator;
        triSum += 360*triFactor;
    }

    //calculate numerator
    triNumerator += triFactor*reading - triSum;

    triSum += reading - (triValue[triFactor] + 360*triWrapValue);

    //track tail wrap
    if (triValue[triFactor-1] < 90 && triValue[triFactor] > 270){
        triWrapValue += 1;
    }else if (triValue[triFactor-1] > 270 && triValue[triFactor] < 90){
        triWrapValue -= 1;
    }

    //Serial.print(triNumerator);Serial.print(" ");

    return (triNumerator/triDenominator+360)%360;
}


//Constants for median filter
const int medFactor = 50;

int medValue[medFactor+1];
int medValueSorted[medFactor+1];
bool medFirstValueFlag = true;

int medianFilter(int reading){
    if (medFirstValueFlag){
        for (int i = 0; i < medFactor; i++){
            medValue[i] = reading;
            medValueSorted[i] = reading;
        }
        
        medFirstValueFlag = false;
    }

    // wrap data
    if (reading < 90 && medValue[0] > 270){
        for (int i = 0; i < medFactor; i++){
            medValue[i] = medValue[i] - 360;
            medValueSorted[i] = medValueSorted[i] - 360;
        }
    }else if (reading > 270 && medValue[0] < 90){
        for (int i = 0; i < medFactor; i++){
            medValue[i] = medValue[i] + 360;
            medValueSorted[i] = medValueSorted[i] + 360;
        }
    }

    //remove last value
    for (int i = 0; i < medFactor-1; i++){
        if (medValueSorted[i] >= medValue[medFactor-1]){
            medValueSorted[i] = medValueSorted[i+1];
        }
    }

    //insert new value
    int i = medFactor-1;
    while (i > 0 && medValueSorted[i-1] > reading){
        medValueSorted[i] = medValueSorted[i-1];
        i--;
    }
    medValueSorted[i] = reading;

    //shift chronological values
    for (int i = medFactor-1; i > 0; i--){
        medValue[i] = medValue[i-1];
    }
    medValue[0] = reading;
    
    return (medValueSorted[int(medFactor/2)] +360) % 360;
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial.println("Start");
    //So we'll have the same data each time
    randomSeed(4);
}

int x = 0;
unsigned long previousMillis = 0;
void loop() {
    // put your main code here, to run repeatedly:
    int reading = getData();

    if ((millis() - previousMillis) > timeInterval){
        Serial.println(medianFilter(exponentialFilter(reading)));
        x+=1;
        
        if (x == 3000){
            Serial.println("5 minutes");
        }

        previousMillis = millis();
    }
}
