/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DankDelayAudioProcessorEditor::DankDelayAudioProcessorEditor (DankDelayAudioProcessor& p)
    : AudioProcessorEditor (&p), processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
}

DankDelayAudioProcessorEditor::~DankDelayAudioProcessorEditor()
{
}

//==============================================================================
void DankDelayAudioProcessorEditor::paint (Graphics& g)
{
    g.fillAll (Colours::deepskyblue);
}

void DankDelayAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}
