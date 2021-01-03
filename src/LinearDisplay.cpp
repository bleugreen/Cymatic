//
//  LinearDisplay.cpp
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//

#include "LinearDisplay.h"
/*
 From Display.h
 
 ofParameterGroup parameters;
 ofxGuiGroup *group;
 std::vector<utils::soundType> dataRequest;
 std::string name;
 int width, height;
*/


LinearDisplay::LinearDisplay(){
    name = "Chromatic";
}

//-------------------------------------------------------------------------------------
// interface methods
//-------------------------------------------------------------------------------------

void LinearDisplay::setup(){
    
    dataRequest = {utils::HPCP, utils::BARK };
    
}

//-------------------------------------------------------------------------------------
void LinearDisplay::setDimensions(int w, int h){
    width = w;
    height = h;
    
    halfW = (width*0.9);
    halfH =  (height*0.425);

    xOffset = h*0.05;
    yOffset = w*0.05;
}

//-------------------------------------------------------------------------------------
void LinearDisplay::buildGui(ofxGuiGroup *parent){
    group = parent->addGroup("Controls");
    
    group->add(smooth.set("Smooth", 3., 1., 10), ofJson({{"precision", 1}}));
    group->add(colorToggle.set("Color", true));
    
}

//-------------------------------------------------------------------------------------
void LinearDisplay::draw(){
    ofPushMatrix();
    ofTranslate(xOffset,yOffset);
    drawLinOctave(halfW, halfH);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(xOffset, halfH+(2*yOffset));
    drawLinScale(halfW, halfH);
    ofPopMatrix();
}

//-------------------------------------------------------------------------------------
void LinearDisplay::update(std::vector<utils::soundData> newData){
//    if(overtoneToggle) dataRequest[1] = utils::SMOOTH_SCALE_OT;
//    else dataRequest[1] = utils::SMOOTH_SCALE;
    
    for(utils::soundData container : newData){
        switch (container.label) {
            case utils::HPCP:
                if(octave.size() != container.data.size()){
                    octave.resize(container.data.size());
                }
                for(int i=0; i<container.data.size(); i++){
                    if(container.data[i] < 0.5) octave[i] = 0;
                    else{
                        octave[i] = utils::approxRollingAverage(octave[i], container.data[i], smooth);
                    }
                }
                break;
            case utils::BARK:
                //utils::normalizeVector(&container.data);
                if(scale.size() != container.data.size()){
                    scale.resize(container.data.size());
                    scale_max.resize(container.data.size());
                }
                for(int i=0; i<container.data.size(); i++){
                    scale[i] = utils::approxRollingAverage(scale[i], container.data[i], smooth);
                    if(container.data[i] > scale_max[i]) scale_max[i] = container.data[i];
                }
                break;
            default:
                break;
        }
    }
    

    
    
}


//-------------------------------------------------------------------------------------
// drawing
//-------------------------------------------------------------------------------------


