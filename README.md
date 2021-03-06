# Anemometer Filter

Various ideas for filters for data from UW Sailbot's anemometer

## Simulated Data Generation

The simulated data is comprised of the following three components. Each component can be modified by editing it's corresponding constants at the top of **Simulated_Data.ino**. Data points are generated at a rate of 10 ticks per second.

### 1. Smooth Data

The smooth data is intended to represent the true direction of the wind relative to the boat. It drifts slowly at a rate of up to **maxVel** with that rate changing by up to plus or minus **velChange** per tick. 

Every tick there is a one in **accelChance** of there being a large shift in the wind's direction. These shifts are intended to simulate a gust or change in the boat's heading. If that's the case the second derivative of the graph will increase to up to **accelRate**. The magnitude of this second derivative will decrease by up to **accelDecrease** every tick until it reaches zero. At this point the magnitude of the first derivative will decrease up to **velDecrease** every tick until it is within the normal range defined by **maxVel**.

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Smooth%20Data.png "Graph of Spikes")

### 2. Noise

The noise is meant to simulate vibrations in the anemometer or other small variations due to issues with the measuring devices. The noise level changes by up to **noiseChange** every tick, to a maximum of plus or minus **maxNoise**.

The data shown in the graph has been translated up by ten units. Normally the noise is centered around zero.

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Noise.png "Graph of Noise") 

### 3. Spikes

Spikes in data are intended to simulate the boat hitting a wave, or any other event that causes a brief shift in data after which the data returns to it's previous reading.

Spikes have a 1 in **spikeChance** of occurring each tick. When a spike occurs the reading will initially increase by **spikeMaxVel** per tick. This rate will decrease by up to **spikeVelDecrease** per tick. Once the rate has reached zero the data will return to it's previous reading, decreasing by up to **spikeDecrease** every tick until this has been reached.

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Spikes.png "Graph of Spikes")

This data is all summed to produce the full simulated data as shown. An efficient filter should be able to take this simulated data and produce something resembling the smooth data it was based on.


## Filter 

### Exponential Moving Average

Graphs of the data passed through the exponential filter are shown below. The filter can be adjusted by changing the value of **expWeightingFactor**. A higher value favors more recent data while a lower value more heavily weights older data.

#### Exponential Filter with Weighting Factor = 0.07

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Exponential%20Filter%2007.png "Graph of Exponential Filtered Data Weighting Factor 0.07")

#### Exponential Filter with Weighting Factor = 0.02

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Exponential%20Filter%2002.png "Graph of Exponential Filtered Data Weighting Factor 0.02")

### Simple Moving Average

Graphs of the data passed through the average filter are shown below. The filter simply takes the mean of the past **averageFactor** data points.

#### Average Filter with Factor = 10

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Average%20Filter%2010.png "Graph of Average Filtered Data Factor 10")

#### Average Filter with Factor = 50

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Average%20Filter%2050.png "Graph of Average Filtered Data Factor 50")

### Triangular Moving Average

Graphs of the data passed through the average filter are shown below. The filter takes a weighted average of the past **averageFactor** data points with more recent points weighted more heavily.

#### Triangular Filter with Factor = 20

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Triangular%20Filter%2020.png "Graph of Triangular Filtered Data Factor 20")

#### Average Filter with Factor = 50

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Triangular%20Filter%2050.png "Graph of Triangular Filtered Data Factor 50")

### Median Filter

Graphs of the data passed through the median filter are shown below. The filter takes the median of the past **medFactor** data points. 

#### Median Filter with Factor = 25

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Median%20Filter%2025.png "Graph of Median Filtered Data Factor 25")

#### Median Filter with Factor = 50

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Median%20Filter%2050.png "Graph of Median Filtered Data Factor 50")

## Step Detection

In order to prevent publishing new values ten times per second unnecessarily I added a very simple step detector. The function checks if the current value is further than **stepDistance** from the last published value. Currently the function constantly outputs the value in order to properly graph and visualize the function, but in implementation should be changed to a predicate used to decide when to publish a value.

#### Step Detection with Average Filter, Step Distance = 5, Average Factor = 50

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Step%20Detect%205%20Average%2050.png "Graph of Step Detect Average Filtered Data")

#### Step Detection with Exponential Filter, Step Distance = 5, Exponential Factor = 50

![alt text](https://raw.githubusercontent.com/benajmin/anemometer-filters/master/Graphs/Step%20Detect%205%20Exp%2007.png "Graph of Step Detect Exponential Filtered Data")

## Conclusions

Based on the simulated data, an average filter with a factor if 50 put through the step detector gives the best results. However, this comes at a cost of complexity and higher memory usage. The exponential filter with a factor of 0.02 gives comparable results while using a fraction of the memory. As such I believe the exponential filter should be used in conjunction with simple step detection. Various constants used will have to be fine tuned with real data.
