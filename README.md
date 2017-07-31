# Good Life Inc

About: With this project, the aim is to fill a crucial gap in currently existing quantified self-measurement devices: the lack of predictive systems which provide useful insights, over and above merely displaying the data collected. This system here is also highly comprehensive, and due to the sheer range of data it collects, can allow for correlations to be drawn from all kinds of factors to an individual’s stress levels, while initially starting from heart rate, and improving over time.
1. The arduino code compiles on the hardware based system to run the sensors.
2. Readings are taken from the sensor and uploaded automatically in ThinkSpeak 
3. The data is retrieved from think speak in a CSV file 
4. Analytics is performed in the data obtained using a decision tree classifier.
5. The analytics system is connected to the IOT based hardware module using an Ethernet module.

In addition to the two uploaded codes, there is an android application as well that was developed for the sole purpose of retrieving the stored data from ThingSpeak Channel and providing easy access of data and analysis on a mobile platform.
