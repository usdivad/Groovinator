/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GroovinatorAudioProcessor::GroovinatorAudioProcessor() :
#ifndef JucePlugin_PreferredChannelConfigurations
    AudioProcessor (BusesProperties()
                 #if ! JucePlugin_IsMidiEffect
                  #if ! JucePlugin_IsSynth
                   .withInput  ("Input",  AudioChannelSet::stereo(), true)
                  #endif
                   .withOutput ("Output", AudioChannelSet::stereo(), true)
                 #endif
                   ),
#endif
    _soundTouch(),
    _soundTouchTempo(1.0),
    _stepStretchRatio(1.0),

    _playHead(NULL),
    _playHeadInfo(),
    _prevPlayHeadTimeInSamples(0),
    _hasPlayHeadBeenSet(false),

    _measureBuffer(),
    _mostRecentMeasureBufferSample(0),
    _mostRecentMeasureStartPpq(0.0),
    _measuresElapsed(0),
    _hasMeasureBufferBeenSet(false),

    _processMode(kManualConcatenateSteps)
{
}

GroovinatorAudioProcessor::~GroovinatorAudioProcessor()
{
}

//==============================================================================
const String GroovinatorAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool GroovinatorAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool GroovinatorAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double GroovinatorAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int GroovinatorAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int GroovinatorAudioProcessor::getCurrentProgram()
{
    return 0;
}

void GroovinatorAudioProcessor::setCurrentProgram (int index)
{
}

const String GroovinatorAudioProcessor::getProgramName (int index)
{
    return {};
}

void GroovinatorAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void GroovinatorAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void GroovinatorAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool GroovinatorAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void GroovinatorAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // Clear debug message
    _processDebugMessage = "";
    
    // ================================
    // Preprocessing
    double sampleRate = getSampleRate();
    AudioPlayHead* playHead = getPlayHead();
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Update all our member variables (playhead, rhythm handler, measure buffer, etc.)
    preprocessUpdate(playHead, sampleRate, numSamples);
    
    // Clear output channels that don't contain input data
    preprocessClearBuffer(buffer);
    
    // For now, no need to process if playhead isn't moving
    // Eventually a host-independent "live mode" would be nice
    if (_playHeadTimeDiff == 0)
        return;
    
    // ================================
    // Processing
    
    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        
        // (but not if there are no samples)
        if (numSamples == 0)
            continue;

        if (_processMode == kSoundTouchPitchShift) // Pitch shift
        {
            processChannelSoundTouchPitchShift(channelData, buffer, channel, sampleRate, numSamples);
        }
        else if (_processMode == kSoundTouchTimeStretch) // Tempo stretch and preserve buffer
        {
            processChannelSoundTouchTimeStretch(channelData, buffer, channel, sampleRate, numSamples);
        }
        else if (_processMode == kManualResample) // Manually resample
        {
            processChannelManualResample(channelData, buffer, channel, sampleRate, numSamples);
        }
        else if (_processMode == kManualConcatenateSteps) // Concatenate with silence
        {
            processChannelManualConcatenateSteps(channelData, buffer, channel, sampleRate, numSamples);
        }
    }
}

// Processing methods used by processBlock()

