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
const int averageFactor = 10;

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
const int triFactor = 50;

int triValue[triFactor+1];
int lastTriAvg = 0;
bool triFirstValueFlag = true;
int triWrapValue = 0;
int triDenominator = triFactor*(triFactor+1)/2;
int triSum;
int triangularFilter(int reading){
    //first value
    if (triFirstValueFlag == true){
        for (int i = 0; i <= triFactor; i++){
            triValue[i] = reading;
        }
        lastTriAvg = reading;
        triFirstValueFlag = false;
        triSum = reading*triFactor;
    }else{
        //shift values
        for (int i = triFactor; i > 0; i--){
            triValue[i] = triValue[i-1];
        }
        triValue[0] = reading;
    }

    //deal with wrapping
    if (reading < 90 && lastTriAvg > 270){
        lastTriAvg -= 360;
    }else if (reading > 270 && lastTriAvg < 90){
        lastTriAvg += 360;
    }

    //deal with wrapping
    if (reading < 90 && triSum/triFactor > 270){
        triSum -= 360*triFactor;
    }else if (reading > 270 && triSum/triFactor < 90){
        triSum += 360*triFactor;
        //Serial.print(triSum/triFactor);Serial.println("YEAAAHHH");
    }

    //keep track of whether reading has wrapped further than tail of average
    if (reading < 90 && triValue[1] > 270){
        triWrapValue -= 1;
    }else if (reading > 270 && triValue[1] < 90){
        triWrapValue += 1;
        //Serial.print("aaaaahhhh1");Serial.print(reading);
    }

    //calculate average
    lastTriAvg += triFactor*reading/triDenominator - triSum/triDenominator;
    lastTriAvg = (lastTriAvg + 360) % 360;

    triSum += reading - (triValue[triFactor] + 360*triWrapValue);

    //Serial.print(triWrapValue);Serial.print("   ");

    //track tail wrap
    if (triValue[triFactor-1] < 90 && triValue[triFactor] > 270){
        triWrapValue += 1;
        //Serial.print("aaaaahhhh2");
    }else if (triValue[triFactor-1] > 270 && triValue[triFactor] < 90){
        triWrapValue -= 1;
    }

    return lastTriAvg;
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
    
    Serial.println(averageFilter(reading));
    delay(100);
    x+=1;
    
    if (x == 3000){
        Serial.println("5 minutes");
    }
}

