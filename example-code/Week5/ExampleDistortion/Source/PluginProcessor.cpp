/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
ExampleDistortionAudioProcessor::ExampleDistortionAudioProcessor()
    : fs (0),
      driveParam (nullptr),
      gainParam (nullptr),
      toneParam (nullptr)
{
    addParameter (driveParam = new GainParameter ("drive", "Drive", 
                                                  -20.0f, 20.0f, 0.0f));
    addParameter (gainParam = new GainParameter ("gain", "Make Up Gain", 
                                                 -20.0f, 20.0f, 0.0f));
    auto callback = [this] (float frequency) 
                    {setFilterFrequency (frequency);};
    addParameter (toneParam = new ParameterWithCallback ("tone", "Tone",
                                                         200.0f, 20000.0f,
                                                         20000.0f, 0.3f,
                                                         callback));
}

ExampleDistortionAudioProcessor::~ExampleDistortionAudioProcessor()
{
}

//==============================================================================
const String ExampleDistortionAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool ExampleDistortionAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool ExampleDistortionAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double ExampleDistortionAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int ExampleDistortionAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int ExampleDistortionAudioProcessor::getCurrentProgram()
{
    return 0;
}

void ExampleDistortionAudioProcessor::setCurrentProgram (int index)
{
}

const String ExampleDistortionAudioProcessor::getProgramName (int index)
{
    return String();
}

void ExampleDistortionAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void ExampleDistortionAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    fs = sampleRate;
    setFilterFrequency (*toneParam);
    lpf.reset();
}

void ExampleDistortionAudioProcessor::releaseResources()
{
}

void ExampleDistortionAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    buffer.applyGain (driveParam->getGainValue());
    
    float *channelData = buffer.getWritePointer (0);
    int numSamples = buffer.getNumSamples();
    float threshold = 0.5;
    
    for (int i = 0; i < numSamples; ++i)
    {
        if (channelData [i] > threshold)
            channelData [i] = threshold;
        else if (channelData [i] < -threshold)
            channelData [i] = -threshold;
    }
    
    buffer.applyGain (gainParam->getGainValue());
    
    lpf.processSamples (channelData, numSamples);
}

//==============================================================================
bool ExampleDistortionAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* ExampleDistortionAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void ExampleDistortionAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void ExampleDistortionAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void ExampleDistortionAudioProcessor::setFilterFrequency (float frequency)
{
    lpf.setCoefficients (IIRCoefficients::makeLowPass (fs, frequency));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new ExampleDistortionAudioProcessor();
}
