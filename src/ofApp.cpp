#include "ofApp.h"

    
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofxBaseGui::enableHiDpi();
    
    win = (ofAppGLFWWindow*)ofGetWindowPtr();
    coordScale = win->getPixelScreenCoordScale();
    ofSetWindowShape(win->getPixelScreenCoordScale()*1024, win->getPixelScreenCoordScale()*768);
//    if(win->getPixelScreenCoordScale() == 2){
//        themeName = "retina-theme.json";
//    }
//    else{
    cout << "scale: " << win->getPixelScreenCoordScale() << endl;
    
    themeName = "default-theme.json";
    

    cout << utils::secondsToTimeLabel(326.993) << endl;
    
    bufferSize = 2048;
    sampleRate = 44100;
    int channels = 1;
    
    inputMode = true;
    
    ofSoundStreamSettings settings;
    settings.setOutListener(this);
    settings.setInListener(this);
    settings.numOutputChannels = 2;
    settings.numInputChannels = 1;
    settings.numBuffers = 8;
    settings.bufferSize = bufferSize;
    
    soundStream.setup(settings);
    
    audioAnalyzerIn.setup(sampleRate, bufferSize, 1);
    audioAnalyzerOut.setup(sampleRate, bufferSize, 2);
    
    player.load("filetype-test.mp3");
    
    
    
    //chrom = new Chromagram(Chromagram::Parameters(sampleRate));
    
    
    
    
    //-------------------------------------------------------------------------------------
    // GUI Initialization
    //-------------------------------------------------------------------------------------
    
    
    
    // main panel
    //-------------------------------------------------------------------------------------
    gui.setupFlexBoxLayout();
    all = gui.addGroup("Sonic Profiler", ofJson({
        {"flex-direction", "column"},
        {"flex", 1},
        {"background-color", "#181818"},
        {"flex-wrap", "wrap"},
        {"show-header", true},
        {"position", "static"},
    }));
    
    
    
    // input mode / file manager
    //-------------------------------------------------------------------------------------
    inputParameters.setName("Input Source");
    inputParameters.add(input0.set("Microphone", false));
    inputParameters.add(input1.set("Play File", false));
    
    inputToggles = all->addGroup(inputParameters);
    inputToggles->setExclusiveToggles(true);
    inputToggles->setConfig(ofJson({{"type", "radio"}}));
    
    fileManager = inputToggles->addGroup("File Manager");
    fileManager->setShowHeader(false);
    
    
    
    playbackControls = fileManager->addGroup("Playback",ofJson({
        {"flex-direction", "row"},
        {"flex", 0},
        {"justify-content", "center"},
        {"margin", 2},
        {"padding", 2},
        {"flex-wrap", "wrap"},
        {"show-header", false},
    }));

    
    playbackControls->add(filePath.set("path"), ofJson({{"text-align", "center"}, {"width", "90%"}}));
    filePath.setMax("AAAAAAAAAAAAAAAAAAAAAAAAA");
    playbackControls->add(playButton.set("Play/Pause"), ofJson({{"type", "fullsize"}, {"text-align", "center"}, {"width", "45%"}}));
    playbackControls->add(resetButton.set("Reset"), ofJson({{"type", "fullsize"}, {"text-align", "center"}, {"width", "45%"}}));
    float duration = player.getDuration();
    playbackControls->add(seekSlider.set("Seek", 0, 0, duration), ofJson({{"precision", 0}, {"width", "100%"}}));
    playbackControls->add(volumeSlider.set("Volume", 100, 0, 100), ofJson({{"precision", 0}, {"width", "100%"}}));
    playbackControls->minimize();
    
    fileManager->add(loadButton.set("Choose File"), ofJson({{"type", "fullsize"}, {"text-align", "center"}}));
    fileManager->minimize();

   
    // misc
    //-------------------------------------------------------------------------------------
