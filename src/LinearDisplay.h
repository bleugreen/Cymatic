//
//  LinearDisplay.h
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//
#ifndef LinearDisplay_h
#define LinearDisplay_h

#include "Display.h"

class LinearDisplay : public Display {
    
public:
    LinearDisplay();
    
    // interface methods
    void setup();
    void setDimensions(int w, int h);
    void buildGui(ofxGuiGroup* parent);
    void draw();
    void update(std::vector<utils::soundData> newData);
    
    
protected:
    
    // drawing methods
    void drawLinOctave(int w, int h);
    void drawLinScale(int w, int h);
    
    // gui parameters
    ofParameter<bool> overtoneToggle;
    ofParameter<bool> colorToggle;
    
    // local audio data
    std::vector<float> octave;
    std::vector<float> scale;
    
    // general drawing variables
    float halfW, halfH, xOffset, yOffset;
    int barWidth, margin, maxHeight, y_offset;
    std::vector<std::string> noteNames = {"A", "A#","B", "C", "C#", "D", "D#", "E", "F", "F#", "G", "G#"};
    
};

#endif /* LinearDisplay_h */
