/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "SoundTouch.h"

#include "GroovinatorRhythmHandler.h"


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
    
    // Custom getters...
    //float getFreq();
    AudioPlayHead::CurrentPositionInfo getPlayHeadInfo();
    double getPlayHeadBpm();
    bool getHasPlayHeadBeenSet();
    int getMostRecentMeasureBufferSample();
    int getMeasureBufferSize();
    double getSoundTouchTempo();
    int getMeasuresElapsed();
    double getSoundTouchInputOutputRatio();
    GroovinatorRhythmHandler& getRhythmHandler();
    
    // ... and custom setters
    void setTestSliderValue(float v);
    
    // Utility methods
    int calculateNumSamplesPerMeasure();
    int calculateNumPulsesPerMeasure();
    int calculatePlayHeadRelativePositionInSamples();
    double calculateSecondsPerBeat();
    int getPlayHeadBarNum(); // WIP
    int getPlayHeadRelativePulseNum(); // WIP

private:
    //void updateValuesFromPlayHead();

    //==============================================================================
    // Audio playback stuff
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
    // Symbolic rhythm
    GroovinatorRhythmHandler _rhythmHandler;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroovinatorAudioProcessor)
};
