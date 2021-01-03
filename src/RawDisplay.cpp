//
//  RawDisplay.cpp
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//

#include <stdio.h>
#include "RawDisplay.h"

RawDisplay::RawDisplay(){
    name = "Frequency";
}

void RawDisplay::setup(){
    dataRequest.push_back(utils::FFT);

    numLabels = 5;
    numLines = 20;
    startBin = 0;
    endBin = 1025;
    freqEnd = 22050;
    
    spectTimer = 0;
    
    spectImg.allocate(300, 480, OF_IMAGE_COLOR);
    spectImg2.allocate(300, 480, OF_IMAGE_COLOR);
}

void RawDisplay::buildGui(ofxGuiGroup* parent){
    group = parent->addGroup("Raw Controls");
    group->setShowHeader(false);
    
    linLogParameters.setName("X-Axis Scale");
    linLogParameters.add(lin.set("Linear", false));
    linLogParameters.add(log.set("Logarithmic", false));
    
    
    linLogToggles = group->addGroup(linLogParameters);
    linLogToggles->setExclusiveToggles(true);
    linLogToggles->setConfig(ofJson({{"type", "radio"}}));
    //linLogToggles->getActiveToggleIndex().addListener(this, &RawDisplay::setRawLinLog);
    linLogToggles->setActiveToggle(0);
    

    windowGroup = group->addGroup("FFT Window");
    freqStart.set("Window Start", 0, 0, 22050);
    freqWidth.set("Window Width", 22050, 1000, 22050);
    smooth.set("Smoothing", 3., 1., 5.);
    numLines.set("Number of Gridlines", 20, 1, 50);
    windowGroup->add<ofxGuiFloatSlider>(freqWidth, ofJson({{"precision", 0}}));
    windowGroup->add<ofxGuiFloatSlider>(freqStart, ofJson({{"precision", 0}}));
    windowGroup->add<ofxGuiFloatSlider>(numLines, ofJson({{"precision", 0}}));
    windowGroup->add<ofxGuiFloatSlider>(smooth, ofJson({{"precision", 1}}));
    windowGroup->add(rescale.set("Rescale Window", false));
    windowGroup->add(gradient.set("Gradient Fill", true));

    
    windowGroup->add(reset.set("Reset Settings"), ofJson({{"type", "fullsize"}, {"text-align", "center"}}));
    reset.addListener(this, &RawDisplay::resetParameters);
    freqWidth.addListener(this, &RawDisplay::fftWindowChanged);
    freqStart.addListener(this, &RawDisplay::fftWindowChanged);
    
}

void RawDisplay::fftWindowChanged(float& val){
    float binWidth = (freqWidth*raw_fft.size())/22050.;
        
    startBin = (freqStart*raw_fft.size())/22050.;
    endBin = startBin+binWidth;
    
    freqEnd = freqStart+freqWidth;
    
    float start_max = 22050 - freqWidth;
    if(freqStart > start_max){
        freqStart.set(start_max);
    }
    freqStart.setMax(start_max);
}

void RawDisplay::resetParameters(){
    freqStart.set(0);
    freqWidth.set(22050);
    smooth.set(3.0);
}


void RawDisplay::draw(){
    ofPushStyle();
    ofPushMatrix();
    ofTranslate(xOffset,yOffset);
    drawFftPlot(halfW, halfH);
    ofPopMatrix();
    
    ofPushMatrix();
    ofTranslate(xOffset,2*yOffset+halfH);
    drawSpectrogram(halfW, halfH);
    ofPopMatrix();
    
    ofPopStyle();

}


void RawDisplay::update(std::vector<utils::soundData> newData){
    
    for(utils::soundData container : newData){
        switch (container.label) {
            case utils::FFT:
                raw_fft = container.data;
                smooth_fft.resize(raw_fft.size());
                
                break;

            default:
                break;
        }
    }
    utils::normalizeVector(&raw_fft);
    
    
    avg = 0;
    for(int i=0; i<raw_fft.size(); i++){
        smooth_fft[i] = utils::approxRollingAverage(smooth_fft[i], raw_fft[i], smooth);
        avg += raw_fft[i];
    }
    avg /= raw_fft.size();
    
    
    std::lock_guard<std::mutex> guard(mtx);
    size = endBin - startBin;
    fft_display.clear();
    fft_display_freqs.clear();
    // Update display data
    for(int i=startBin; i<endBin; i++){
        if(i < 0 || i >= smooth_fft.size()){
            fft_display.push_back(0);
        }
        else{
            fft_display.push_back(smooth_fft[i]);
        }
    }
    
    if(rescale){
        float max = 0;
        for(float val : raw_fft){
            if(val > max) max = val;
        }
        if(max != 0){
            for(int i=0; i<raw_fft.size(); i++){
                raw_fft[i] /= max;
            }
        }
    }
    

    
    
}
void RawDisplay::setDimensions(int w, int h){
    width = w;
    height = h;
    
    halfW = ((float)width*0.9);
    halfH =  ((float)height*0.425);

    xOffset = width*0.05;
    yOffset = height*0.05;
    
    
}


