/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundTouch.h"


//==============================================================================
/**
*/
class GroovinatorAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    GroovinatorAudioProcessor();
    ~GroovinatorAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
    
    // Custom getters and setters
    //float getFreq();
    void setTestSliderValue(float v);
    AudioPlayHead::CurrentPositionInfo getPlayHeadInfo();
    double getPlayHeadBpm();
    bool getHasPlayHeadBeenSet();
    int getMostRecentMeasureBufferSample();
    int getMeasureBufferSize();
    double getSoundTouchTempo() {return _soundTouchTempo;}
    int getMeasuresElapsed() {return _measuresElapsed;}
    double getSoundTouchInputOutputRatio() {return _soundTouch.getInputOutputRatio();}
    
    // Utility methods
    int calculateNumSamplesPerMeasure();
    int calculateNumPulsesPerMeasure();
    int calculatePlayHeadRelativePositionInSamples();
    double calculateSecondsPerBeat();
    int getPlayHeadBarNum(); // WIP
    int getPlayHeadRelativePulseNum(); // WIP

private:
    //void updateValuesFromPlayHead();

    //float _freq;
    double _sampleRate;
    
    soundtouch::SoundTouch _soundTouch;
    double _soundTouchTempo;

    AudioPlayHead* _playHead;
    AudioPlayHead::CurrentPositionInfo _playHeadInfo;
    int _prevPlayHeadTimeInSamples;
    bool _hasPlayHeadBeenSet;
    
    AudioSampleBuffer _measureBuffer;
    int _mostRecentMeasureBufferSample;
    double _mostRecentMeasureStartPpq;
    int _measuresElapsed;
    bool _hasMeasureBufferBeenSet;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroovinatorAudioProcessor)
};
