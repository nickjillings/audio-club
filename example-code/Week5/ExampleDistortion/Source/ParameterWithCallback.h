#ifndef PARAMETERWITHCALLBACK_H_INCLUDED
#define PARAMETERWITHCALLBACK_H_INCLUDED

#include <functional>
#include "../JuceLibraryCode/JuceHeader.h"

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
    NormalisableRange <float> range;    
    std::function <void (float)> callback;
    
    float getValue() const override;
    void setValue (float newValue) override;
    float getDefaultValue() const override;
    int getNumSteps() const override;
    String getText (float value, int length) const override;
    float getValueForText (const String& text) const override;
};

#endif  // PARAMETERWITHCALLBACK_H_INCLUDED