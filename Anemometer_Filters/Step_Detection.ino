// Constants for simple step detection
const int stepDistance = 5;

int lastValue = 0;

int stepDetect(int value){
    if (abs(value - lastValue) > stepDistance){
        lastValue = value;
    }

    return lastValue;
}

