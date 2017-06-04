//
//  GroovinatorUIRhythmComponent.cpp
//  Groovinator
//
//  Created by David Su on 6/1/17.
//
//

#include "GroovinatorUIRhythmComponent.h"


//==============================================================================
GroovinatorUIStepButton::GroovinatorUIStepButton(const String& buttonName, int index, int totalNumSteps, GroovinatorRhythmHandler& rhythmHandler) : Button(buttonName), _index(index), _totalNumSteps(totalNumSteps), _rhythmHandler(rhythmHandler)
{
    
}

void GroovinatorUIStepButton::paintButton(Graphics& g, bool isMouseOverButton, bool isButtonDown)
{
    if (isPlayHeadAtStep())
    {
        g.fillAll(Colours::orangered);
        return;
    }
    
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

bool GroovinatorUIStepButton::isPlayHeadAtStep()
{
    double playHeadProportion = _rhythmHandler.getProportionOfRhythmElapsed();
    int playHeadIndex = GroovinatorRhythmHandler::proportionToStepIndex(playHeadProportion, _totalNumSteps);
    return playHeadIndex == _index;
}

//==============================================================================

GroovinatorUIRhythmComponent::GroovinatorUIRhythmComponent(RhythmComponentType rhythmType, GroovinatorRhythmHandler& rhythmHandler) : Component(),  _rhythmHandler(rhythmHandler), _rhythmType(rhythmType)
{
    setRhythmHandler(rhythmHandler);
}

void GroovinatorUIRhythmComponent::setRhythmHandler(GroovinatorRhythmHandler& rhythmHandler)
{
    _rhythmHandler = rhythmHandler;
    printf("rhythm handler has been reset: %s + %s\n", rhythmHandler.getOriginalRhythmStr().c_str(), rhythmHandler.getTargetRhythmStr().c_str());
}

void GroovinatorUIRhythmComponent::buttonClicked(Button* button)
{
    int buttonIdx = _stepButtons.indexOf((const GroovinatorUIStepButton*) button);
    if (buttonIdx >=0)
    {
        if (_rhythmType == kOriginalRhythm)
            _rhythmHandler.toggleOriginalRhythmStepAt(buttonIdx);
        else if (_rhythmType == kTargetRhythm)
            _rhythmHandler.toggleTargetRhythmStepAt(buttonIdx);
        
        repaint();
    }
}


void GroovinatorUIRhythmComponent::paint(juce::Graphics &g)
{
    // Get rhythm data
    GroovinatorRhythmHandler::RhythmSequence rhythm;
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
    
    //printf("rhythm=%s, numSteps=%d, stepButtonsSize=%d\n", rhythmStr.c_str(), numSteps, _stepButtons.size());
    
    // Recreate all the buttons if necessary
    if (numSteps != _stepButtons.size())
    {
        _stepButtons.clear();
        
        for (int i=0; i<numSteps; i++)
        {
            String buttonName;
            String rhythmTypeName = _rhythmType == kOriginalRhythm ? "original" : "target";
            buttonName  << rhythmTypeName << "Step" << i;
            
            GroovinatorUIStepButton* stepButton = new GroovinatorUIStepButton(buttonName, i, numSteps, _rhythmHandler);
            stepButton->addListener(this);
            
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
        stepButtonY = rhythm[i] == 1 ?  stepButtonY : stepButtonY + (getHeight()-stepButtonHeight+1); // Steps at bottom of region
        //stepButtonY = rhythm[i] == 1 ?  stepButtonY : stepButtonY + (getHeight()/2) - (stepButtonHeight/2); // Steps in middle of region
        stepButton->setBounds(stepButtonX, stepButtonY, stepButtonWidth, stepButtonHeight);
        
        stepButton->repaint();
    }
}
