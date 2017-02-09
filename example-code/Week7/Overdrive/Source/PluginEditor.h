

#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"


class OverdriveAudioProcessorEditor  : public AudioProcessorEditor,
                                       public Slider::Listener,
                                       private Timer
{
public:
    OverdriveAudioProcessorEditor (OverdriveAudioProcessor&);
    ~OverdriveAudioProcessorEditor();

    void paint (Graphics&) override;
    void resized() override;
    
    void sliderValueChanged (Slider *slider) override;
    void timerCallback() override;
    

private:
    OverdriveAudioProcessor& processor;
    Slider threshSlider;
    Slider inputGainSlider;
    Slider outputGainSlider;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdriveAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED

