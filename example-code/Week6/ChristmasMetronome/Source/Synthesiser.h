#ifndef SYNTHESISER_H_INCLUDED
#define SYNTHESISER_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// A synthesiser sound which will apply to downbeats.
class DownBeatSound : public SynthesiserSound
{
public:
    DownBeatSound();
    ~DownBeatSound();

    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};

// A synthesiser sound which will apply to other beats.
class OtherBeatSound : public SynthesiserSound
{
public:
    OtherBeatSound();
    ~OtherBeatSound();

    bool appliesToNote (int midiNoteNumber) override;
    bool appliesToChannel (int midiChannel) override;
};

// A synthesiser voice for playing our sleigh bell sample.
class SleighBellVoice : public SynthesiserVoice
{
public:
    // The voice will apply to a particular note.
    SleighBellVoice (int midiNoteInit);
    ~SleighBellVoice();
    
    // The SynthesiserVoice interface functions.
    bool canPlaySound (SynthesiserSound *sound) override;
    void startNote (int midiNoteNumber, float velocity, 
                    SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void renderNextBlock (AudioSampleBuffer &outputBuffer, int startSample, int numSamples) override;
    
    // Functions for preparing and releasing AudioSource resources.
    void prepareToPlay (int samplesPerBlock, double sampleRate);
    void releaseResources();
    
    // Functions to set the parameters for the synthesis.
    void setGain (float newGain);
    void setLpfFrequency (float newFrequency);
    void setHpfFrequency (float newFrequency);
    
    // Some enumerated note numbers to avoid repetition in the code.
    enum NoteNumbers
    {
        DownBeatNote = 48,
        OtherBeatNote = 50
    };
    
private:
    // Some variables to hold parameters and filters.
    static const int numChannels = 2;
    int midiNote;
    float gain, gainValue;
    double fs;
    IIRFilter lpf [numChannels], hpf [numChannels];
    float lpfFrequency, hpfFrequency;

    // An object which will read audio data from our sleigh bell sample file.
    ScopedPointer <AudioFormatReaderSource> readerSource;

    // An object which will resample the audio data to the host's sample rate.
    ScopedPointer <ResamplingAudioSource> audioSource;

    // An audio buffer to store store the current buffer of audio from the file.
    AudioSampleBuffer currentBuffer;
    
    // Functions to update filter coefficients and reset their internal buffers.
    void updateLpf();
    void updateHpf();
    void resetFilters();
    
    // A structure which holds the data read from the sleigh bell sample file.
    struct AudioLoader
    {
        AudioLoader();
        ~AudioLoader();
        
        // An object which manages the types of audio file we can read.
        AudioFormatManager formatManager;

        // An object which reads audio files.
        ScopedPointer <AudioFormatReader> reader;
    };
    
    // A single instance of the AudioLoader structure is shared across every
    // instance of our SleighBellVoice class. This saves every voice from having to 
    // load the file itself.
    SharedResourcePointer <AudioLoader> sleighBellReader;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SleighBellVoice)
};

#endif  // SYNTHESISER_H_INCLUDED
