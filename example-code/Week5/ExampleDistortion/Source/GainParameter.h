#ifndef GAINPARAMETER_H_INCLUDED
#define GAINPARAMETER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// A parameter class to represent dB gain values.
class GainParameter : public AudioParameterFloat
{
public:
    GainParameter (String parameterID, String name,
                   float minValue, float maxValue,
                   float defaultValue);
    ~GainParameter();
    
    // some functions specific to our gain value
    // (it's in dB) see cpp file for more
    String getLabel() const override;    
    float getGainValue();
};

#endif  // GAINPARAMETER_H_INCLUDED
