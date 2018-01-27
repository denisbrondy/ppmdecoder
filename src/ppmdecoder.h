
#ifndef ppmdecoder_f
#define ppmdecoder_f

const int FORWARD = 0;
const int REVERSE = 1;

class PpmDecoder {
    public:
        PpmDecoder(int pPin, int pNbrChannel);
        void setChannelOutput(int pChannel, int pMin, int pMax, int pWay);
        int getChannelValue(int pChannel);
    private:
        int* _channelsMins;
        int* _channelsMaxs;
        int* _channelsWays;
};

#endif