void RawDisplay::drawFftPlot(int w, int h){
    if(fft_display.size() <= 1) return;
    
    
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
    
    std::string label = "FFT Plot";
    if(yOffset > 20) ofDrawBitmapString(label, 0, -8);
    
    ofPopStyle();
    
    ofPushMatrix();
    ofTranslate(w*0.025, h*0.05);
    drawFftWindow(w*0.95, h*0.9);
    ofPopMatrix();
    
}


void RawDisplay::drawFftWindow(float w, float h){
    ofPushMatrix();
    ofTranslate(0, h);
    
    drawGridLines(w, h);
    
    ofPath plot;
    plot.moveTo(0, 0);
    plot.lineTo(0, 0);
    

    float x,y;
    
    std::lock_guard<std::mutex> guard(mtx);
    
    x = 0;
    for(int i=0; i<size; i++){
        if(lin){
            x = (((float)i) / size) * w;
        }
        else{
            float lin_x = (i*w) / (size-1);
            x = w*(logf(lin_x+1)/logf(w+1));
        }
        
        y = -(h * min(fft_display[i], (float)0.95));
        plot.curveTo(x, y);
    }
    
    plot.lineTo(w, 0);
    plot.close();
    
    plot.setPolyWindingMode(OF_POLY_WINDING_ODD);
    if(gradient && y == y){
        ofMesh mesh = plot.getTessellation();
        std::vector<ofColor_<float>> colors;
        for(glm::vec<3, float, glm::packed_highp> vtx : mesh.getVertices()){
            float hue = 128+(abs(vtx.y) / h)*128;
            float brightness = 50 + (abs(vtx.y) / h)*205;
            
            colors.push_back(ofColor::fromHsb(hue, brightness, 200));
        }
        mesh.addColors(colors);
        mesh.draw(OF_MESH_FILL);
    }
    else{
        plot.setFilled(false);
        plot.setStrokeWidth(1);
        plot.setStrokeColor(ofColor::white);
        plot.draw();
    }
    
    ofPopMatrix();
}

void RawDisplay::drawGridLines(float w, float h){
    // Draw lines
    float x = 0;
    float freq, labelX;
    float prevLabelEnd = -1;
    
    for(int i=0; i<numLines; i++){
        freq = freqStart + ((freqEnd - freqStart)*i)/(numLines-1);
        std::string label = utils::formatFreq(freq);
        int width = utils::getBitmapStringWidth(label);
        
        if(lin){
            x = (i*w)/(numLines);
        }
        else{
            float lin_x =(i*w)/(numLines-1);
            x = w*(logf(lin_x+1)/logf(w+1));
        }
        
        if(i==0){
            ofSetColor(70);
            ofDrawLine(x, 0, x, -(h-5));
            
            ofSetColor(100);
            ofDrawBitmapString(label, 0, -h);
            prevLabelEnd = width;
        }
        else{
            if(i%2 == 0){
                labelX = x-(width/2);
                
                ofSetColor(70);
                ofDrawLine(x, 0, x, -(h-5));
                
                ofSetColor(100);
                if(labelX > prevLabelEnd) {
                    ofDrawBitmapString(label, labelX, -h);
                    prevLabelEnd = labelX+width;
                }
            }
            else{
                ofSetColor(50);
                ofDrawLine(x, 0, x, -(h*0.95));
            }
        }
    }
}

void RawDisplay::drawSpectrogram(int w, int h){
//    if(spectTimer >= 30){
        spectTimer = 0;
    
        // update image
        spectImg2.setFromPixels(spectImg.getPixels());
        
        
        // push old data left
        for(int x=1; x<spectImg.getWidth(); x++){
            for(int y=0; y<spectImg.getHeight(); y++){
                ofColor oldColor = spectImg2.getColor(x-1, y);
                spectImg.setColor(x, y, oldColor);
            }
        }
        
        //add new data at end
        for(int y=0; y<spectImg.getHeight(); y++){
            float val = (0.5-avg)+raw_fft[y];
            
            float h = val*255;
            float s = 200-(val*155);
            float b = val*255;
            ofColor color = ofColor::fromHsb(h, s, b);
            spectImg.setColor(0, spectImg.getHeight()-(y+1), color);
        }
        
        spectImg.update();

    
    spectImg.draw(0, 0, 0, w, h);

    //ofColor color = ofColor::fromHsb(raw_fft[y]*255, raw_fft[y]*255, raw_fft[y]*255);
    //draw image
}