//    dc.setup(&analysis, ofGetWidth(), ofGetHeight(), all);
    
    all->add(minimizeButton.set("Collapse All"), ofJson({{"type", "fullsize"}, {"text-align", "center"}}));
    
    // listeners
    //-------------------------------------------------------------------------------------
    
    // input radio
    inputToggles->getActiveToggleIndex().addListener(this, &ofApp::setInputMode);
    inputToggles->setActiveToggle(0);
    
    // file buttons
    loadButton.addListener(this, &ofApp::loadFile);
    playButton.addListener(this, &ofApp::playFile);
    resetButton.addListener(this, &ofApp::restartFile);
    
    // minimize button
    minimizeButton.addListener(this, &ofApp::minimizePressed);
    
    seekSlider.addListener(this, &ofApp::seekChanged);
    volumeSlider.addListener(this, &ofApp::volumeChanged);
    
    all->loadTheme(themeName);
    
    file_pos = 0;
    ready = true;
}

//-------------------------------------------------------------------------------------
// listeners
//-------------------------------------------------------------------------------------

//--------------------------------------------------------------
// Update input source and show/hide file manager
void ofApp::seekChanged(float &val){
    if(val != file_pos){
        float pct = val / player.getDuration();
        player.setPosition(pct);
    }
}

//--------------------------------------------------------------
// Update input source and show/hide file manager
void ofApp::volumeChanged(float &val){
    player.setVolume(val / 100.);
}


//--------------------------------------------------------------
// Update input source and show/hide file manager
void ofApp::setInputMode(int& index){
    switch (index) {
        case 0:
            inputMode = true;
            fileManager->minimize();
            player.setPaused(true);
            break;
        case 1:
            inputMode = false;
            fileManager->maximize();
            if(fileLoaded) playbackControls->maximize();
            break;

        default:
            break;
    }
}


//--------------------------------------------------------------
// Open system dialog and allow user to choose .wav file
void ofApp::loadFile(){
    
    // Listener fires twice when pressed (click & release)
    // So this check makes sure it only prompts once
    if(!loadPressed){
        loadPressed = true;

        // Opens system dialog asking for a file
        // Checks to make sure it's .wav then loads

        ofFileDialogResult result = ofSystemLoadDialog("Load file");

        if(result.bSuccess) {
            fileLoaded = false;
            playbackControls->minimize();
            string path = result.getPath();
            string name = result.getName();
            cout << path << endl;

     
                try{
                    player.unload();
                    if(player.load(path)){
                        if(name.length() < 25) filePath.set(name);
                        else{
                            std::string croppedName = name.substr(0, 24)+"...";
                            filePath.set(croppedName);
                        }
                        fileLoaded = true;
                        seekSlider.set(0);
                        seekSlider.setMax(player.getDuration());
                        playbackControls->maximize();
                    }
                    else{
                        ofSystemAlertDialog("Invalid File");
                    }
                }
                catch(...){
                    ofSystemAlertDialog("Invalid File");
                }

        }
    }

    loadPressed = false;
}

//--------------------------------------------------------------
// Toggle whether a file should be playing or not
void ofApp::playFile(){
    // Listener fires twice when pressed (click & release I think)
    // So this check makes sure it only prompts once
    if(!playPressed){
        playPressed = true;
        if(player.isPlaying()) {
            player.setPaused(true);
        }
        else {
            player.play();
        }


        shouldPlayAudio = !shouldPlayAudio;
    }

    playPressed = false;
    
}

//--------------------------------------------------------------
// Toggle whether a file should be playing or not
void ofApp::restartFile(){
    
    // Listener fires twice when pressed (click & release I think)
    // So this check makes sure it only prompts once
    if(!resetPressed){
        resetPressed = true;
        player.setPaused(true);
        player.setPosition(0);
    }

    resetPressed = false;
    
}


//--------------------------------------------------------------
// Collapse main panels
void ofApp::minimizePressed(){
//    dc.minimize();
    inputToggles->minimize();
}

