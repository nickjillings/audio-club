#ifndef PLUGINEDITOR_H_INCLUDED
#define PLUGINEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LookAndFeel.h"
#include "ParameterSlider.h"

// Our plug-in's editor class.
class ChristmasMetronomeAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    ChristmasMetronomeAudioProcessorEditor (ChristmasMetronomeAudioProcessor& owner);
    ~ChristmasMetronomeAudioProcessorEditor();

    void paint (Graphics &g) override;

private:
    // Hold a reference to the processor so we can set its parameters.
    ChristmasMetronomeAudioProcessor& processor;
    
    // An image to draw on the background of the editor.
    ScopedPointer <Drawable> background;
    int width, height;
    
    // Some sliders to control various parameters.
    ScopedPointer <ParameterSlider> gainSliders [ChristmasMetronomeAudioProcessor::numVoices];
    ScopedPointer <ParameterSlider> lpfSliders [ChristmasMetronomeAudioProcessor::numVoices];
    ScopedPointer <ParameterSlider> hpfSliders [ChristmasMetronomeAudioProcessor::numVoices];

    // Some look and feel objects to apply to the different types of slider.
    ChristmasLookAndFeel gainLook, lpfLook, hpfLook;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasMetronomeAudioProcessorEditor)
};


#endif  // PLUGINEDITOR_H_INCLUDED
