/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
GainPluginAudioProcessorEditor::GainPluginAudioProcessorEditor (GainPluginAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    
    addAndMakeVisible (gainSlider);
    gainSlider.setBounds (10, 10, 200, 20);
    gainSlider.setRange (-20.0, 20.0, 0.0);
    gainSlider.addListener (this);
    
    startTimerHz (25);
}

GainPluginAudioProcessorEditor::~GainPluginAudioProcessorEditor()
{
}

//==============================================================================
void GainPluginAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::white);
}

void GainPluginAudioProcessorEditor::timerCallback()
{
    gainSlider.setValue ((float) *processor.gainParameter, dontSendNotification);
}

void GainPluginAudioProcessorEditor::sliderValueChanged (Slider *slider)
{
    if (slider == &gainSlider)
        *processor.gainParameter = (float) gainSlider.getValue();
}
