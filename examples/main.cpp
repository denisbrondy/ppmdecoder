#include <Arduino.h>
#include "ppmdecoder.h"

PpmDecoder* ppm;

void setup() {
    Serial.begin(115200);
    ppm = new PpmDecoder(D5, 5);
    ppm->setChannelOutput(1, 0, 255, FORWARD);
    ppm->setChannelOutput(3, 600, 1200, REVERSE);
}

void loop() {
    delay(1000);
    Serial.print("Channel 1 : ");Serial.println(ppm->getChannelValue(1));
    Serial.print("Channel 2 : ");Serial.println(ppm->getChannelValue(2));
    Serial.print("Channel 3 : ");Serial.println(ppm->getChannelValue(3));
    Serial.print("Channel 4 : ");Serial.println(ppm->getChannelValue(4));
    Serial.print("Channel 5 : ");Serial.println(ppm->getChannelValue(5));
}