void ofApp::maximize(){
//    dc.maximize();
//    inputToggles->maximize();
}

//--------------------------------------------------------------
// Retrieves and formats current frame of audio input then sends to analysis
void ofApp::audioIn(ofSoundBuffer& buffer) {
        if(inputMode && ready)
    {
        audioAnalyzerIn.analyze(buffer);
        
        spectrum_l = audioAnalyzerIn.getValues(HPCP, 0);
        //spectrum_r = audioAnalyzer.getValues(SPECTRUM, 1);
        
        
        //Waveform update:
        waveform_l.clear();
        //waveform_r.clear();
        
        //waveform for channel 0
        int ch=0;
        for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
            float sample = soundBuffer.getSample(i, ch);
            float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth()*0.5);
            float y = ofMap(sample, -1, 1, 0, ofGetHeight()*0.5);
            waveform_l.addVertex(x, y);
        }
    }
}


//--------------------------------------------------------------
void ofApp::update(){
    if(!inputMode && ready){
        soundBuffer = player.getCurrentSoundBuffer(bufferSize);
        
        file_pos = player.getPosition() * player.getDuration();

        seekSlider = file_pos;
        
        audioAnalyzerOut.analyze(soundBuffer);
        
        spectrum_l = audioAnalyzerOut.getValues(HPCP, 0, 0.8);
        

        //spectrum_r = audioAnalyzer.getValues(SPECTRUM, 1);
        
        
        //Waveform update:
        waveform_l.clear();
        //waveform_r.clear();
        
        //waveform for channel 0
        int ch=0;
        for(size_t i = 0; i < soundBuffer.getNumFrames(); i++) {
            float sample = soundBuffer.getSample(i, ch);
            float x = ofMap(i, 0, soundBuffer.getNumFrames(), 0, ofGetWidth()*0.5);
            float y = ofMap(sample, -1, 1, 0, ofGetHeight()*0.5);
            waveform_l.addVertex(x, y);
        }
    }
}

//--------------------------------------------------------------
void ofApp::draw(){
    if(coordScale != win->getPixelScreenCoordScale()){
        coordScale = win->getPixelScreenCoordScale();
        switch (coordScale) {
            case 1:
                all->loadTheme("default-theme.json");
                break;
            case 2:
                all->loadTheme("retina-theme.json");
                break;
            default:
                break;
        }
    }
    
    
    //draw waveforms:
    ofSetColor(ofColor::hotPink);
    waveform_l.draw();
//    waveform_r.draw();
    
    //draw spectrums:
    int mw = ofGetWidth()/2;
    
    ofPushMatrix();
    ofTranslate(ofGetWidth()/2, 0);
    ofSetColor(ofColor::cyan);
    float bin_w = (float) mw / spectrum_l.size();
    for (int i = 0; i < spectrum_l.size(); i++){
        float scaledValue = ofMap(spectrum_l[i], 0, 1, 0.0, 1.0, true);//clamped value
        float bin_h = -1 * (scaledValue * ofGetHeight()/2);
        ofDrawRectangle(i*bin_w, ofGetHeight()/2, bin_w, bin_h);
    }
    ofPopMatrix();
    
    
    ofSetColor(100);
    ofDrawLine(ofGetWidth()/2, 0, ofGetWidth()/2, ofGetHeight());
    ofDrawLine(0, ofGetHeight()/2, ofGetWidth(), ofGetHeight()/2);
    ofSetColor(255);
    ofDrawBitmapString("ofxAudioAnalyzer - AUDIO FILE PLAYER EXAMPLE \nPress any key to play audio file ", 350, 32);

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    if(key == ' ') player.play();
    if(key == 'q') inputMode = !inputMode;
}

//--------------------------------------------------------------
void ofApp::audioOut(ofSoundBuffer& buffer){
    
}


//--------------------------------------------------------------
void ofApp::exit(){
   
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){
    
}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){
    
}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){
    
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){
    
}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){
    
}
