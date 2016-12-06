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
    : fs (0), // zero initialise all the members
      driveParam (nullptr),
      gainParam (nullptr),
      toneParam (nullptr)
{
    // create the parameters and add them to the plug-in
    addParameter (driveParam = new GainParameter ("drive", "Drive", 
                                                  -20.0f, 20.0f, 0.0f));
    addParameter (gainParam = new GainParameter ("gain", "Make Up Gain", 
                                                 -20.0f, 20.0f, 0.0f));

    // here we create a lambda (anonymous function) which calls the 
    // plug-in's setFilterFrequencyFunction()
    // This is passed to out toneParam parameter as the callback it 
    // should call when its value is changed.
    // This way when the tone parameter is changed it will
    // ask the plug-in to recalculate its filter coefficients
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
    // save the sample rate so we can use it when we need to calculate filter coefficients
    fs = sampleRate;

    // if the sample rate has changed we need to recalculate the filter coefficients
    setFilterFrequency (*toneParam);

    // reset the filter's internal buffers as the playhead may have been moved from 
    // the position it was in when audio was last stopped
    lpf.reset();
}

void ExampleDistortionAudioProcessor::releaseResources()
{
}

void ExampleDistortionAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    // apply the drive gain
    buffer.applyGain (driveParam->getGainValue());
    
    // get an array of the channel data
    float *channelData = buffer.getWritePointer (0);
    int numSamples = buffer.getNumSamples();
    float threshold = 0.5;
    
    // loop through the data and apply clipping
    for (int i = 0; i < numSamples; ++i)
    {
        if (channelData [i] > threshold)
            channelData [i] = threshold;
        else if (channelData [i] < -threshold)
            channelData [i] = -threshold;
    }
    
    // apply the make up gain
    buffer.applyGain (gainParam->getGainValue());
    
    // apply the tone filter
    lpf.processSamples (channelData, numSamples);
}

//==============================================================================
bool ExampleDistortionAudioProcessor::hasEditor() const
{
    return false; // we have no editor
}

AudioProcessorEditor* ExampleDistortionAudioProcessor::createEditor()
{
    // even though we have made hasEditor() return false some
    // hosts will still call this, so we just hand them back nothing
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
