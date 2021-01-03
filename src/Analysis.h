//
//  Analysis.h
//  soundProfiler
//
//  Created by Mitch on 12/27/20.
//



#ifndef Analysis_h
#define Analysis_h


#include "utils.h"
#include "ofMain.h"
#include "ofxAudioAnalyzer.h"


class Analysis
{
    public:
        Analysis();
        void init(int sampleRate, int bufferSizeIn, int channelsIn);
    
        // per-frame operations
        void analyzeFrame(ofSoundBuffer& buffer);

        bool smoothFrame();
    
        
    
        // getters
        bool isFrameReady();
    
        std::vector<float> getData(utils::soundType type);
        int getSize(utils::soundType type);
    
        // setters
        void setAddOvertone(bool b);
        
        
    private:
//        ofxFft* fft;
//        Chromagram* chrom;
        ofxAudioAnalyzer analyzer;
    
        bool frameReady, addOvertone;
        
        int sampleRate, bufferSize, channels;
    
        bool sendToFft{};
    
};

#endif /* Analysis_h */
