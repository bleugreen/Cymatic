//
//  Display.h
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//
#ifndef Display_h
#define Display_h

#include "ofxGuiExtended.h"
#include "utils.h"

class Display{
public:
    virtual void draw() = 0;
    virtual void update(std::vector<utils::soundData> newData) = 0;
    virtual void setup() = 0;
    virtual void setDimensions(int w, int h) = 0;
    virtual void buildGui(ofxGuiGroup* parent) = 0;
    
    std::string name;
    ofParameterGroup parameters;
    ofxGuiGroup* group;
    
    std::vector<utils::soundType> dataRequest;
    
protected:
    float width, height;
};

#endif /* Display_h */
