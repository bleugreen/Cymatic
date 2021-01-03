#pragma once

#include "ofMain.h"
#include "ofxGuiExtended.h"
#include "Chromagram.h"
#include "ofxAudioAnalyzer.h"
#include "ofSoundPlayerExtended.h"
#include "Analysis.h"
#include "DisplayController.h"
#include "utils.h"

#define WIN_WIDTH 1000
#define WIN_HEIGHT 800

class ofApp : public ofBaseApp{
    
public:
    //--------------------------------------------------------------------------------
    // of builtins
    //--------------------------------------------------------------------------------
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
    
    bool ready{};
    
    
    //--------------------------------------------------------------------------------
    // audio
    //--------------------------------------------------------------------------------
    void audioIn(ofSoundBuffer& buffer);
    void audioOut(ofSoundBuffer& buffer);

    ofSoundStream soundStream;
    ofSoundPlayerExtended  player;
    ofSoundBuffer soundBuffer;
    
    Analysis analysis;
    
    int sampleRate;
    int bufferSize;
    bool inputMode{};

    
    //--------------------------------------------------------------------------------
    // drawing
    //--------------------------------------------------------------------------------
    void updateLayout(int w, int h);

    DisplayController dc;
    
    ofAppGLFWWindow* win;
    std::string themeName;
    int coordScale;
    int controlWidth;
    

    //--------------------------------------------------------------------------------
    // GUI
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    //   general
    //--------------------------------------------------------------------------------
    void minimizePressed();
    void maximize();
    
    ofxGuiGroup* all;
    ofxGui gui;
    ofxGuiGroup* modeControls;
    ofParameter<void> minimizeButton;

    
    //--------------------------------------------------------------------------------
    //   input mode
    //--------------------------------------------------------------------------------
    void setInputMode(int& index);
    
    ofxGuiGroup *inputToggles;
    ofParameterGroup inputParameters;
    ofParameter<bool> input0;
    ofParameter<bool> input1;


    //--------------------------------------------------------------------------------
    //   file manager
    //--------------------------------------------------------------------------------
    void loadFile();
    
    ofxGuiGroup *fileManager;
    ofParameter<void> loadButton;
    ofParameter<string> filePath;

    bool inputBool{true};
    bool fileLoaded{};
    bool loadPressed{};
    
    
    //--------------------------------------------------------------------------------
    //   file player
    //--------------------------------------------------------------------------------
    void playFile();
    void restartFile();
    void seekChanged(float& val);
    void volumeChanged(float& val);
    
    ofxGuiGroup *playbackControls;
    ofParameter<void> playButton;
    ofParameter<void> resetButton;
    ofParameter<float> seekSlider;
    ofParameter<float> volumeSlider;
    
    float file_pos;
    bool playPressed{};
    bool resetPressed{};
    bool shouldPlayAudio{};

}; // ofApp

