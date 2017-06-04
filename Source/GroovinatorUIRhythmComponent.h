//
//  GroovinatorUIRhythmComponent.h
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#ifndef GroovinatorUIRhythmComponent_h
#define GroovinatorUIRhythmComponent_h

#include "../JuceLibraryCode/JuceHeader.h"

#include "GroovinatorRhythmHandler.h"


//==============================================================================
/**
 */
class GroovinatorUIStepButton : public Button
{
public:
    GroovinatorUIStepButton(const String& buttonName, int index, int totalNumSteps, GroovinatorRhythmHandler& rhythmHandler);
    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
    bool isPlayHeadAtStep();
private:
    int _index;
    GroovinatorRhythmHandler& _rhythmHandler;
    int _totalNumSteps;
};

//==============================================================================
/**
 */
class GroovinatorUIRhythmComponent : public Component,
                                     public Button::Listener
{
public:
    enum RhythmComponentType
    {
        kOriginalRhythm,
        kTargetRhythm
    };
    
    GroovinatorUIRhythmComponent(RhythmComponentType rhythmType, GroovinatorRhythmHandler& rhythmHandler);
    void paint(Graphics& g) override;
    void buttonClicked(Button* button) override;
    
    void setRhythmHandler(GroovinatorRhythmHandler& rhythmHandler);
private:
    GroovinatorRhythmHandler& _rhythmHandler;
    RhythmComponentType _rhythmType;
    OwnedArray<GroovinatorUIStepButton> _stepButtons;
};

#endif /* GroovinatorUIRhythmComponent_h */
