#include "Synthesiser.h"

//======================================================
// The synthesiser sounds.
//======================================================
DownBeatSound::DownBeatSound()
{
}

DownBeatSound::~DownBeatSound()
{
}

bool DownBeatSound::appliesToNote (int midiNoteNumber)
{        
    // The downbeat sound applies to downbeat MIDI notes
    if (midiNoteNumber == SleighBellVoice::DownBeatNote)
        return true;
    else
        return false;
}

bool DownBeatSound::appliesToChannel (int /*midiChannel*/)
{
    // We don't care what channel the MIDI comes in on.
    return true;
}

OtherBeatSound::OtherBeatSound()
{
}

OtherBeatSound::~OtherBeatSound()
{
}

bool OtherBeatSound::appliesToNote (int midiNoteNumber)
{    
    // The other beat sound applies to other beat MIDI notes
    if (midiNoteNumber == SleighBellVoice::OtherBeatNote)
        return true;
    else
        return false;
}

bool OtherBeatSound::appliesToChannel (int /*midiChannel*/)
{
    // We don't care what channel the MIDI comes in on.
    return true;
}

//======================================================
// The synthesiser voice.
//======================================================
SleighBellVoice::SleighBellVoice (int midiNoteInit)
    : midiNote (midiNoteInit),
      gain (0.0f), gainValue (0.0f),
      fs (44100.0),
      lpfFrequency (20000.0f), hpfFrequency (1.0f)
{
    // Create a new object to read audio from the sleigh bell sample file.
    readerSource = new AudioFormatReaderSource (sleighBellReader->reader, false);
    // Set it so it doesn't loop.
    readerSource->setLooping (false);
    
    // Wrap that object in another object which will resample the audio for us.
    audioSource = new ResamplingAudioSource (readerSource, false, numChannels);
}

SleighBellVoice::~SleighBellVoice()
{
}
    
bool SleighBellVoice::canPlaySound (SynthesiserSound *sound)
{
    // Only play notes which this voice is set up to.
    return sound->appliesToNote (midiNote);
}

void SleighBellVoice::startNote (int /*midiNoteNumber*/, float /*velocity*/, 
                                 SynthesiserSound* /*sound*/, int /*currentPitchWheelPosition*/)
{
    // Turn up the gain/
    gain = gainValue;

    // Reset the filters' internal buffers.
    resetFilters();

    // Rewind the audio source to the start.
    readerSource->setNextReadPosition (0);
}

void SleighBellVoice::stopNote (float /*velocity*/, bool /*allowTailOff*/)
{
    // Mute the audio.
    gain = 0.0f;

    // Inform the synthesiser that this voice is free to use.
    clearCurrentNote();
}

void SleighBellVoice::pitchWheelMoved (int /*newPitchWheelValue*/)
{
}

void SleighBellVoice::controllerMoved (int /*controllerNumber*/, int /*newControllerValue*/)
{
}

void SleighBellVoice::renderNextBlock (AudioSampleBuffer &outputBuffer, int startSample, int numSamples)
{
    // Copy the next block of samples from the sleigh bell sample into our local buffer.
    audioSource->getNextAudioBlock (AudioSourceChannelInfo (&currentBuffer, 0, numSamples));
    
    const int numOutputChannels = outputBuffer.getNumChannels();
    
    for (int i = 0; i < jmin (numChannels, numOutputChannels); ++i)
    {
        float *channelData = currentBuffer.getWritePointer (i, startSample);
        
        // Apply the filters to the audio.
        lpf [i].processSamples (channelData, numSamples);
        hpf [i].processSamples (channelData, numSamples);
        
        // Apply the gain and add our sound to the output buffer.
        outputBuffer.addFrom (i, startSample, currentBuffer, i, 0, numSamples, gain);
    }
}

void SleighBellVoice::prepareToPlay (int samplesPerBlock, double sampleRate)
{
    // Save the sample rate.
    fs = sampleRate;
    
    // Update filter coefficients to use the new sample rate.
    updateLpf();
    updateHpf();
    resetFilters();
    
    // Prepare the audio sources and set the resampling ratio.
    double sourceSampleRate = sleighBellReader->reader->sampleRate;
    readerSource->prepareToPlay (samplesPerBlock, sourceSampleRate);
    audioSource->setResamplingRatio (sourceSampleRate / sampleRate);
    audioSource->prepareToPlay (samplesPerBlock, sampleRate);

    // Ensure there is enough room in our local buffer to store a buffer of audio
    currentBuffer.setSize (numChannels, samplesPerBlock * 4);
    
    // Tell the synthesiser this voice is ready to be used.
    clearCurrentNote();
}

void SleighBellVoice::releaseResources()
{
    // Release whatever resources the audio sources allocated.
    audioSource->releaseResources();
    readerSource->releaseResources();
}

// Set the gain.
void SleighBellVoice::setGain (float newGain)
{
    gainValue = newGain;
}

// Set lpf frequency.
void SleighBellVoice::setLpfFrequency (float newFrequency)
{
    lpfFrequency = newFrequency;
    updateLpf();
}

// Set hpf frequency.
void SleighBellVoice::setHpfFrequency (float newFrequency)
{
    hpfFrequency = newFrequency;
    updateHpf();
}

// Recalculate the lpf coefficients.
void SleighBellVoice::updateLpf()
{
    for (int i = 0; i < numChannels; ++i)
        lpf [i].setCoefficients (IIRCoefficients::makeLowPass (fs, lpfFrequency));
}

// Recalculate the hpf coefficients.
void SleighBellVoice::updateHpf()
{
    for (int i = 0; i < numChannels; ++i)
        hpf [i].setCoefficients (IIRCoefficients::makeHighPass (fs, hpfFrequency));
}

// Reset the filters' internal buffers.
void SleighBellVoice::resetFilters()
{
    for (int i = 0; i < numChannels; ++i)
    {
        lpf [i].reset();
        hpf [i].reset();
    }
}

//======================================================
// The audio loader.
//======================================================
SleighBellVoice::AudioLoader::AudioLoader()
{
    // Enable us to read .wav files.
    formatManager.registerFormat (new WavAudioFormat, true);

    // Create an object which reads data from the sleighbell.wav file.
    reader = formatManager.createReaderFor (new MemoryInputStream (BinaryData::sleighbell_wav,
                                                                   BinaryData::sleighbell_wavSize,
                                                                   false));
}

SleighBellVoice::AudioLoader::~AudioLoader()
{
}
