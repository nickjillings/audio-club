#ifndef GAINPARAMETER_H_INCLUDED
#define GAINPARAMETER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

class GainParameter : public AudioParameterFloat
{
public:
    GainParameter (String parameterID, String name,
                   float minValue, float maxValue,
                   float defaultValue);
    ~GainParameter();
    
    String getLabel() const override;
    
    float getGainValue();
};

#endif  // GAINPARAMETER_H_INCLUDED
