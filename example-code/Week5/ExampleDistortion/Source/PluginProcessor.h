/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// we need to include the headers of the parameter types we made
#include "GainParameter.h"
#include "ParameterWithCallback.h"

//==============================================================================
/**
*/
class ExampleDistortionAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    ExampleDistortionAudioProcessor();
    ~ExampleDistortionAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

private:
    //==============================================================================
    double fs; // we will store the sample rate here for calculating filter coefficients
    GainParameter *driveParam, *gainParam; // two gain parameters
    IIRFilter lpf; // an IIR filter for the tone control
    ParameterWithCallback *toneParam; // a parameter for the tone control
    
    // a function to calculate the filter coefficients for a given cut of frequency
    void setFilterFrequency (float frequency);
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ExampleDistortionAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
