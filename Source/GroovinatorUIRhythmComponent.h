//
//  GroovinatorUIRhythmComponent.h
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#ifndef GroovinatorUIRhythmComponent_h
#define GroovinatorUIRhythmComponent_h

#include <vector>

#include "../JuceLibraryCode/JuceHeader.h"

#include "GroovinatorRhythmHandler.h"


//==============================================================================
/**
 */
class GroovinatorUIStepButton : public Button
{
public:
    GroovinatorUIStepButton(const String& buttonName);
    void paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown) override;
};

//==============================================================================
/**
 */
class GroovinatorUIRhythmComponent : public Component
{
public:
    enum RhythmComponentType
    {
        kOriginalRhythm,
        kTargetRhythm
    };
    
    GroovinatorUIRhythmComponent(RhythmComponentType rhythmType, GroovinatorRhythmHandler& rhythmHandler);
    void paint(Graphics& g) override;
    
    void setRhythmHandler(GroovinatorRhythmHandler& rhythmHandler);
private:
    GroovinatorRhythmHandler& _rhythmHandler;
    RhythmComponentType _rhythmType;
    OwnedArray<GroovinatorUIStepButton> _stepButtons;
};

#endif /* GroovinatorUIRhythmComponent_h */
