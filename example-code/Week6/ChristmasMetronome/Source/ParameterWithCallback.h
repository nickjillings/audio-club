#ifndef PARAMETERWITHCALLBACK_H_INCLUDED
#define PARAMETERWITHCALLBACK_H_INCLUDED

#include <functional>
#include "../JuceLibraryCode/JuceHeader.h"

// A parameter class which calls a callback function whenever its value is changed.
class ParameterWithCallback : public AudioProcessorParameterWithID
{
public:
    // There are a number of constructor parameter.
    // parameterID - a unique ID for this parameter
    // name - the name of this parameter
    // minValue, maxValue, defaultValue - the range of values
    // skewValue - allows us to control the taper of the parameter (log, anti-log etc.)
    // callback - a function to be called when the parameter is changed
    ParameterWithCallback (String parameterID, String name,
                           float minValue, float maxValue,
                           float defaultValue, float skewValue,
                           std::function <void (float)> callback);
    ~ParameterWithCallback();
    
    // Some useful functions to ease the use of the parameter.
    float get() const;
    operator float() const;
    ParameterWithCallback& operator= (float newValue);
    
private:
    // Variables to store the parameter value in.
    float value, defaultValue;

    // This range variable makes it easy to convert between values 
    // in the range 0 to 1 (as the host wants) and the range
    // our plug-in uses.
    NormalisableRange <float> range;    

    // Some C++ magic for holding a function in a variable.
    // Our callback variable can be used as though it were a function.
    std::function <void (float)> callback;
    
    // AudioProcessorParameter interface (see JUCE docs)
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    int getNumSteps() const override;
    String getText (float x, int length) const override;
    float getValueForText (const String &text) const override;
};

#endif  // PARAMETERWITHCALLBACK_H_INCLUDED