// summed-octave
//--------------------------------------------------------------------------------------
void LinearDisplay::drawLinOctave(int w, int h){
    
    // Draw border
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofNoFill();
    ofRectangle outer_rect;
    outer_rect.x = 0;
    outer_rect.y = 0;
    outer_rect.width = w;
    outer_rect.height = h;
    ofDrawRectangle(outer_rect);
    std::string label = "Summed Octave";
    if(yOffset > 20) ofDrawBitmapString(label, 0, -8);
    ofPopStyle();
    
    if(octave.size() <= 1) return;
    
    // Initialize graph values
    //    Data = Summed Octave
    //    Bars take up 80% of total width
    //    Margins take up rest of total width
    //    Max bar height is 95% of height
    //    Labels are displayed if bar width > 12 pixels
    barWidth = (((float)w) * 0.8) / (float)octave.size();
    margin = (((float)w) - barWidth*octave.size()) / ((float)octave.size()+1);
    maxHeight = (-20)+((float)h)*0.95;
    y_offset = (float)(h + maxHeight)/2;
    bool labelsOn = (barWidth > 12);
    
    
    int x = 0;
    int noteNum = 0;
    int octaveNum = 0;
    int labelXOffset = max((barWidth-15)/2, 0); // labels are ~15px, so offset centers them
    int yPosLabel;
    
    ofPushMatrix();
    ofTranslate(margin, y_offset); //Move to bottom-left corner for start
    
    //loop through raw values
    for(int i=0; i<octave.size(); i++){
        
        ofPushStyle();
        
        ofRectangle rect;
        rect.x = x;
        rect.y = 0;
        rect.width = barWidth;

        // If rectangle height is below min. threshold, draw min rectangle
        // Note label follows rectangle if possible, otherwise sits on top of min rect.
        // y-axis is 'flipped' i.e. negative is upwards
        if(octave[i] < 0.05 || octave[i] > 1.0 || octave[i] != octave[i]) {
            rect.height =  -3;
            yPosLabel = -6;
        }
        else{
            rect.height = -octave[i]*maxHeight;
            yPosLabel = std::max((int)rect.height-6, (int)-maxHeight-10);
        }
        
        float hue, sat, brightness;
        if(colorToggle){
            hue = (i%12)*(255.0/12);
            sat = 100+octave[i%12]*155;
            brightness = 55+octave[i]*200;
        }
        else{
            hue = 0;
            sat = 0;
            brightness = octave[i]*255;
        }
        
        
        ofColor color = ofColor::fromHsb(hue, sat, brightness);
        ofSetColor(color);
        
        ofDrawRectangle(rect);
        ofPopStyle();
        
        // Draw note label
        if(labelsOn){
            ofSetColor(ofColor::white);
            ofDrawBitmapString(noteNames[noteNum], x+labelXOffset, yPosLabel);
        }
        
        // increment x position, note, and octave (if necessary)
        x += barWidth+margin;
        noteNum++;
        if(noteNum > 11){
            noteNum = 0;
        }
    }
    
    ofPopMatrix();
}


//--------------------------------------------------------------------------------------
// full-scale
//--------------------------------------------------------------------------------------
void LinearDisplay::drawLinScale(int w, int h){

    // Draw border
    ofPushStyle();
    ofSetColor(ofColor::white);
    ofNoFill();
    ofRectangle outer_rect;
    outer_rect.x = 0;
    outer_rect.y = 0;
    outer_rect.width = w;
    outer_rect.height = h;
    ofDrawRectangle(outer_rect);
    std::string label = "Bark Scale";
    if(yOffset > 20) ofDrawBitmapString(label, 0, -8);
    ofPopStyle();

    if(scale.size() <= 1) return;
    // Initialize graph values
    //    Data = Summed Octave
    //    Bars take up 80% of total width
    //    Margins take up rest of total width
    //    Max bar height is 95% of height
    //    No labels
    barWidth = (((float)w) * 0.85) / (float)scale.size();
    margin = (((float)w) - barWidth*scale.size()) / ((float)scale.size()-1);
    float edgeMargin = ((float)w - (barWidth*scale.size() + margin*(scale.size()-1))) / 2;
    maxHeight = ((float)h)*0.95;
    y_offset = (float)(h + maxHeight)/2;

    int x = 0;
    int noteNum = 0;
    int octaveNum = 2;

    ofPushMatrix();
    ofTranslate(edgeMargin, y_offset); //Move to bottom-left corner for start

    //loop through raw values
    for(int i=0; i<scale.size(); i++){

        ofPushStyle();

        ofRectangle rect;
        rect.x = x;
        rect.y = 0;
        rect.width = barWidth;

        // If rectangle height is below min. threshold, draw min rectangle
        // y-axis is 'flipped' i.e. negative is upwards
        float scaledVal = ofMap(scale[i], 0, scale_max[i], 0, 1);
        rect.height = -scaledVal*maxHeight;
    

        float hue, sat, brightness;
        if(colorToggle){
            hue = (i%12)*(255.0/12);
            sat = 100+scale[i%12]*155;
            brightness = 55+scale[i]*200;
        }
        else{
            hue = 0;
            sat = 0;
            brightness = scale[i]*255;
        }

        ofColor color = ofColor::fromHsb(hue, sat, brightness);
        ofSetColor(color);

        ofDrawRectangle(rect);
        ofPopStyle();

        // increment x position, note, and octave (if necessary)
        x += barWidth+margin;
    }

    ofPopMatrix();
}

