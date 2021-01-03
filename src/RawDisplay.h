//
//  RawDisplay.h
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//

#ifndef RawDisplay_h
#define RawDisplay_h

#include "Display.h"
#include "ofxGuiExtended.h"
#include <mutex>


class RawDisplay : public Display {
    
public:
    RawDisplay();
    
    // interface methods
    void setup();
    void draw();
    void setDimensions(int w, int h);
    void buildGui(ofxGuiGroup* parent);
    void update(std::vector<utils::soundData> newData);
    
protected:
    
    // drawing methods
    void drawFftPlot(int w, int h);
    void drawFftWindow(float w, float h);
    void drawGridLines(float w, float h);
    
    void drawSpectrogram(int w, int h);
    ofImage spectImg, spectImg2;
    int spectTimer;
    
    // local audio data
    std::vector<float> raw_fft;
    std::vector<float> smooth_fft;
    std::vector<float> fft_display;
    std::vector<float> fft_display_freqs;
    
    // gui parameters
    ofxGuiGroup *linLogToggles, *windowGroup;
    ofParameterGroup linLogParameters;
    ofParameter<bool> lin, log;
    ofParameter<bool> rescale;
    ofParameter<float> freqStart;
    ofParameter<float> freqWidth;
    ofParameter<float> smooth;
    ofParameter<void> reset;
    ofParameter<float> numLines;
    
    ofParameter<bool> gradient;
    
    // gui listeners
    void resetParameters();
    void fftWindowChanged(float& val);
    void setRawLinLog(int& index);
    
    // fft window 
    int startBin;
    int endBin;
    

    float freqEnd;
    float numLabels;
    bool lineB;
    bool labelB;
    
    float avg;
    
    
    float halfW;
    float halfH;
    float xOffset;
    float yOffset;

    
    float size;
    
    
    
    std::mutex mtx;
};

#endif /* RawDisplay_h */
