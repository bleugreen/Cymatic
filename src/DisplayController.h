//
//  DisplayController.h
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//


#pragma once

#include "Analysis.h"
#include "LinearDisplay.h"
#include "RawDisplay.h"
//#include "OscDisplay.h"

class DisplayController{
    
public:
    DisplayController();
    
    // setup
    void setup(Analysis* a, int w, int h, ofxGuiGroup* all);
    
    // general control
    void draw();
    void update();
    void updateLayout(int w, int h);
    void minimize();
    void maximize();
    
    // mode selection
    void setMode(int index);
    int getMode();
    void incMode();
    
    ofParameterGroup modeSelector;
    
    ofxGuiGroup *modeSelectorGroup; // add mode buttons
    
    ofxGuiGroup *modeControlGroup; // add all mode-specific parameters, only show current mode
    
protected:
    void setDisplayMode(int& index);
    
    std::vector<std::shared_ptr<Display>> modes;
    std::vector<ofxGuiGroup*> modeControls;
    
    std::shared_ptr<LinearDisplay> ld;
    std::shared_ptr<RawDisplay> rd;
    //std::shared_ptr<OscDisplay> od;
    
    Analysis* analysis;
    
    ofParameter<bool> disp0, disp1, disp2;
    
    bool ready{};
    int width;
    int height;
    int current_mode;
}; // DisplayController.h



