/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class GroovinatorAudioProcessorEditor  : public AudioProcessorEditor,
                                         public Timer,
                                         private Slider::Listener
{
public:
    GroovinatorAudioProcessorEditor (GroovinatorAudioProcessor&);
    ~GroovinatorAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    
    virtual void timerCallback() override;

private:
    // Listener handlers
    void sliderValueChanged(Slider* slider) override;
    
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GroovinatorAudioProcessor& processor;
    
    // UI components
    Slider _freqSlider;
    Label _bpmLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroovinatorAudioProcessorEditor)
};
