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
    
    ofxAudioAnalyzer audioAnalyzerIn;
    ofxAudioAnalyzer audioAnalyzerOut;

    ofSoundPlayerExtended  player;
    ofSoundBuffer soundBuffer;

    int sampleRate;
    int bufferSize;
    
    Chromagram* chrom;
    
    bool ready{};
    
    ofAppGLFWWindow* win;
    int coordScale;
    
    std::string themeName;
    
    //--------------------------------------------------------------------------------
    // audio
    //--------------------------------------------------------------------------------
    void audioIn(ofSoundBuffer& buffer);
    void audioOut(ofSoundBuffer& buffer);

    ofSoundStream soundStream;
    bool inputMode{};
    
    Analysis analysis;
    
    vector<float> spectrum_l, spectrum_r;

    ofPolyline waveform_l, waveform_r;
    
    //--------------------------------------------------------------------------------
    // drawing
    //--------------------------------------------------------------------------------
    void updateLayout(int w, int h);
    int controlWidth;

    DisplayController dc;

    //--------------------------------------------------------------------------------
    // GUI
    //--------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------
    //   general
    //--------------------------------------------------------------------------------
    ofxGuiGroup* all;
    ofxGui gui;
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
    
    ofParameter<void> loadButton;
    ofParameter<string> filePath;

    bool inputBool{true}, fileLoaded{};
    bool loadPressed{};

    void loadFile();
    
    
    //--------------------------------------------------------------------------------
    //   file player
    //--------------------------------------------------------------------------------
    bool playPressed{}, resetPressed{}, shouldPlayAudio{};
    void playFile();
    void restartFile();
    
    ofxGuiGroup *playbackControls;
    ofParameter<void> playButton;
    ofParameter<void> resetButton;
    ofParameter<float> seekSlider;
    ofParameter<float> volumeSlider;
    
    void seekChanged(float& val);
    void volumeChanged(float& val);
    
    float file_pos;


    
    
    
};

