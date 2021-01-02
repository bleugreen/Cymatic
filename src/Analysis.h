//
//  Analysis.h
//  soundProfiler
//
//  Created by Mitch on 12/27/20.
//



#ifndef Analysis_h
#define Analysis_h

#include "ofxFft.h"
#include "utils.h"
//#include "CQParameters.h"
//#include "Chromagram.h"

class Analysis
{
    public:
        Analysis();
        void init(int bufSize);
    
        // per-frame operations
        void analyzeFrame(std::vector<float> sample, int bufferSize);
        void analyzeFrameFft(std::vector<float> sample, int bufferSize);
        void analyzeFrameQ(std::vector<float> sample, int bufferSize);
        bool smoothFrame();
    
        
    
        // getters
        bool isFrameReady();
    
        std::vector<float> getData(utils::soundType type);
        int getSize(utils::soundType type);
    
        // setters
        void setAddOvertone(bool b);
        
        
    private:
        ofxFft* fft;
//        Chromagram* chrom;
    
        bool frameReady, addOvertone;
        
        int bufferSize, fft_size , oct_size, scale_size;
        
        
        std::vector<float> raw_fft;
        std::vector<float> raw_octave;
        std::vector<float> raw_scale;
        
        std::vector<float> smooth_octave;
        std::vector<float> smooth_scale;
        std::vector<float> smooth_scale_ot;

        std::vector<float> in_fft;
        std::vector<float> in_octave;
        std::vector<float> in_scale;
    
    
        std::vector<int> fullBinList;
        std::vector<float> freqlist;
    
        // constants
        const float a4 = 440;
        std::vector<float> chromaticScale = {440, 466.16, 493.88, 523.25, 554.37, 587.33, 622.25, 659.26, 698.46, 739.99, 783.99, 830.61};
    
        bool sendToFft{};
    
};

#endif /* Analysis_h */
