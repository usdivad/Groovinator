/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GroovinatorAudioProcessorEditor::GroovinatorAudioProcessorEditor (GroovinatorAudioProcessor& p)
:   AudioProcessorEditor (&p),
    processor (p),
    _originalRhythmBgRect(GroovinatorUIRhythmComponent::kOriginalRhythm, processor.getRhythmHandler()),
    _targetRhythmBgRect(GroovinatorUIRhythmComponent::kTargetRhythm, processor.getRhythmHandler())
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (640, 396);
    
    //==============================================================================
    // Look and feel
    getLookAndFeel().setColour(TextButton::buttonColourId, Colours::white);
    getLookAndFeel().setColour(TextButton::textColourOffId, Colour::fromString("ff6d6d6d"));
    
    //==============================================================================
    // Test slider (pitch-shift, rate, etc. depending on the need...)
    _testSlider.setSliderStyle(Slider::LinearVertical);
    //_testSlider.setRange(-24.0, 24.0); // Semitones
    _testSlider.setRange(0.1, 3.0); // Rate
    _testSlider.setTextBoxStyle(Slider::TextBoxBelow, false, 50, 20);
    //_testSlider.showTextBox();
    _testSlider.setPopupDisplayEnabled(true, true, this);
    //_testSlider.setTextValueSuffix(" (test)");
    //_testSlider.setValue(0.0); // Semitones
    _testSlider.setValue(1.0); // Rate
    _testSlider.addListener(this);
    
    // Playhead info label
    _playHeadInfoLabel.setText("", dontSendNotification);
    _playHeadInfoLabel.setJustificationType(Justification::centred);
    
    // Debug label
    _debugLabel.setFont(10.0f);
    _debugLabel.setText("", dontSendNotification);
    
    // Process mode selector
    _processModeSelector.addItem("Classic Stretch", GroovinatorAudioProcessor::kSoundTouchTimeStretch);
    _processModeSelector.addItem("Sample 'n' Shift", GroovinatorAudioProcessor::kManualResample);
    _processModeSelector.addItem("Step Repeater", GroovinatorAudioProcessor::kManualConcatenateSteps);
    //_processModeSelector.addItem("Classic time-stretch", GroovinatorAudioProcessor::kSoundTouchTimeStretch);
    _processModeSelector.setJustificationType(Justification::centred);
    _processModeSelector.setColour(ComboBox::backgroundColourId, Colour::fromString("ff404040"));
    _processModeSelector.setSelectedId(GroovinatorAudioProcessor::kSoundTouchTimeStretch);
    _processModeSelector.addListener(this);
    
    //==============================================================================
    //Rhythm
    
    // Labels
    _originalRhythmLabel.setText("Original rhythm", dontSendNotification);
    _targetRhythmLabel.setText("Target rhythm", dontSendNotification);
    
    // Rectangles
    // _originalRhythmBgRect and _targetRhythmBgRect; they're now handled inside GroovinatorUIRhythmComponent
    
    // Num steps
    _originalNumStepsSlider.setSliderStyle(Slider::IncDecButtons);
    _originalNumStepsSlider.setRange(1, 99, 1);
    _originalNumStepsSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 30, 20);
    _originalNumStepsSlider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection);
    _originalNumStepsSlider.setColour(Slider::textBoxBackgroundColourId, Colour::fromString("ff404040"));
    _originalNumStepsSlider.setValue(8);
    _originalNumStepsSlider.addListener(this);
    _originalNumStepsLabel.setText("# steps", dontSendNotification);
    
    _targetNumStepsSlider.setSliderStyle(Slider::IncDecButtons);
    _targetNumStepsSlider.setRange(1, 99, 1);
    _targetNumStepsSlider.setTextBoxStyle(Slider::TextBoxLeft, false, 30, 20);
    _targetNumStepsSlider.setIncDecButtonsMode(Slider::incDecButtonsDraggable_AutoDirection);
    _targetNumStepsSlider.setColour(Slider::textBoxBackgroundColourId, Colour::fromString("ff404040"));
    _targetNumStepsSlider.setValue(8);
    _targetNumStepsSlider.addListener(this);
    _targetNumStepsLabel.setText("# steps", dontSendNotification);
    
    // Add components to editor
    //addAndMakeVisible(&_testSlider);
    addAndMakeVisible(&_debugLabel);
    addAndMakeVisible(&_playHeadInfoLabel);
    addAndMakeVisible(&_originalRhythmLabel);
    addAndMakeVisible(&_targetRhythmLabel);
    addAndMakeVisible(&_originalRhythmBgRect);
    addAndMakeVisible(&_targetRhythmBgRect);
    addAndMakeVisible(&_originalNumStepsSlider);
    addAndMakeVisible(&_originalNumStepsLabel);
    addAndMakeVisible(&_targetNumStepsSlider);
    addAndMakeVisible(&_targetNumStepsLabel);
    addAndMakeVisible(&_processModeSelector);
    
    // Start timer
    startTimer(100);
}

