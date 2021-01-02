#include "ofApp.h"

    
//--------------------------------------------------------------
void ofApp::setup(){
    ofSetVerticalSync(true);
    ofxGuiEnableHiResDisplay();

    gui.setup("panel"); // most of the time you don't need a name but don't forget to call setup
    
    bufferSize = 2048;
    sampleRate = 44100;
    int channels = 2;
    
    audioAnalyzer.setup(sampleRate, bufferSize, channels);
    
    player.load("filetype-test.mp3");
    
    
    chrom = new Chromagram(Chromagram::Parameters(sampleRate));
}

//--------------------------------------------------------------
void ofApp::exit(){
   
}



//--------------------------------------------------------------
void ofApp::update(){
}

//--------------------------------------------------------------
void ofApp::draw(){

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
    player.play();
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
