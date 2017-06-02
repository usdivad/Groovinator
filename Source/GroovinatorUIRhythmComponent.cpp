//
//  GroovinatorUIRhythmComponent.cpp
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#include "GroovinatorUIRhythmComponent.h"


//==============================================================================
GroovinatorUIStepButton::GroovinatorUIStepButton(const String& buttonName) : Button(buttonName)
{
    
}

void GroovinatorUIStepButton::paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    if (isButtonDown)
    {
        g.fillAll(Colours::orange);
    }
    else if (isMouseOverButton)
    {
        g.fillAll(Colours::purple);
    }
    else
    {
        g.fillAll(Colours::indigo);
    }
}

//==============================================================================

GroovinatorUIRhythmComponent::GroovinatorUIRhythmComponent(RhythmComponentType rhythmType, GroovinatorRhythmHandler& rhythmHandler) : Component(), _rhythmType(rhythmType), _rhythmHandler(rhythmHandler)
{
    setRhythmHandler(rhythmHandler);
}

void GroovinatorUIRhythmComponent::setRhythmHandler(GroovinatorRhythmHandler& rhythmHandler)
{
    _rhythmHandler = rhythmHandler;
    printf("rhythm handler has been reset: %s\n", GroovinatorRhythmHandler::rhythmToString(rhythmHandler.getOriginalRhythm()).c_str());
}


void GroovinatorUIRhythmComponent::paint(juce::Graphics &g)
{
    // Get rhythm data
    std::vector<int> rhythm;
    if (_rhythmType == kOriginalRhythm)
    {
        rhythm = _rhythmHandler.getOriginalRhythm();
    }
    else if (_rhythmType == kTargetRhythm)
    {
        rhythm = _rhythmHandler.getTargetRhythm();
    }
    
    int numSteps = rhythm.size();
    std::string rhythmStr = GroovinatorRhythmHandler::rhythmToString(rhythm);
    
    printf("rhythm=%s, numSteps=%d, stepButtonsSize=%d\n", rhythmStr.c_str(), numSteps, _stepButtons.size());
    
    // Recreate all the buttons if necessary
    if (numSteps != _stepButtons.size())
    {
        _stepButtons.clear();
        
        for (int i=0; i<numSteps; i++)
        {
            String buttonName;
            String rhythmTypeName = _rhythmType == kOriginalRhythm ? "original" : "target";
            buttonName  << rhythmTypeName << "Step" << i;
            
            GroovinatorUIStepButton* stepButton = new GroovinatorUIStepButton(buttonName);
            addAndMakeVisible(stepButton);
            _stepButtons.add(stepButton);
            
            printf("created %s\n", buttonName.toRawUTF8());
        }
    }
    
    // Actually draw stuff
    g.fillAll(Colours::darkslategrey);
    
    Rectangle<int> bounds = getBounds();
    double stepButtonWidth = bounds.getWidth() / (double) numSteps;
    double stepButtonPulseHeight = bounds.getHeight();
    double stepButtonStepHeight = bounds.getHeight() * 0.3;
    
    for (int i=0; i<numSteps; i++)
    {
        GroovinatorUIStepButton* stepButton = _stepButtons[i];
        double stepButtonX = i * stepButtonWidth;
        //double stepButtonY = getHeight() / 2.0;
        double stepButtonY = 0.0;
        double stepButtonHeight = rhythm[i] == 1 ? stepButtonPulseHeight : stepButtonStepHeight;
        stepButtonY = rhythm[i] == 1 ?  stepButtonY : stepButtonY + (getHeight()-stepButtonHeight+1);
        stepButton->setBounds(stepButtonX, stepButtonY, stepButtonWidth, stepButtonHeight);
    }
}
