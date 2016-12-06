#include "ParameterWithCallback.h"

ParameterWithCallback::ParameterWithCallback (String parameterID, String name,
                                              float minValue, float maxValue,
                                              float defaultValue, 
                                              float skewValue,
                                              std::function <void (float)> callback)
    : AudioProcessorParameterWithID (parameterID, name),
      value (defaultValue), defaultValue (defaultValue),
      range (minValue, maxValue, 0.001f, skewValue),
      callback (callback)
{
    callback (defaultValue);
}
                                              
ParameterWithCallback::~ParameterWithCallback()
{
}
    
float ParameterWithCallback::get() const
{
    return value;
}

ParameterWithCallback::operator float() const
{
    return value;
}

ParameterWithCallback& ParameterWithCallback::operator= (float newValue)
{
    if (value != newValue)
        setValueNotifyingHost (range.convertTo0to1 (newValue));

    return *this;
}
    
float ParameterWithCallback::getValue() const
{
    return range.convertTo0to1 (value);
}

void ParameterWithCallback::setValue (float newValue)
{
    value = range.convertFrom0to1 (newValue);
    callback (value);
}

float ParameterWithCallback::getDefaultValue() const
{
    return range.convertTo0to1 (defaultValue);
}

int ParameterWithCallback::getNumSteps() const
{
    return AudioProcessorParameterWithID::getNumSteps();
}

String ParameterWithCallback::getText (float value, int length) const
{
    String asText (range.convertFrom0to1 (value), 2);
    return length > 0 ? asText.substring (0, length) : asText;
}

float ParameterWithCallback::getValueForText (const String& text) const
{ 
    return range.convertTo0to1 (text.getFloatValue());
}