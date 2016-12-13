#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Synthesiser.h"

ChristmasMetronomeAudioProcessor::ChristmasMetronomeAudioProcessor()
    // We have a lot of stuff to initialise in this class.
    : AudioProcessor (BusesProperties().withOutput ("Output", AudioChannelSet::stereo(), true)),
      fs (44100.0f),
      voiceNotes {SleighBellVoice::DownBeatNote, SleighBellVoice::OtherBeatNote},
      defaultGains {0.0f, 0.0f},
      defaultLpfFrequencies {20000.0f, 10000.0f},
      defaultHpfFrequencies {5000.0f, 3000.0f},
      gainIds {"dbgain", "obgain"},
      lpfIds {"dblpf", "oblpf"},
      hpfIds {"dbhpf", "obhpf"},
      gainNames {"Down Beat Gain", "Other Beat Gain"},
      lpfNames {"Down Beat LPF Frequency", "Other Beat LPF Frequency"},
      hpfNames {"Down Beat HPF Frequency", "Other Beat HPF Frequency"}
{  
    // Loop through all the voices we want the synthesiser to have.
    for (int i = 0; i < numVoices; ++i)
    {
        // Add a voice to the synthesiser.
        sleighBellSynth.addVoice (new SleighBellVoice (voiceNotes [i]));
        
        // Create a gain parameter for this voice.
        auto gainCallback = [this, i] (float gain) {setVoiceGain (i, Decibels::decibelsToGain (gain));};
        addParameter (gainParams [i] = new ParameterWithCallback (gainIds [i], gainNames [i],
                                                                  -20.0f, 3.0f,
                                                                  defaultGains [i], 1.0f,
                                                                  gainCallback));
                                                         
        // Create an lpf parameter for this voice.
        auto lpfCallback = [this, i] (float frequency) {setVoiceLpfFrequency (i, frequency);};
        addParameter (lpfParams [i] = new ParameterWithCallback (lpfIds [i], lpfNames [i],
                                                                 3000.0f, 20000.0f,
                                                                 defaultLpfFrequencies [i], 1.0f / 3.0f,
                                                                 lpfCallback));
                                                        
        // Create a hpf parameter for this voice.
        auto hpfCallback = [this, i] (float frequency) {setVoiceHpfFrequency (i, frequency);};
        addParameter (hpfParams [i] = new ParameterWithCallback (hpfIds [i], hpfNames [i],
                                                                 3000.0f, 10000.0f,
                                                                 defaultHpfFrequencies [i], 1.0f / 3.0f,
                                                                 hpfCallback));
    }
       
    // Add some sounds to the synthesiser.
    sleighBellSynth.addSound (new DownBeatSound); 
    sleighBellSynth.addSound (new OtherBeatSound);                                                    
}

ChristmasMetronomeAudioProcessor::~ChristmasMetronomeAudioProcessor()
{
}

const String ChristmasMetronomeAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ChristmasMetronomeAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ChristmasMetronomeAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double ChristmasMetronomeAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ChristmasMetronomeAudioProcessor::getNumPrograms()
{
    return 1;           
}

int ChristmasMetronomeAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ChristmasMetronomeAudioProcessor::setCurrentProgram (int /*index*/)
{
}

const String ChristmasMetronomeAudioProcessor::getProgramName (int /*index*/)
{
    return String();
}

void ChristmasMetronomeAudioProcessor::changeProgramName (int /*index*/, const String& /*newName*/)
{
}

void ChristmasMetronomeAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Save the sample rate.
    fs = sampleRate;
    
    // Inform the synthesiser of the playback parameters.
    sleighBellSynth.setCurrentPlaybackSampleRate (sampleRate);
    prepareVoices (sampleRate, samplesPerBlock);
}

void ChristmasMetronomeAudioProcessor::releaseResources()
{
    // Release the resources the voices allocated.
    releaseVoiceResources();
}

bool ChristmasMetronomeAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const AudioChannelSet& mainOutput = layouts.getMainOutputChannelSet();

    // Our plug-in only supports mono or stereo output.
    if (mainOutput.size() > 2) 
        return false;
        
    return true;
}

void ChristmasMetronomeAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{   
    // Get the current information about the play head.
    AudioPlayHead::CurrentPositionInfo head;
    getPlayHead()->getCurrentPosition (head);
    
    // If the playhead is not moving don't do anything.
    if (! head.isPlaying)
        return;
        
    // We don't want any external MIDI getting into our synth.
    midiMessages.clear();
    
    // Find out some stuff about the time signature and bpm.
    double crotchetsPerMinute = head.bpm;
    double samplesPerCrotchet = fs * 60.0 / crotchetsPerMinute;
    double crotchetsPerBeat = 4.0 / head.timeSigDenominator;
    double samplesPerBeat = samplesPerCrotchet * crotchetsPerBeat;
    
    int timeSig = head.timeSigNumerator;
    
    // Work out what beat of the current bar we are on and at what sample we should
    // play a note from our synthesiser.
    double beatInBar = (head.ppqPosition - head.ppqPositionOfLastBarStart) / crotchetsPerBeat;
    int beatInteger = static_cast <int> (floor (beatInBar));
    double beatFraction = beatInBar - beatInteger;
    double beatSample = - samplesPerBeat * beatFraction;
    
    // Make sure the sample we are playing on is in or after the current buffer.
    while (beatSample < 0)
    {
        beatSample += samplesPerBeat;
        ++beatInteger;
    }
    
    const int numSamples = buffer.getNumSamples();
    
    // Loop through position at which we need to play a note from the synthesiser.
    for (; beatSample < numSamples; beatSample += samplesPerBeat, ++beatInteger)
    {
        // If the current beat is a downbeat add a downbeat note to the MIDI buffer.
        if (beatInteger % timeSig == 0)
            midiMessages.addEvent (MidiMessage::noteOn (1, SleighBellVoice::DownBeatNote, static_cast <uint8> (127)),
                                   static_cast <int> (round (beatSample)));
        // If not add an other beat note.
        else
            midiMessages.addEvent (MidiMessage::noteOn (1, SleighBellVoice::OtherBeatNote, static_cast <uint8> (127)),
                                   static_cast <int> (round (beatSample)));
    }
    
    // Pass the MIDI data to the synthesiser so it can make the right noises.
    sleighBellSynth.renderNextBlock (buffer, midiMessages, 0, numSamples);
    
    // Stop any of the midi we generated from escaping the plug-in.
    midiMessages.clear();
}

bool ChristmasMetronomeAudioProcessor::hasEditor() const
{
    return true;
}

AudioProcessorEditor* ChristmasMetronomeAudioProcessor::createEditor()
{
    return new ChristmasMetronomeAudioProcessorEditor (*this);
}

void ChristmasMetronomeAudioProcessor::getStateInformation (MemoryBlock& /*destData*/)
{
}

void ChristmasMetronomeAudioProcessor::setStateInformation (const void* /*data*/, int /*sizeInBytes*/)
{
}

// Set the gain of one of the voices.
void ChristmasMetronomeAudioProcessor::setVoiceGain (int voice, float gain)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (voice));
    v->setGain (gain);
}

// Set the lpf frequency of one of the voices.
void ChristmasMetronomeAudioProcessor::setVoiceLpfFrequency (int voice, float frequency)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (voice));
    v->setLpfFrequency (frequency);
}

// Set the hpf frequency of one of the voices.
void ChristmasMetronomeAudioProcessor::setVoiceHpfFrequency (int voice, float frequency)
{
    SleighBellVoice *v = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (voice));
    v->setHpfFrequency (frequency);
}

// Give all the voices the sample rate so they can set their filters and resampling rates
// accordingly.
void ChristmasMetronomeAudioProcessor::prepareVoices (double sampleRate, int samplesPerBlock)
{       
    for (int i = 0; i < numVoices; ++i)
    {
        SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (i));
        voice->prepareToPlay (samplesPerBlock, sampleRate);
        voice->setLpfFrequency (*lpfParams [i]);
        voice->setHpfFrequency (*hpfParams [i]);
    }
}

// Tell the voices to release their resources.
void ChristmasMetronomeAudioProcessor::releaseVoiceResources()
{
    for (int i = 0; i < numVoices; ++i)
    {
        SleighBellVoice *voice = static_cast <SleighBellVoice*> (sleighBellSynth.getVoice (i));
        voice->releaseResources();
    }
}

// Create a new instance of our plug-in.
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ChristmasMetronomeAudioProcessor();
}
