#ifndef PLUGINPROCESSOR_H_INCLUDED
#define PLUGINPROCESSOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "ParameterWithCallback.h"

// Our plug-in class.
class ChristmasMetronomeAudioProcessor  : public AudioProcessor
{
public:
    ChristmasMetronomeAudioProcessor();
    ~ChristmasMetronomeAudioProcessor();

    // All the AudioProcessor interface functions.
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages) override;

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

    // The parameters are public so the editor can use them.
    static const int numVoices = 2;
    ParameterWithCallback *gainParams [numVoices], *lpfParams [numVoices], *hpfParams [numVoices];
    
private:
    // A variable to store the sample rate in.
    double fs;

    // A synthesiser for playing the sleigh bell sounds.
    Synthesiser sleighBellSynth;
    
    // Some arrays holding information about the parameters.
    int voiceNotes [numVoices];
    float defaultGains [numVoices], defaultLpfFrequencies [numVoices], defaultHpfFrequencies [numVoices];
    String gainIds [numVoices], lpfIds [numVoices], hpfIds [numVoices];
    String gainNames [numVoices], lpfNames [numVoices], hpfNames [numVoices];
    
    // Some functions for setting the parameters of the synthesiser's voices.
    void setVoiceGain (int voice, float gain);
    void setVoiceLpfFrequency (int voice, float frequency);
    void setVoiceHpfFrequency (int voice, float frequency);
    void prepareVoices (double sampleRate, int samplesPerBlock);
    void releaseVoiceResources();
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasMetronomeAudioProcessor)
};

#endif  // PLUGINPROCESSOR_H_INCLUDED
