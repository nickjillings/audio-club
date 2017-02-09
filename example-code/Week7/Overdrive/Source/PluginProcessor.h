
#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED
#include "../JuceLibraryCode/JuceHeader.h"

class OverdriveAudioProcessor  : public AudioProcessor
{
public:
    OverdriveAudioProcessor();
    ~OverdriveAudioProcessor();

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    // ---- Processing functions -----
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void processBlock (AudioSampleBuffer&, MidiBuffer&) override;
    void releaseResources() override;
    // ---- Processing functions -----
    
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    const String getName() const override;
    bool acceptsMidi() const override;
    bool producesMidi() const override;
    double getTailLengthSeconds() const override;

    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    // declare thresh param object...
    AudioParameterFloat *threshParam;
    AudioParameterFloat *inputGainParam;
    AudioParameterFloat *outputGainParam;


private:
    double threshold;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OverdriveAudioProcessor)
};

#endif