void GroovinatorAudioProcessor::preprocessUpdate(AudioPlayHead* playHead, double sampleRate, int numSamples)
{
    // Update sample rate
    _sampleRate = sampleRate;
    
    // Getting host info from playhead (we can only do this from within processBlock,
    // which is why we keep track of playhead info so we can use it elsewhere)
    _playHead = playHead;
    if (_playHead != nullptr)
    {
        _playHead->getCurrentPosition(_playHeadInfo);
        _hasPlayHeadBeenSet = true;
        //_hostBpm = curPosInfo.bpm;
    }
    else
    {
        _hasPlayHeadBeenSet = false;
        return;
    }
    
    // Update rhythm handler
    _rhythmHandler.setProportionOfRhythmElapsed(calculateProportionOfMeasureElapsed());
    
    // Setup SoundTouch
    _soundTouch.setSampleRate(sampleRate);
    //_soundTouch.setChannels(totalNumInputChannels);
    _soundTouch.setChannels(1);
    _soundTouch.setRate(1.0);
    //_soundTouch.setSetting(SETTING_USE_QUICKSEEK, 1);
    
    // Set _stepStretchRatio and _soundTouchTempo (they're set to be equal here, but this is not guaranteed to always be the case)
    double rhythmProportion = _rhythmHandler.getProportionOfRhythmElapsed();
    int rhythmNumSteps = _rhythmHandler.getOriginalNumSteps();
    int rhythmStepIndex = GroovinatorRhythmHandler::proportionToStepIndex(rhythmProportion, rhythmNumSteps);
    std::vector<double> rhythmStepStretchRatios = _rhythmHandler.calculateStepStretchRatios();
    if (rhythmStepIndex >= 0 && rhythmStepIndex < rhythmStepStretchRatios.size())
    {
        _stepStretchRatio = rhythmStepStretchRatios[rhythmStepIndex];
        _soundTouchTempo = _stepStretchRatio;
        //_soundTouchTempo = std::min(std::max(_soundTouchTempo, 0.1), 0.9);
    }
    //_soundTouchTempo = 0.5; // Hard-code this to test (doesn't work yet for >1.0)
    //_soundTouch.setTempo(_soundTouchTempo);
    
    
    // Create measure buffer to store time-stretched data
    //if (/*_playHeadInfo.timeInSamples == 0 ||*/ calculatePlayHeadRelativePositionInSamples() == 0)
    if (calculatePlayHeadRelativePositionInSamples() <= numSamples || _playHeadInfo.ppqPositionOfLastBarStart != _mostRecentMeasureStartPpq || _playHeadInfo.timeInSamples == 0)
    {
        int measureBufferSize = calculateNumSamplesPerMeasure();
        
        if (_processMode == kSoundTouchPitchShift)
        {
            // pass
        }
        if (_processMode == kSoundTouchTimeStretch)
        {
            measureBufferSize = (int) (calculateNumSamplesPerMeasure() * _soundTouch.getInputOutputRatio());
        }
        else if (_processMode == kManualResample)
        {
            double ratioSum;
            double ratioAvg;
            for (size_t i=0; i<rhythmStepStretchRatios.size(); i++)
            {
                ratioSum += rhythmStepStretchRatios[i];
            }
            ratioAvg = ratioSum / rhythmStepStretchRatios.size();
            measureBufferSize = (int) (calculateNumSamplesPerMeasure() / ratioAvg);
        }
        else if (_processMode == kManualConcatenateSteps)
        {
            measureBufferSize = calculateNumSamplesPerMeasure();
        }
        
        _measureBuffer = AudioSampleBuffer(getTotalNumInputChannels(), measureBufferSize);
        _hasMeasureBufferBeenSet = true;
        
        for (int i=0; i<_measureBuffer.getNumChannels(); i++)
            _measureBuffer.clear(i, 0, _measureBuffer.getNumSamples()); // Make sure to clear measure buffer so we don't get a nasty pop when first playing
        
        // Reset most recent _measureBuffer sample index
        //_mostRecentMeasureBufferSample = std::max(calculatePlayHeadRelativePositionInSamples(), 0);
        _mostRecentMeasureBufferSample = 0;
        
        _measuresElapsed++;
    }
    
    // Update most recent measure start position
    _mostRecentMeasureStartPpq = _playHeadInfo.ppqPositionOfLastBarStart;
    
    // Update playhead time diff
    _playHeadTimeDiff = _playHeadInfo.timeInSamples - _prevPlayHeadTimeInSamples;
    _prevPlayHeadTimeInSamples = _playHeadInfo.timeInSamples;
    if (_playHeadTimeDiff == 0)
        //if (_playHeadInfo.timeInSamples == 0 && playHeadTimeDiff == 0)
        //if (_playHeadInfo.timeInSamples == 0)
    {
        _measuresElapsed = 0;
    }
}

