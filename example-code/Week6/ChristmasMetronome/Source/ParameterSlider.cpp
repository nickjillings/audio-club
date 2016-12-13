#include "ParameterSlider.h"

ParameterSlider::ParameterSlider (AudioProcessorParameter& param, LookAndFeel *lookAndFeel)
        // Make the slider a rotary one and store the parameter we are controlling.
        : Slider (Slider::RotaryHorizontalVerticalDrag, Slider::NoTextBox), 
          parameter (param)
{
    // Set the range of the slider and set its look and feel.
    setRange (0.0, 1.0, 0.0);
    setLookAndFeel (lookAndFeel);

    // Start the timer.
    startTimerHz (30);
}

ParameterSlider::~ParameterSlider()
{
}

void ParameterSlider::valueChanged()
{
    // If the user is moving the slider, update the plug-in's parameter
    // and notify the host of the change.
    if (isMouseButtonDown())
        parameter.setValueNotifyingHost (static_cast <float> (getValue()));
}

void ParameterSlider::timerCallback()
{
    const float newValue = parameter.getValue();

    // If the parameter value has been changed externally update the slider's position.
    if (newValue != getValue() && ! isMouseButtonDown())
        setValue (newValue);
}
