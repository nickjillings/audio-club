#include "GainParameter.h"

GainParameter::GainParameter (String parameterID, String name,
                              float minValue, float maxValue,
                              float defaultValue)
    : AudioParameterFloat (parameterID, name,
                           minValue, maxValue,
                           defaultValue)
{
}

GainParameter::~GainParameter()
{
}
    
String GainParameter::getLabel() const
{
    return "dB";
}
    
float GainParameter::getGainValue()
{
    return Decibels::decibelsToGain (get());
}