//
//  analysis.cpp
//  soundProfiler
//
//  Created by Mitch on 12/27/20.
//
#include "Analysis.h"


Analysis::Analysis(){}


//--------------------------------------------------------------
void Analysis::init(int sampleRate, int bufferSizeIn, int channelsIn){
    bufferSize = bufferSizeIn;
    channels = channelsIn;
    analyzer.setup(sampleRate, bufferSize, channels);
    
    // Disable unused algorithms
    for(int i=0; i<channels; i++){
        analyzer.setActive(i, PITCH_FREQ, false);
        analyzer.setActive(i, PITCH_CONFIDENCE, false);
        analyzer.setActive(i, PITCH_SALIENCE, false);
        analyzer.setActive(i, INHARMONICITY, false);
        analyzer.setActive(i, HFC, false);
        analyzer.setActive(i, CENTROID, false);
        analyzer.setActive(i, SPECTRAL_COMPLEXITY, false);
        analyzer.setActive(i, DISSONANCE, false);
        analyzer.setActive(i, ROLL_OFF, false);
        analyzer.setActive(i, ODD_TO_EVEN, false);
        analyzer.setActive(i, STRONG_PEAK, false);
        analyzer.setActive(i, STRONG_DECAY, false);
        analyzer.setActive(i, MEL_BANDS, false);
        analyzer.setActive(i, MFCC, false);
        analyzer.setActive(i, MULTI_PITCHES, false);
        analyzer.setActive(i, PITCH_SALIENCE_FUNC_PEAKS, false);
        analyzer.setActive(i, TRISTIMULUS, false);
        analyzer.setActive(i, CHROMAGRAM, false);
        analyzer.setActive(i, ONSETS, false);
    }
}


//--------------------------------------------------------------
void Analysis::analyzeFrame(ofSoundBuffer& buffer){
    if(buffer.getNumChannels() != channels ||
       buffer.getSampleRate() != sampleRate)
    {
        bufferSize = buffer.getBuffer().size() / buffer.getNumChannels();
        channels = buffer.getNumChannels();
        sampleRate = buffer.getSampleRate();
        analyzer.reset(sampleRate, bufferSize, channels);
    }
    
    analyzer.analyze(buffer);
}


//---------------------------------------------------------------------------
// getters & setters
//---------------------------------------------------------------------------

//--------------------------------------------------------------
bool Analysis::isFrameReady(){ return frameReady; }




//--------------------------------------------------------------
std::vector<float> Analysis::getData(utils::soundType st){
    switch (st) {
        default: case utils::FFT:
            return analyzer.getValues(SPECTRUM, 0);
            break;
        case utils::HPCP:
            return analyzer.getValues(HPCP, 0);
            break;
        case utils::BARK:
            return analyzer.getValues(BARK, 0);
            break;
    }
}

//int Analysis::getSize(utils::soundType st){
//    switch (st) {
//        default: case utils::RAW_FULL:
//            return fft_size;
//            break;
//        case utils::SMOOTH_OCTAVE:
//        case utils::RAW_OCTAVE:
//            return oct_size;
//            break;
//        case utils::RAW_SCALE:
//        case utils::SMOOTH_SCALE:
//        case utils::SMOOTH_SCALE_OT:
//            return scale_size;
//            break;
//    }
//}
