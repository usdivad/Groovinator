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
        g.fillAll(Colours::lightgrey);
    }
    else
    {
        g.fillAll(Colours::indigo);
    }
}

//==============================================================================

GroovinatorUIRhythmComponent::GroovinatorUIRhythmComponent(RhythmComponentType rhythmType, GroovinatorRhythmHandler& rhythmHandler) : Component(), _rhythmType(rhythmType), _rhythmHandler(rhythmHandler)
{
    
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
        }
    }
    
    // Actually draw stuff
    g.fillAll(Colours::white);
    
    Rectangle<int> bounds = getBounds();
    double stepButtonWidth = bounds.getWidth() / (double) numSteps;
    double stepButtonPulseHeight = bounds.getHeight() * 0.75;
    double stepButtonStepHeight = bounds.getHeight() * 0.5;
    
    for (int i=0; i<numSteps; i++)
    {
        GroovinatorUIStepButton* stepButton = _stepButtons[i];
        double stepButtonX = i * stepButtonWidth;
        double stepButtonY = getHeight() / 2.0;
        double stepButtonHeight = rhythm[i] == 1 ? stepButtonPulseHeight : stepButtonStepHeight;
        stepButton->setBounds(stepButtonX, stepButtonY, stepButtonWidth, stepButtonHeight);
    }
}
