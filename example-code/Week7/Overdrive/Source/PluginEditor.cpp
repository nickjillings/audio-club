#include "PluginProcessor.h"
#include "PluginEditor.h"

OverdriveAudioProcessorEditor::OverdriveAudioProcessorEditor (OverdriveAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p)
{
    // thresh slider properties...
    addAndMakeVisible(threshSlider);
    threshSlider.setRange(0.0, 1.0);
    threshSlider.setValue(1.0);
    threshSlider.setBounds (20, 60, 250, 20);
    threshSlider.addListener(this);
    
    // input slider properties...
    addAndMakeVisible(inputGainSlider);
    inputGainSlider.setRange(0.0, 3.0);
    inputGainSlider.setValue(1.0);
    inputGainSlider.setBounds (20, 20, 250, 20);
    inputGainSlider.addListener(this);
    
    // output slider properties...
    addAndMakeVisible(outputGainSlider);
    outputGainSlider.setRange(0.0, 3.0);
    outputGainSlider.setValue(1.0);
	outputGainSlider.setBounds (20, 100, 250, 20);
    outputGainSlider.addListener(this);
    
    setSize (300, 260);
    
    // start the timer to update params...
    startTimerHz (25);
}

OverdriveAudioProcessorEditor::~OverdriveAudioProcessorEditor()
{

}

void OverdriveAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::red);
}

void OverdriveAudioProcessorEditor::resized()
{

}

void OverdriveAudioProcessorEditor::sliderValueChanged (Slider *s)
{
    if(s == &threshSlider)
    {
        *(processor.threshParam) = threshSlider.getValue();
    }
    else if (s == &inputGainSlider)
    {
        *(processor.inputGainParam) = inputGainSlider.getValue();
    }
    else if (s == &outputGainSlider)
    {
        *(processor.outputGainParam) = outputGainSlider.getValue();
    }
}

void OverdriveAudioProcessorEditor::timerCallback()
{
    threshSlider.setValue(*processor.threshParam, dontSendNotification);
}


