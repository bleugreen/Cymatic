//
//  DisplayController.cpp
//  SoundProfiler
//
//  Created by Mitch on 12/29/20.
//

#include "DisplayController.h"

DisplayController::DisplayController(){}

void DisplayController::setup(Analysis* a, int w, int h, ofxGuiGroup* all){
    analysis = a;
    
    current_mode = 0;
    
//    ld = std::shared_ptr<LinearDisplay>(new LinearDisplay());
//    ld->setup();
//    modes.push_back(ld);
    
    rd = std::shared_ptr<RawDisplay>(new RawDisplay());
    rd->setup();
    modes.push_back(rd);
    
//    od = std::shared_ptr<OscDisplay>(new OscDisplay());
//    od->setup();
//    modes.push_back(od);
    
    modeSelector.setName("Display Mode");
   // modeSelector.add(disp0.set(ld->name,false));
    modeSelector.add(disp1.set(rd->name,false));
  //  modeSelector.add(disp2.set(od->name,false));

    
    modeSelectorGroup = all->addGroup(modeSelector);
    modeSelectorGroup->setExclusiveToggles(true);
    modeSelectorGroup->loadTheme("default-theme.json");
    modeSelectorGroup->setConfig(ofJson({{"type", "radio"}}));
    
    modeControlGroup = all->addGroup("Mode Controls");

    for(int i=0; i<modes.size(); i++){
        
        modes[i]->setDimensions(10, 10); // STUB
        
        ofxGuiGroup* modeGroup = modeControlGroup->addGroup("p."+std::to_string(i));
        modeGroup->setShowHeader(false);
        modes[i]->buildGui(modeGroup);
        if(i != 0) modeGroup->minimize();
        modeControls.push_back(modeGroup);
    }
        
    
    
    
    // set up modes
    // build selector list
    //
    modeSelectorGroup->getActiveToggleIndex().addListener(this, &DisplayController::setDisplayMode);
    modeSelectorGroup->setActiveToggle(0);

    
    
    ready = true;
    
}

void DisplayController::setDisplayMode(int& index){
    modeControlGroup->minimizeAll();
    switch(index){
            default: case 0:
            current_mode = 0;
            modeControls[0]->maximize();
                break;
//            case 1:
//            current_mode = 1;
//            modeControls[1]->maximize();
//                break;
//            case 2:
//            current_mode = 2;
//            modeControls[2]->maximize();
//                break;
        }
}


void DisplayController::draw(){
    if(modes[current_mode] != NULL){
        modes[current_mode]->draw();
    }
}

void DisplayController::minimize(){
    modeSelectorGroup->minimize();
    modeControlGroup->minimize();
}

void DisplayController::maximize(){
    modeSelectorGroup->maximize();
    modeControlGroup->maximize();
}

void DisplayController::update(){
    int n = current_mode;
    if(modes[n] != NULL){
        std::vector<utils::soundData> ret;
        for(utils::soundType req : modes[current_mode]->dataRequest){
            utils::soundData container;
            container.label = req;
            std::vector<float> inData = analysis->getData(req);
            container.data = inData;
            
            ret.push_back(container);
        }
        
        modes[n]->update(ret);
    }
}

void DisplayController::updateLayout(int w, int h){
    width = w;
    height = h;
    
    for(std::shared_ptr<Display> mode : modes){
        mode->setDimensions(w, h);
    }
}
void DisplayController::incMode(){
    int n = (current_mode+1) % modes.size();
    modeSelectorGroup->setActiveToggle(n);
}


int DisplayController::getMode(){
    return current_mode;
}

void DisplayController::setMode(int index){
    modeSelectorGroup->setActiveToggle(index);
}

