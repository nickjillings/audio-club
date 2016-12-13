#ifndef PARAMETERSLIDER_H_INCLUDED
#define PARAMETERSLIDER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// A custom slider class which will update a given parameter in our plug-in.
// We inherit Timer so we can update the slider's position if parameters are changed by
// the host.
class ParameterSlider : public Slider,
                        private Timer
{
public:
    // The constructor takes a reference to the parameter to control and a
    // pointer to the look and feel class to use.
    ParameterSlider (AudioProcessorParameter& param, LookAndFeel *lookAndFeel);
    ~ParameterSlider();

    // This function will be called when the slider is moved by the user.
    // We will use it to update the plug-ins parameters.
    void valueChanged() override;

    // This function will be called periodically so we can update the slider position.
    void timerCallback() override;
    
private:
    // A variable to store a reference to the parameter we are controlling in.
    AudioProcessorParameter& parameter;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ParameterSlider)
};


#endif  // PARAMETERSLIDER_H_INCLUDED
