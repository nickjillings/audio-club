/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "../../../Useful_Bits/ParameterWithCallback.h"

//==============================================================================
/**
*/
class PimpinEqAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    PimpinEqAudioProcessor();
    ~PimpinEqAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

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
    // Somewhere to keep the sample rate
    double fs;
    
    // Here we define the number of bands
    static const int numBands = 3;

    // An array of OwnedArrays to hold the filters in.
    // We need numBands filters for each channel of audio.
    OwnedArray <IIRFilter> filters [numBands];

    // Arrays for the parameters.
    // Each band has frequency, Q and gain parameters.
    ParameterWithCallback *frequencyParams [numBands];
    ParameterWithCallback *qParams [numBands];
    ParameterWithCallback *gainParams [numBands];
    
    // A function to call when parameters are updated.
    void updateFilterCoefficients (int band);

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PimpinEqAudioProcessor)
};


#endif  // PLUGINPROCESSOR_H_INCLUDED
