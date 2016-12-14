/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


//==============================================================================
/**
*/
class GainPluginAudioProcessorEditor  : public AudioProcessorEditor,
                                        public Slider::Listener,
                                        private Timer
{
public:
    GainPluginAudioProcessorEditor (GainPluginAudioProcessor&);
    ~GainPluginAudioProcessorEditor();

    void timerCallback() override;
    void sliderValueChanged (Slider *slider) override;
    
    //==============================================================================
    void paint (Graphics&) override;
    
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GainPluginAudioProcessor& processor;
    Slider gainSlider;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GainPluginAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
