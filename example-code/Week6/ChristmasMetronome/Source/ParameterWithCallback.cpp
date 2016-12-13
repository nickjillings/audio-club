#include "ParameterWithCallback.h"

ParameterWithCallback::ParameterWithCallback (String parameterID, String name,
                                              float minValue, float maxValue,
                                              float defaultValue, 
                                              float skewValue,
                                              std::function <void (float)> callback)
    : AudioProcessorParameterWithID (parameterID, name), // Construct the base class.
      value (defaultValue), defaultValue (defaultValue), // Initialise members.
      range (minValue, maxValue, 0.001f, skewValue),
      callback (callback)
{
    // Call the callback on construction.
    callback (defaultValue);
}
                                              
ParameterWithCallback::~ParameterWithCallback()
{
}
    
// Return the value.
float ParameterWithCallback::get() const
{
    return value;
}

// Allow this class to be casted to a float.
// Objects of this type can be used wherever you would use a float.
ParameterWithCallback::operator float() const
{
    return value;
}

// Allows this class to be assigned to using the = operator.
// e.g. gainParameter = 4.3f
ParameterWithCallback& ParameterWithCallback::operator= (float newValue)
{
    if (value != newValue)
        setValueNotifyingHost (range.convertTo0to1 (newValue));

    return *this;
}
    
// Return the value in the range 0 to 1.
float ParameterWithCallback::getValue() const
{
    return range.convertTo0to1 (value);
}

// Set the value from a value in the range 0 to 1.
void ParameterWithCallback::setValue (float newValue)
{
    value = range.convertFrom0to1 (newValue);

    // Call the callback to react to the parameter update.
    callback (value);
}

// Return the default parameter in the range 0 to 1.
float ParameterWithCallback::getDefaultValue() const
{
    return range.convertTo0to1 (defaultValue);
}

// Get the number of steps the parameter has in its range.
int ParameterWithCallback::getNumSteps() const
{
    return AudioProcessorParameterWithID::getNumSteps();
}

// Return the parameter value as a string.
String ParameterWithCallback::getText (float x, int length) const
{
    String asText (range.convertFrom0to1 (x), 2);

    // If a shorter string is required, shorten the string accordingly.
    return length > 0 ? asText.substring (0, length) : asText;
}

// Convert a string to a value in the range 0 to 1.
float ParameterWithCallback::getValueForText (const String &text) const
{ 
    return range.convertTo0to1 (text.getFloatValue());
}
