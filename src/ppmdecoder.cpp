#include "Arduino.h"
#include "ppmdecoder.h"

const int PPM_MIN = 700;
const int PPM_MAX = 1500;
const long PPM_SYNCH = 10000;

long lastTime;
int pin;
int channelNumber;

volatile boolean inSynch = false;
volatile int channelCounter = 0;
volatile int* channelsPulses;
volatile long* channelsStartTimes;


void readPPM() {
    long currentTime = micros();
    if (currentTime - lastTime > PPM_SYNCH) {
        inSynch = true;
        channelCounter = 0;
    } else if (inSynch == true) {
        int a = digitalRead(pin);
        if (a) {
            channelsStartTimes[channelCounter] = currentTime;
        } else {
            channelsPulses[channelCounter] = currentTime - channelsStartTimes[channelCounter];
            channelCounter++;
        }
        if (channelCounter == channelNumber) {
            inSynch = false;
        }
    }
    lastTime = currentTime;
}

PpmDecoder::PpmDecoder(int pPin, int pNbrChannel) {
    pin = pPin;
    channelNumber = pNbrChannel;
    channelsPulses = new int[channelNumber];
    channelsStartTimes = new long[channelNumber];
    _channelsMins = new int[channelNumber];
    _channelsMaxs = new int[channelNumber];
    _channelsWays = new int[channelNumber];
    for (int i = 1 ; i < channelNumber + 1 ; i++) {
        setChannelOutput(i, PPM_MIN, PPM_MAX, FORWARD);
        channelsPulses[i - 1] = PPM_MIN;
    }
    pinMode(pin, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(pin), readPPM, CHANGE);
}

void PpmDecoder::setChannelOutput(int pChannel, int pMin, int pMax, int pWay) {
    if (pChannel > 0 and pChannel < channelNumber + 1) {
        _channelsMins[pChannel-1] = pMin;
        _channelsMaxs[pChannel-1] = pMax;
        _channelsWays[pChannel-1] = pWay;
    }
}

int PpmDecoder::getChannelValue(int pChannel) {
    if (pChannel > 0 and pChannel < channelNumber + 1) {
        if (channelsPulses[pChannel-1] > PPM_MAX or channelsPulses[pChannel-1] < PPM_MIN) { // In case signal is lost
            if (_channelsWays[pChannel-1] == FORWARD) {
                channelsPulses[pChannel-1] = PPM_MIN;
            } else {
                channelsPulses[pChannel-1] = PPM_MAX;
            }
        }
        noInterrupts(); // get safely the pulse value
        int pulse = channelsPulses[pChannel-1];
        interrupts();
        if (_channelsWays[pChannel-1] == FORWARD) {
            return map(pulse, PPM_MIN, PPM_MAX, _channelsMins[pChannel-1], _channelsMaxs[pChannel-1]);
        } else {
            return map(pulse, PPM_MIN, PPM_MAX, _channelsMaxs[pChannel-1], _channelsMins[pChannel-1]);
        }
    } else {
        return 0;
    }
}