void GroovinatorAudioProcessor::preprocessClearBuffer(AudioSampleBuffer& buffer)
{
    // Audio reading and writing
    //
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

void GroovinatorAudioProcessor::processChannelSoundTouchPitchShift(float *channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples)
{
    _soundTouch.setSampleRate(sampleRate);
    //_soundTouch.setChannels(totalNumInputChannels);
    _soundTouch.setChannels(1);
    _soundTouch.setPitch(_soundTouchTempo);
    _soundTouch.putSamples(channelData, numSamples);
    
    int numReceivedSamples = 0;
    int receiveIterationNum = 0;
    
    //if (numReceivedSamples != numSamples)
    //{
    //    printf("put %d samples but only received %d samples\n", numSamples, numReceivedSamples);
    //}
    
    buffer.clear(channel, 0, numSamples);
    
    numReceivedSamples = _soundTouch.receiveSamples(channelData, numSamples);
    
    //do
    //{
    //    receiveIterationNum++;
    //    numReceivedSamples = _soundTouch.receiveSamples(channelData, numSamples);
    //    //printf("%d.%d: received %d of %d samples\n", channel, receiveIterationNum, numReceivedSamples, numSamples);
    //}
    //while (numReceivedSamples != 0 && numReceivedSamples != numSamples);
    ////while (numReceivedSamples != 0);
}

void GroovinatorAudioProcessor::processChannelSoundTouchTimeStretch(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples)
{
    // Set tempo
    _soundTouch.setTempo(_soundTouchTempo);
    
    // Get input samples
    _soundTouch.putSamples(channelData, numSamples);
    
    // Process and write measure buffer samples, if we can
    float* measureChannelData = _measureBuffer.getWritePointer (channel, _mostRecentMeasureBufferSample);
    int totalNumReceivedSamples = 0;
    int numReceivedSamples = 0;
    int receiveIterationNum = 0;
    //int numOutputSamples = (int) (numSamples / _soundTouchTempo);
    int numOutputSamples = (int) (_soundTouch.getInputOutputRatio() * numSamples);
    //int numOutputSamples = (int) (_soundTouchTempo * numSamples);
    bool canWriteToMeasureBuffer = (_measureBuffer.getNumSamples() > _mostRecentMeasureBufferSample+numOutputSamples); // && (_mostRecentMeasureBufferSample+numOutputSamples < calculateNumSamplesPerMeasure())
    if (canWriteToMeasureBuffer)
    {
        // Stretch via SoundTouch
        // Clear measure buffer following most recent sample
        // TODO: Fix behavior here!!
        //_measureBuffer.clear(channel, 0, _measureBuffer.getNumSamples());
        
        // Receive samples from SoundTouch
        //do
        //{
        //    receiveIterationNum++;
        //    numReceivedSamples = _soundTouch.receiveSamples(measureChannelData, numOutputSamples);
        //    //numReceivedSamples = _soundTouch.receiveSamples(tmpChannelData, numOutputSamples);
        //    //printf("%d.%d: received %d of %d samples\n", channel, receiveIterationNum, numReceivedSamples, numSamples);
        //    totalNumReceivedSamples += numReceivedSamples;
        //}
        //while (numReceivedSamples != 0);
        ////while (numReceivedSamples != 0 && numReceivedSamples != numOutputSamples);
        numReceivedSamples = _soundTouch.receiveSamples(measureChannelData, numOutputSamples);
        
        // Update most recent sample index
        //_mostRecentMeasureBufferSample += numOutputSamples;
        _mostRecentMeasureBufferSample += numReceivedSamples;
    }
    
    // Write output samples from measure buffer
    int posInSamples = calculatePlayHeadRelativePositionInSamples();
    //int endPosInSamples = posInSamples + numSamples;
    int endPosInSamples = posInSamples + numReceivedSamples;
    bool canWriteOutput = endPosInSamples < _mostRecentMeasureBufferSample && endPosInSamples < _measureBuffer.getNumSamples();
    if (canWriteOutput)
    {
        // Clear output buffer
        buffer.clear(channel, 0, buffer.getNumSamples());
        
        const float* measureChannelOutputData = _measureBuffer.getReadPointer(channel, posInSamples);
        
        // Write manually
        for (int sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
            //for (int sampleIdx=0; sampleIdx<numReceivedSamples; sampleIdx++)
            //for (int sampleIdx=0; sampleIdx<std::min(numSamples, numReceivedSamples); sampleIdx++)
        {
            channelData[sampleIdx] = measureChannelOutputData[sampleIdx];
        }
    }
}

void GroovinatorAudioProcessor::processChannelManualResample(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples)
{
    // Process and write measure buffer samples, if we can
    float* measureChannelData = _measureBuffer.getWritePointer (channel, _mostRecentMeasureBufferSample);
    int numOutputSamples = (int) (numSamples / _stepStretchRatio);
    bool canWriteToMeasureBuffer = (_measureBuffer.getNumSamples() > _mostRecentMeasureBufferSample+numOutputSamples); // && (_mostRecentMeasureBufferSample+numOutputSamples < calculateNumSamplesPerMeasure())
    if (canWriteToMeasureBuffer)
    {
        // Put samples and add silence (leads to choppiness)
        //_measureBuffer.copyFrom(channel, 0, _measureBuffer, channel, posInSamples, numSamples);
        //                for (size_t sampleIdx=0; sampleIdx<numOutputSamples; sampleIdx++)
        //                {
        //                    if (sampleIdx < numSamples)
        //                    {
        //                        measureChannelData[sampleIdx] = channelData[sampleIdx]; // Sample
        //                    }
        //                    else
        //                    {
        //                        measureChannelData[sampleIdx] = 0.0; // Silence
        //                        measureChannelData[sampleIdx] = channelData[numSamples-1]; // Last sample of measure
        //                    }
        //                }
        
        // Copy input samples according to ratio (leads to pitch shifting, but can sound cool)
        for (size_t outputSampleIdx=0; outputSampleIdx<numOutputSamples; outputSampleIdx++)
        {
            int inputSampleIdx = (int) ((outputSampleIdx/(double)numOutputSamples) * numSamples);
            measureChannelData[outputSampleIdx] = channelData[inputSampleIdx];
        }
        
        // Update most recent sample index
        _mostRecentMeasureBufferSample += numOutputSamples;
    }
    
    // Write output samples from measure buffer
    int posInSamples = calculatePlayHeadRelativePositionInSamples();
    int endPosInSamples = posInSamples + numSamples;
    //int endPosInSamples = posInSamples + numOutputSamples;
    bool canWriteOutput = endPosInSamples < _mostRecentMeasureBufferSample && endPosInSamples < _measureBuffer.getNumSamples();
    if (canWriteOutput)
    {
        // Clear output buffer
        buffer.clear(channel, 0, buffer.getNumSamples());
        
        const float* measureChannelOutputData = _measureBuffer.getReadPointer(channel, posInSamples);
        
        // Write manually
        for (int sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
        {
            channelData[sampleIdx] = measureChannelOutputData[sampleIdx];
        }
    }
}

void GroovinatorAudioProcessor::processChannelManualConcatenateSteps(float* channelData, AudioSampleBuffer& buffer, int channel, double sampleRate, int numSamples)
{
    // Process and write measure buffer samples, if we can
    float* measureChannelData = _measureBuffer.getWritePointer (channel, _mostRecentMeasureBufferSample);
    bool canWriteToMeasureBuffer = (_measureBuffer.getNumSamples() > _mostRecentMeasureBufferSample + numSamples);
    if (canWriteToMeasureBuffer)
    {
        // Simply copy samples from channel to measure
        for (size_t sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
        {
            measureChannelData[sampleIdx] = channelData[sampleIdx];
        }
        
        // Update most recent sample index
        _mostRecentMeasureBufferSample += numSamples;
    }
    
    // Calculate original and target sample positions
    int numSamplesPerOriginalStep = (int) (calculateNumSamplesPerMeasure() / _rhythmHandler.getOriginalNumSteps());
    int numSamplesPerTargetStep = (int) (calculateNumSamplesPerMeasure() / _rhythmHandler.getTargetNumSteps());
    
    int originalStepIdx = (int) (calculateProportionOfMeasureElapsed() * _rhythmHandler.getOriginalNumSteps());
    int targetStepIdx = (int) (calculateProportionOfMeasureElapsed() * _rhythmHandler.getTargetNumSteps());
    
    int proportionOfOriginalStepElapsedInSamples = calculatePlayHeadRelativePositionInSamples() % numSamplesPerOriginalStep;
    double proportionOfStepElapsed = proportionOfOriginalStepElapsedInSamples / (double) numSamplesPerOriginalStep;
    int proportionOfTargetStepElapsedInSamples = (int) (proportionOfStepElapsed * numSamplesPerTargetStep);
    
    bool originalStepIsPulse = _rhythmHandler.getOriginalRhythm()[originalStepIdx] == 1;
    bool targetStepIsPulse = _rhythmHandler.getOriginalRhythm()[targetStepIdx] == 1;
    
    GroovinatorRhythmHandler::RhythmSequence originalRhythm = _rhythmHandler.getOriginalRhythm();
    int mostRecentOriginalPulseIdx = 0;
    int mostRecentOriginalStepIdx = 0;
    for (size_t i=0; i<originalRhythm.size(); i++)
    {
        if ((numSamplesPerOriginalStep * i) + proportionOfOriginalStepElapsedInSamples < _mostRecentMeasureBufferSample)
        {
            mostRecentOriginalStepIdx = i;
            if (originalRhythm[i] == 1)
                mostRecentOriginalPulseIdx = i;
        }
    }
    
    // Clear output buffer
    buffer.clear(channel, 0, numSamples);
    
    // Write output samples from measure buffer
    bool onlyUsePulses = true;
    int posInSamples = (mostRecentOriginalStepIdx * numSamplesPerOriginalStep) + proportionOfTargetStepElapsedInSamples;
    if (onlyUsePulses)
        posInSamples = (mostRecentOriginalPulseIdx * numSamplesPerOriginalStep) + proportionOfTargetStepElapsedInSamples;
//    if (targetStepIsPulse)// && mostRecentOriginalPulseIdx >=0)
    {
        const float* measureChannelOutputData = _measureBuffer.getReadPointer(channel, posInSamples);

        // Write manually
        for (int sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
        {
            channelData[sampleIdx] = measureChannelOutputData[sampleIdx];
        }
    }
    
    _processDebugMessage    << "proportionOfStepElapsed=" << proportionOfStepElapsed
                            << ", posInSamples=" << posInSamples
                            << ", mostRecentOriginalPulseIdx=" << mostRecentOriginalPulseIdx
                            << ", posInSamples=" << posInSamples
                            << "";
    
    
//    int posInSamples = calculatePlayHeadRelativePositionInSamples();
//    int endPosInSamples = posInSamples + numSamples;
//    //int endPosInSamples = posInSamples + numOutputSamples;
//    bool canWriteOutput = endPosInSamples < _mostRecentMeasureBufferSample && endPosInSamples < _measureBuffer.getNumSamples();
//    if (canWriteOutput)
//    {
//        // Clear output buffer
//        buffer.clear(channel, 0, buffer.getNumSamples());
//        
//        const float* measureChannelOutputData = _measureBuffer.getReadPointer(channel, posInSamples);
//        
//        // Write manually
//        for (int sampleIdx=0; sampleIdx<numSamples; sampleIdx++)
//        {
//            channelData[sampleIdx] = measureChannelOutputData[sampleIdx];
//        }
//    }
}

//==============================================================================
bool GroovinatorAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* GroovinatorAudioProcessor::createEditor()
{
    return new GroovinatorAudioProcessorEditor (*this);
}

//==============================================================================
void GroovinatorAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void GroovinatorAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new GroovinatorAudioProcessor();
}

// Getters

//float GroovinatorAudioProcessor::getFreq()
//{
//    return _freq;
//}

double GroovinatorAudioProcessor::getPlayHeadBpm()
{
    return _playHeadInfo.bpm;
}

AudioPlayHead::CurrentPositionInfo GroovinatorAudioProcessor::getPlayHeadInfo()
{
    return _playHeadInfo;
}

bool GroovinatorAudioProcessor::getHasPlayHeadBeenSet()
{
    return _hasPlayHeadBeenSet;
}

int GroovinatorAudioProcessor::getPlayHeadBarNum()
{
    // TODO:
    // - Handle cases where denominator != 4
    // - Handle time signature changes (do we have access to enough info for this to work?)
    return _playHeadInfo.ppqPositionOfLastBarStart / _playHeadInfo.timeSigNumerator;
}

int GroovinatorAudioProcessor::getPlayHeadRelativePulseNum()
{
    return _playHeadInfo.ppqPosition - _playHeadInfo.ppqPositionOfLastBarStart;
}

int GroovinatorAudioProcessor::getMostRecentMeasureBufferSample()
{
    return _mostRecentMeasureBufferSample;
}

int GroovinatorAudioProcessor::getMeasureBufferSize()
{
    return _measureBuffer.getNumSamples();
}

double GroovinatorAudioProcessor::getSoundTouchTempo()
{
    return _soundTouchTempo;
}

int GroovinatorAudioProcessor::getMeasuresElapsed()
{
    return _measuresElapsed;
}

double GroovinatorAudioProcessor::getSoundTouchInputOutputRatio()
{
    return _soundTouch.getInputOutputRatio();
}

GroovinatorRhythmHandler& GroovinatorAudioProcessor::getRhythmHandler()
{
    return _rhythmHandler;
}

 String GroovinatorAudioProcessor::getProcessDebugMessage()
{
    return _processDebugMessage;
}

// Setters
void GroovinatorAudioProcessor::setTestSliderValue(float v)
{
    //_freq = v;
    //_soundTouch.setPitchSemiTones(v);
    
    //_soundTouch.setRate(v);
    
    _soundTouchTempo = v;
    //_soundTouch.setTempo(v);
}

// Utility methods

int GroovinatorAudioProcessor::calculatePlayHeadRelativePositionInSamples()
{
    if (!_hasPlayHeadBeenSet)
        return 0;
    
    int samplesPerMeasure = calculateNumSamplesPerMeasure();
    int pulsesPerMeasure = calculateNumPulsesPerMeasure();
    double relativePositionInPulses = (_playHeadInfo.ppqPosition - _playHeadInfo.ppqPositionOfLastBarStart) / (double) pulsesPerMeasure;
    
    //int relativePositionInSamples = (int) (_sampleRate * calculateSecondsPerBeat() * relativePositionInPulses);
    int relativePositionInSamples = (int) (relativePositionInPulses * samplesPerMeasure);
    relativePositionInSamples = relativePositionInSamples >= samplesPerMeasure ? 0 : relativePositionInSamples;
    return std::max(relativePositionInSamples, 0);
}

int GroovinatorAudioProcessor::calculateNumPulsesPerMeasure()
{
    if (!_hasPlayHeadBeenSet)
        return 0;
    
    return (int) (4.0 * _playHeadInfo.timeSigNumerator / _playHeadInfo.timeSigDenominator);
}

int GroovinatorAudioProcessor::calculateNumSamplesPerMeasure()
{
    if (!_hasPlayHeadBeenSet || _sampleRate == 0)
        return 0;
    
    // BPM
    double secondsPerBeat = calculateSecondsPerBeat();

    // Pulses (beats)
    //double ppqLastBarStart = _playHeadInfo.ppqPositionOfLastBarStart;
    int pulsesPerMeasure = calculateNumPulsesPerMeasure();
    
    // Time since last edit
    //double timeInSeconds = _playHeadInfo.timeInSeconds;
    //int timeInSamples = _playHeadInfo.timeInSamples;
    //double samplesPerSecond = timeInSamples / timeInSeconds;
    
    // ... Or just use sample rate (same result)
    double samplesPerSecond = _sampleRate;
    
    // Calculate number of samples
    int numSamples = (int) (samplesPerSecond * secondsPerBeat * pulsesPerMeasure);
    
    return std::max(numSamples, 0);
}

double GroovinatorAudioProcessor::calculateSecondsPerBeat()
{
    if (!_hasPlayHeadBeenSet)
        return 0.0;

    return 60.0 / _playHeadInfo.bpm;
}

double GroovinatorAudioProcessor::calculateProportionOfMeasureElapsed()
{
    return calculatePlayHeadRelativePositionInSamples() / (double) calculateNumSamplesPerMeasure();
}

//void GroovinatorAudioProcessor::updateValuesFromPlayHead()
//{
//    _hostBpm = _playHeadInfo.bpm;
//}
