#include "ParameterWithCallback.h"

ParameterWithCallback::ParameterWithCallback (String parameterID, String name,
                                              float minValue, float maxValue,
                                              float defaultValue, 
                                              float skewValue,
                                              std::function <void (float)> callback)
    : AudioProcessorParameterWithID (parameterID, name), // construct base class
      value (defaultValue), defaultValue (defaultValue), // initialise members
      range (minValue, maxValue, 0.001f, skewValue),
      callback (callback)
{
    // call the callback on construction
    callback (defaultValue);
}
                                              
ParameterWithCallback::~ParameterWithCallback()
{
}
    
// return the value
float ParameterWithCallback::get() const
{
    return value;
}

// allow this class to be casted to a float
// objects of this type can be used wherever you would use a float
ParameterWithCallback::operator float() const
{
    return value;
}

// allow this class to be assigned to using the = symbol
ParameterWithCallback& ParameterWithCallback::operator= (float newValue)
{
    if (value != newValue)
        setValueNotifyingHost (range.convertTo0to1 (newValue));

    return *this;
}
    
// return the value in the range 0 to 1
float ParameterWithCallback::getValue() const
{
    return range.convertTo0to1 (value);
}

// set the value from a value in the range 0 to 1
void ParameterWithCallback::setValue (float newValue)
{
    value = range.convertFrom0to1 (newValue);
    
    // call the callback to inform the plug-in of the parameter change
    callback (value);
}

// return the default value of this parameter in the range 0 to 1
float ParameterWithCallback::getDefaultValue() const
{
    return range.convertTo0to1 (defaultValue);
}

// get the number of steps the parameter has in it's range
int ParameterWithCallback::getNumSteps() const
{
    return AudioProcessorParameterWithID::getNumSteps();
}

// return the parameter value as a string
String ParameterWithCallback::getText (float value, int length) const
{
    String asText (range.convertFrom0to1 (value), 2);
    
    // this is some tasty shorthad (syntactic sugar) for a small if else statement
    // test ? ifTrue : ifFalse;
    return length > 0 ? asText.substring (0, length) : asText;
}

// convert text to a value
float ParameterWithCallback::getValueForText (const String& text) const
{ 
    return range.convertTo0to1 (text.getFloatValue());
}
