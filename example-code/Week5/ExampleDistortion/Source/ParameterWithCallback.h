#ifndef PARAMETERWITHCALLBACK_H_INCLUDED
#define PARAMETERWITHCALLBACK_H_INCLUDED

#include <functional>
#include "../JuceLibraryCode/JuceHeader.h"

// A parameter class which calls a callback whenever it's value is changed.
class ParameterWithCallback : public AudioProcessorParameterWithID
{
public:
    ParameterWithCallback (String parameterID, String name,
                           float minValue, float maxValue,
                           float defaultValue, float skewValue,
                           std::function <void (float)> callback);
    ~ParameterWithCallback();
    
    float get() const;
    operator float() const;
    ParameterWithCallback& operator= (float newValue);
    
private:
    float value, defaultValue;

    // this range variable makes it easy to convert between
    // values in the range 0 to 1 (as the host wants) and the
    // range our plug-in wants
    NormalisableRange <float> range;

    // some c++ magic for holding a function in a variable
    // our callback variable can be used as though it were a function
    std::function <void (float)> callback; 
    
    // AudioProcessorParameter interface
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    int getNumSteps() const override;
    String getText (float value, int length) const override;
    float getValueForText (const String& text) const override;
};

#endif  // PARAMETERWITHCALLBACK_H_INCLUDED
