/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"


//==============================================================================
DankDelayAudioProcessor::DankDelayAudioProcessor()
    : 
#ifndef JucePlugin_PreferredChannelConfigurations
      AudioProcessor (BusesProperties()
                    #if ! JucePlugin_IsMidiEffect
                     #if ! JucePlugin_IsSynth
                      .withInput  ("Input",  AudioChannelSet::stereo(), true)
                     #endif
                      .withOutput ("Output", AudioChannelSet::stereo(), true)
                    #endif
                      ),
#endif
      delayGain (nullptr),
      writeTap (0), readTap (0)
{                                                           
    addParameter (delayGain = new AudioParameterFloat ("delayGain", "GelayGain", -40.0f, 0.0f, -6.0f));   
}

DankDelayAudioProcessor::~DankDelayAudioProcessor()
{
}

//==============================================================================
const String DankDelayAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool DankDelayAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool DankDelayAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double DankDelayAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int DankDelayAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int DankDelayAudioProcessor::getCurrentProgram()
{
    return 0;
}

void DankDelayAudioProcessor::setCurrentProgram (int index)
{
}

const String DankDelayAudioProcessor::getProgramName (int index)
{
    return String();
}

void DankDelayAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void DankDelayAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    delayBuffer.setSize (getTotalNumInputChannels(), delayTimeSamples + 1);
    delayBuffer.clear();
    writeTap = 0;
    readTap = 1;
}

void DankDelayAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool DankDelayAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void DankDelayAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
        
    int writeTapStart = writeTap;
    int readTapStart = readTap;

    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        float* channelData = buffer.getWritePointer (channel);
        float* delayData = delayBuffer.getWritePointer (channel);
        
        writeTap = writeTapStart;
        readTap = readTapStart;
        
        for (int i = 0; i < buffer.getNumSamples(); ++i)
        { 
            channelData [i] += delayData [readTap];
            delayData [writeTap] = Decibels::decibelsToGain <float> (*delayGain)
                                   * (delayData [writeTap] + channelData [i]);  
            
            ++writeTap;
            writeTap %= delayTimeSamples + 1;
            
            ++readTap;
            readTap %= delayTimeSamples + 1;
        }
    }
}

//==============================================================================
bool DankDelayAudioProcessor::hasEditor() const
{
    return false; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* DankDelayAudioProcessor::createEditor()
{
    return nullptr; //new DankDelayAudioProcessorEditor (*this);
}

//==============================================================================
void DankDelayAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void DankDelayAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new DankDelayAudioProcessor();
}
