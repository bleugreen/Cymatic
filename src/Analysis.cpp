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
    
//    // Data array initialization
//    // Builds list of frequencies
//    for(int i=-2; i<=3; i++){
//        for(int j=0; j<chromaticScale.size(); j++){
//            freqlist.push_back(chromaticScale[j]*pow(2, i));
//        }
//    }
//    
//    // Builds corresponding list of FFT bins
//    // Translate frequency list to bin list
//    for(int i=0; i<freqlist.size(); i++){
//        float bin = fft->getBinFromFrequency(freqlist[i], 44100);
//        fullBinList.push_back((int)bin);
//        
//    }
//    
//    // Resize data structures
//    // In order to simplify passing octave + individual note data between
//    // audio and drawing threads, we use one array where:
//    //     audioData[0,oct_size-1] = octave data
//    //     audioData[oct_size, wholeDataSize] = individual note data
//    oct_size = chromaticScale.size();
//    scale_size = fullBinList.size();
//    
//    
//    fft_size = fft->getBinSize();
//    
//    for(int i=0; i<fft_size; i++){
//        in_fft.push_back(0.001);
//        raw_fft.push_back(0.001);
//    }
//    
//    
//    for(int i=0; i<oct_size; i++){
//        raw_octave.push_back(0.001);
//        smooth_octave.push_back(0.001);
//        
//        in_octave.push_back(0.001);
//    }
//    
//    for(int i=0; i<scale_size; i++){
//        raw_scale.push_back(0.001);
//        smooth_scale.push_back(0.001);
//        smooth_scale_ot.push_back(0.001);
//        
//    }
    
  //  chrom = new Chromagram(Chromagram::Parameters(44100));
    
    
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




//--------------------------------------------------------------
bool Analysis::smoothFrame(){
    // Smoothing+Moving data from buffer
    
//    // NaN checker (doesn't get used often but sometimes it's helpful
//    if(raw_scale[0] != raw_scale[0]){
//        // NaN. skip frame
//        
//        
//        // Gradual fade out for smoothed data sources
//        for(float val : smooth_scale){
//            val = utils::approxRollingAverage(val, 0, 30);
//        }
//        
//        for(float val : smooth_scale_ot){
//            val = utils::approxRollingAverage(val, 0, 30);
//        }
//        
//        for(float val : smooth_octave){
//            val = utils::approxRollingAverage(val, 0, 30);
//        }
//        
//        // Clear graphs if NaN and on output mode (usually means file not playing)
//        return false;
//        // Sometimes buffer will be NaN during input lag
//        // So if we reset the graph each time (on input mode) it would be jumpy
//        // Haven't observed the same issue on output
//    }
//    
//    raw_fft = in_fft;
//    
//    // At the moment, smoothing consists of:
//    //   - rolling average to make it less 'jumpy'
//    for(int i=0; i<oct_size; i++){
//        //raw_octave[i] = ofClamp(raw_octave[i], 0, 1); // clamp new data
//        smooth_octave[i] = utils::approxRollingAverage(smooth_octave[i], raw_octave[i], 3);
//        if(smooth_octave[i] < 0.3) smooth_octave[i] *= smooth_octave[i];
//    }
//    
//    float newVal, overtone;
//    for(int i=0; i<scale_size; i++){
//        newVal = raw_scale[i];
//        overtone = 0;
//        int count = 0;
//        for(int j=i+12; j<scale_size; j+=12){
//            overtone += raw_scale[j];
//            count += 1;
//        }
//        if(count > 0) {
//            overtone /= count;
//            newVal = (raw_scale[i]+overtone)/2;
//        }
//        
//        smooth_scale_ot[i] = utils::approxRollingAverage(smooth_scale[i], newVal, 3);
//        smooth_scale[i] = utils::approxRollingAverage(smooth_scale[i], raw_scale[i], 3);
//    }
//    
//    return true;
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
