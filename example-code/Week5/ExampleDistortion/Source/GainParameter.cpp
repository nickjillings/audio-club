#include "GainParameter.h"

GainParameter::GainParameter (String parameterID, String name,
                              float minValue, float maxValue,
                              float defaultValue)
    : AudioParameterFloat (parameterID, name,
                           minValue, maxValue,
                           defaultValue) // Passing the constructor arguments to 
                                         // AudioParameterFloat's constructor.
{
}

GainParameter::~GainParameter()
{
}
    
// Return a label that hosts should put after this parameter value.
// Useful for things like units.
String GainParameter::getLabel() const
{
    return "dB";
}
    
// Return the parameter value as a gain which can be multiplied by a signal
// rather than a dB value.
float GainParameter::getGainValue()
{
    // JUCE has handy dB functions in it.
    return Decibels::decibelsToGain (get());
}