GroovinatorAudioProcessorEditor::~GroovinatorAudioProcessorEditor()
{
}

//==============================================================================
void GroovinatorAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    //g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    g.fillAll(Colours::black);
    
    // TODO: Use LookAndFeel for all the colors etc.
    
    // Title
    g.setColour (Colours::white);
    g.setFont (24.0f);
    g.drawFittedText ("G R O O V I N A T O R", 0, 0, getWidth(), 30, Justification::centred, 1);
    
    // Rhythm boxes
    //g.setColour(Colours::darkslategrey);
    //g.fillRect(_originalRhythmBgRect);
    //g.fillRect(_targetRhythmBgRect);
}

void GroovinatorAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    _testSlider.setBounds(5, 80, 10, (getHeight()/2));
    _playHeadInfoLabel.setBounds(0, 40, getWidth(), 20);
    _debugLabel.setBounds(0, getHeight()-33, getWidth(), 33);
    
    
    double processModeSelectorWidth = getWidth()/4;
    double processModeSelectorHeight = 20;
    _processModeSelector.setBounds((getWidth()/2) - (processModeSelectorWidth/2), 80, processModeSelectorWidth, processModeSelectorHeight);
    
    
    // Rhythm components
    // Component positions will generally go in the format:
    // .setBounds(<componentName>X, <original/target>RhythmY + <componentName>YOffset, <componentName>Width, <componentName>Height)
    
    // Labels
    //double rhythmLabelX = 20;
    double rhythmLabelX = (getWidth()/2.0) - (480/2.0);
    double rhythmLabelYOffset = 0;
    double rhythmLabelWidth = 200;
    double rhythmLabelHeight = 20;
    
    // Rectangles
    double rhythmBgRectX = rhythmLabelX;
    double rhythmBgRectYOffset = rhythmLabelYOffset + rhythmLabelHeight + 5;
    //double rhythmBgRectWidth = (getWidth() - (2*rhythmBgRectX)) * 0.8;
    double rhythmBgRectWidth = 480;
    double rhythmBgRectHeight = 99;
    
    // Num steps labels and sliders
    double numStepsWidth = 60;
    //double numStepsX = getWidth() - rhythmBgRectX - numStepsWidth; // rhythmBgRectX + rhythmBgRectWidth + 10;
    double numStepsX = getWidth() - 20 - numStepsWidth;
    
    double numStepsLabelYOffset = 0;
    double numStepsLabelWidth = numStepsWidth;
    double numStepsLabelHeight = 20;
    
    double numStepsSliderYOffset = 20;
    double numStepsSliderWidth = numStepsWidth;
    double numStepsSliderHeight = 50;
    
    // Overall rhythm position
    double originalRhythmY = 100;
    double targetRhythmY = originalRhythmY + + rhythmLabelHeight + rhythmBgRectHeight + 33;
    
    _originalRhythmLabel.setBounds(rhythmLabelX, originalRhythmY + rhythmLabelYOffset, rhythmLabelWidth, rhythmLabelHeight);
    _targetRhythmLabel.setBounds(rhythmLabelX, targetRhythmY + rhythmLabelYOffset, rhythmLabelWidth, rhythmLabelHeight);
    
    _originalRhythmBgRect.setBounds(rhythmBgRectX, originalRhythmY + rhythmBgRectYOffset, rhythmBgRectWidth, rhythmBgRectHeight);
    _targetRhythmBgRect.setBounds(rhythmBgRectX, targetRhythmY + rhythmBgRectYOffset, rhythmBgRectWidth, rhythmBgRectHeight);
    
    _originalNumStepsLabel.setBounds(numStepsX, originalRhythmY + numStepsLabelYOffset, numStepsLabelWidth, numStepsLabelHeight);
    _originalNumStepsSlider.setBounds(numStepsX, originalRhythmY + numStepsSliderYOffset, numStepsSliderWidth, numStepsSliderHeight);
    _targetNumStepsLabel.setBounds(numStepsX, targetRhythmY + numStepsLabelYOffset, numStepsLabelWidth, numStepsLabelHeight);
    _targetNumStepsSlider.setBounds(numStepsX, targetRhythmY + numStepsSliderYOffset, numStepsSliderWidth, numStepsSliderHeight);
}

