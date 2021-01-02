#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxStk.h"
#include "Chromagram.h"

class ofApp : public ofBaseApp{
    
public:
    void setup();
    void update();
    void draw();
    
    void exit();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void mouseEntered(int x, int y);
    void mouseExited(int x, int y);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);
    
    
    //--------------------------------------------------------------------------------
    // audio
    //--------------------------------------------------------------------------------
    void audioIn(ofSoundBuffer& buffer);
    void audioOut(ofSoundBuffer& buffer);
    void soundstream_init();


    int bufferSize;
    stk::FileLoop file;
    ofSoundStream soundStream;
    bool shouldPlayAudio{};
    
    //--------------------------------------------------------------------------------
    //   general
    //--------------------------------------------------------------------------------
    ofxGuiGroup* all;
    ofxGuiGroup* modeControls;
    ofParameter<void> minimizeButton;

    void minimizePressed();
    void maximize();





    //--------------------------------------------------------------------------------
    //   input mode
    //--------------------------------------------------------------------------------
    ofxGuiGroup *inputToggles;
    ofParameterGroup inputParameters;
    ofParameter<bool> input0;
    ofParameter<bool> input1;

    void setInputMode(int& index);


    //--------------------------------------------------------------------------------
    //   file manager
    //--------------------------------------------------------------------------------
    ofxGuiGroup *fileManager;
    ofxGuiGroup *playbackControls;
    ofParameter<void> loadButton;
    ofParameter<void> playButton;
    ofParameter<void> resetButton;
    ofParameter<string> filePath;

    bool inputBool{true}, fileLoaded{};
    bool loadPressed{}, playPressed{}, resetPressed{};

    void loadFile();
    void playFile();
    void restartFile();
    
    
    Chromagram* chrom;
    ofxPanel gui;

    void circleResolutionChanged(int & circleResolution);
    void ringButtonPressed();

    bool bHide;

    ofParameter<float> radius;
    ofParameter<ofColor> color;
    ofParameter<glm::vec2> center;
    ofParameter<int> circleResolution;
    ofParameter<bool> filled;
    ofxButton twoCircles;
    ofxButton ringButton;
    ofParameter<string> screenSize;

    

    ofSoundPlayer ring;
    
    
    
};

