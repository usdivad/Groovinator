/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "../Library/SoundTouch/SoundTouch.h"

#include "GroovinatorRhythmHandler.h"


//==============================================================================
/**
*/
class GroovinatorAudioProcessor : public AudioProcessor
{
public:
    //==============================================================================
    enum ProcessMode
    {
        kSoundTouchPitchShift=1, // ComboBox IDs must be > 0
        kSoundTouchTimeStretch,
        kManualResample,
        kManualConcatenateSteps
    };
    
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
    int getMostRecentMeasureBufferSample(int channel);
    int getMeasureBufferSize();
    double getSoundTouchTempo();
    int getMeasuresElapsed();
    double getSoundTouchInputOutputRatio();
    GroovinatorRhythmHandler& getRhythmHandler();
    String getProcessDebugMessage();
    std::vector<double> getStepStretchRatios(); // TODO: Migrate this to rhythm handler
    ProcessMode getProcessMode();
    
    // ... and custom setters
    void setTestSliderValue(float v);
    void setProcessMode(ProcessMode processMode);
    
    // Utility methods
    int calculateNumSamplesPerMeasure();
    int calculateNumPulsesPerMeasure();
    int calculatePlayHeadRelativePositionInSamples();
    double calculateSecondsPerBeat();
    int getPlayHeadBarNum(); // WIP
    int getPlayHeadRelativePulseNum(); // WIP
    double calculateProportionOfMeasureElapsed(); // 0.0-1.0

private:
    //void updateValuesFromPlayHead();
    
    // Processing methods called by processBlock()
    void preprocessUpdate(AudioPlayHead* playHead, double sampleRate, int numSamples);
    void preprocessClearBuffer(AudioSampleBuffer& buffer);
    void processChannelSoundTouchPitchShift(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples);
    void processChannelSoundTouchTimeStretch(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples);
    void processChannelManualResample(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples);
    void processChannelManualConcatenateSteps(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples);
    
    //==============================================================================
    // Audio playback stuff
    //float _freq;
    double _sampleRate;
    
    soundtouch::SoundTouch _soundTouch;
    double _soundTouchTempo;
    std::vector<double> _stepStretchRatios;
    double _stepStretchRatio;

    AudioPlayHead* _playHead;
    AudioPlayHead::CurrentPositionInfo _playHeadInfo;
    int _prevPlayHeadTimeInSamples;
    bool _hasPlayHeadBeenSet;
    int _playHeadTimeDiff;
    
    AudioSampleBuffer _measureBuffer;
    std::vector<int> _mostRecentMeasureBufferSamplesByChannel;
    double _mostRecentMeasureStartPpq;
    int _measuresElapsed;
    bool _hasMeasureBufferBeenSet;
    
    ProcessMode _processMode;
    
    //==============================================================================
    // Symbolic rhythm
    GroovinatorRhythmHandler _rhythmHandler;
    
    //==============================================================================
    // Debugging
    String _processDebugMessage;
    
    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GroovinatorAudioProcessor)
};