//
void GroovinatorAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    double v = slider->getValue();
    if (slider == &_testSlider)
    {
        processor.setTestSliderValue(v);
        printf("test slider changed\n");
    }
    else if (slider == &_originalNumStepsSlider)
    {
        GroovinatorRhythmHandler& rhythmHandler = processor.getRhythmHandler();
        rhythmHandler.setOriginalNumSteps((int) v);
        _originalRhythmBgRect.setRhythmHandler(rhythmHandler);
        _originalRhythmBgRect.repaint();
        
        printf("%s\n", rhythmHandler.getOriginalRhythmStr().c_str());
        printf("original num steps slider changed\n");
    }
    else if (slider == &_targetNumStepsSlider)
    {
        GroovinatorRhythmHandler& rhythmHandler = processor.getRhythmHandler();
        rhythmHandler.setTargetNumSteps((int) v);
        _targetRhythmBgRect.setRhythmHandler(rhythmHandler);
        _targetRhythmBgRect.repaint();
        
        printf("%s\n", rhythmHandler.getTargetRhythmStr().c_str());
        printf("target num steps slider changed\n");
    }
}

void GroovinatorAudioProcessorEditor::comboBoxChanged(juce::ComboBox *comboBox)
{
    if (comboBox == &_processModeSelector)
    {
        // Set process mode
        GroovinatorAudioProcessor::ProcessMode processMode = (GroovinatorAudioProcessor::ProcessMode) comboBox->getSelectedId();
        processor.setProcessMode(processMode);
        
        // Show or hide target rhythm components depending on mode
        bool showTarget = (processMode != GroovinatorAudioProcessor::kManualConcatenateSteps);
        _targetRhythmLabel.setVisible(showTarget);
        _targetRhythmBgRect.setVisible(showTarget);
        _targetNumStepsLabel.setVisible(showTarget);
        _targetNumStepsSlider.setVisible(showTarget);
    }
}

void GroovinatorAudioProcessorEditor::timerCallback()
{
    // Set playhead info label text
    //_playHeadInfoLabel.setText(String(processor.getHostBpm(), 0), sendNotification);
    String playHeadLabelText;
    if (processor.getHasPlayHeadBeenSet())
    {
        AudioPlayHead::CurrentPositionInfo playHeadInfo = processor.getPlayHeadInfo();

        playHeadLabelText << "BPM: "                  << String((int)playHeadInfo.bpm)
                          << " | Meter: "             << String(playHeadInfo.timeSigNumerator)
                                                      << "/" << String(playHeadInfo.timeSigDenominator)
        
                          //<< " | Position: "          << String(processor.getPlayHeadBarNum())
                          //                            << "." << String(processor.getPlayHeadRelativePulseNum());
        
                          << " | Position: "          << String(playHeadInfo.ppqPosition, 2)
                          << " | Last Bar Start: "    << String((int)playHeadInfo.ppqPositionOfLastBarStart);
        
                          //<< " | Time: " << String((int)playHeadInfo.timeInSeconds)
                          //<< "s (" << String(playHeadInfo.timeInSamples) << " samples)";
    }
    else
    {
        playHeadLabelText = "Start playback to make playhead info available";
    }
    _playHeadInfoLabel.setText(playHeadLabelText, sendNotification);
    
    // Set debug label text
    /*
    String debugLabelText = "";
    GroovinatorRhythmHandler rhythmHandler = processor.getRhythmHandler();
    
    // TODO: Figure out why this crashes sometimes when releasing String
    debugLabelText  << "// "
    
                    // Audio
                    << "sampsPerMeasure=" << String(processor.calculateNumSamplesPerMeasure())
                    //<< ", pulses per measure: " << String(processor.calculateNumPulsesPerMeasure())
                    << ", posSamps=" << String(processor.calculatePlayHeadRelativePositionInSamples())
                    << ", mostRecentSamp(0)=" << String(processor.getMostRecentMeasureBufferSample(0))
                    << ", diff(0)=" << String(processor.getMostRecentMeasureBufferSample(0)-processor.calculatePlayHeadRelativePositionInSamples())
                    << ", soundTouchTempo=" << String(processor.getSoundTouchTempo())
                    << ", measureBufSz=" << String(processor.getMeasureBufferSize())
                    //<< "\n"
                    << ", measureNum=" << String(processor.getMeasuresElapsed())
                    << ", ioRatio=" << String(processor.getSoundTouchInputOutputRatio(), 2)
                    << ", " << processor.getProcessDebugMessage()
    
                    // Rhythm
                    << "\n"
                    << "origRhythm=" << rhythmHandler.getOriginalRhythmStr()
                    << ", trgtRhythm=" << rhythmHandler.getTargetRhythmStr()
                    << ", stepStretchRatios=" << GroovinatorRhythmHandler::stepStretchRatiosToString(processor.getStepStretchRatios())
    
                    << " //";
    _debugLabel.setText(debugLabelText, sendNotification);
    //_debugLabel.toFront(false);
    */
}
