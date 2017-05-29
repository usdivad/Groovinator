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
    _playHead(NULL),
    _hasPlayHeadBeenSet(false)
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
    // Getting host info from playhead (we can only do this from within processBlock,
    // which is why we keep track of playhead info so we can use it elsewhere)
    _playHead = getPlayHead();
    if (_playHead != nullptr)
    {
        _playHead->getCurrentPosition(_playHeadInfo);
        _hasPlayHeadBeenSet = true;
        //_hostBpm = curPosInfo.bpm;
    }
    //printf("host bpm: %f\n", getHostBpm());
    
    // Audio reading and writing
    //
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    
    const int numSamples = buffer.getNumSamples();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        // Pitch shift
        if (numSamples > 0)
        {
            _soundTouch.setSampleRate(getSampleRate());
            //_soundTouch.setChannels(totalNumInputChannels);
            _soundTouch.setChannels(1);
            _soundTouch.putSamples(channelData, numSamples);
            
            int numReceivedSamples = 0;
            int receiveIterationNum = 0;
            
            //if (numReceivedSamples != numSamples)
            //{
            //    printf("put %d samples but only received %d samples\n", numSamples, numReceivedSamples);
            //}
            
            do
            {
                receiveIterationNum++;
                numReceivedSamples = _soundTouch.receiveSamples(channelData, numSamples);
                //printf("%d.%d: received %d of %d samples\n", channel, receiveIterationNum, numReceivedSamples, numSamples);
            }
            while (numReceivedSamples != 0 && numReceivedSamples != numSamples);
        }
    }
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

// Getters and setters
void GroovinatorAudioProcessor::setFreq(float v)
{
    _freq = v;
    _soundTouch.setPitchSemiTones(v);
}

float GroovinatorAudioProcessor::getFreq()
{
    return _freq;
}

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

//void GroovinatorAudioProcessor::updateValuesFromPlayHead()
//{
//    _hostBpm = _playHeadInfo.bpm;
//}
