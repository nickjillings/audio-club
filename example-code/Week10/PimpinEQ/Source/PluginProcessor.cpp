/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include <sstream>

//==============================================================================
PimpinEqAudioProcessor::PimpinEqAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       ),
#endif
      fs (44100.0),
      pluginState (*this, nullptr)
{
    // Define the ranges of all the parameters.
    const float minFreqs [numBands] = {20.0f, 200.0f, 500.0f};
    const float defaultFreqs [numBands] = {100.0f, 500.0f, 2000.0f};
    const float maxFreqs [numBands] = {500.0f, 2000.0f, 20000.0f};
    const float frequencySkew = 0.25f;
    
    const float minQ = 0.2f, defaultQ = 0.7f, maxQ = 5.0f;
    const float qSkew = 0.5f;
    NormalisableRange <float> qRange (minQ, maxQ, 0.0f, qSkew);   
    
    const float minGain = -20.0f, defaultGain = 0.0f, maxGain = 20.0f;
    const float gainSkew = 1.0f;
    NormalisableRange <float> gainRange (minGain, maxGain, 0.0f, gainSkew);
    
    // Loop through bands to create the parameters
    for (int band = 0; band < numBands; ++band)
    {
        // Make some strings to use in parameter names and IDs
        String bandName = "Band " + String (band + 1);
        String bandID = "band" + String (band);
        
        // Add the frequency parameter
        NormalisableRange <float> frequencyBandRange (minFreqs [band],
                                                      maxFreqs [band],
                                                      0.0f,
                                                      frequencySkew);
        String frequencyBandName = bandName + " Frequency";
        String frequencyBandID = bandID + "frequency";
        frequencyParamIDs [band] = frequencyBandID; // store the ID
        pluginState.createAndAddParameter (frequencyBandID,
                                           frequencyBandName,
                                           "",
                                           frequencyBandRange,
                                           defaultFreqs [band],
                                           nullptr,
                                           nullptr);
        // listen for updates to the parameter
        pluginState.addParameterListener (frequencyBandID, this);
        
        // Add the Q parameter
        String qBandName = bandName + " Q Factor";
        String qBandID = bandID + "q";
        qParamIDs [band] = qBandID; // store the ID
        pluginState.createAndAddParameter (qBandID,
                                           qBandName,
                                           "",
                                           qRange,
                                           defaultQ,
                                           nullptr,
                                           nullptr);
        // listen for updates to the parameter
        pluginState.addParameterListener (qBandID, this);
                                                 
        // Add the gain parameter
        String gainBandName = bandName + " Gain";
        String gainBandID = bandID + "gain";
        gainParamIDs [band] = gainBandID; // store the ID
        pluginState.createAndAddParameter (gainBandID,
                                           gainBandName,
                                           "",
                                           gainRange,
                                           defaultGain,
                                           nullptr,
                                           nullptr);
        // listen for updates to the parameter
        pluginState.addParameterListener (gainBandID, this);
    }   
}

PimpinEqAudioProcessor::~PimpinEqAudioProcessor()
{
}

//==============================================================================
const String PimpinEqAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool PimpinEqAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool PimpinEqAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

double PimpinEqAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int PimpinEqAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int PimpinEqAudioProcessor::getCurrentProgram()
{
    return 0;
}

void PimpinEqAudioProcessor::setCurrentProgram (int index)
{
}

const String PimpinEqAudioProcessor::getProgramName (int index)
{
    return String();
}

void PimpinEqAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void PimpinEqAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // store the sample rate
    fs = sampleRate;
    
    int numChannels = getTotalNumInputChannels();
    
    // loop through bands to add filters
    for (int band = 0; band < numBands; ++band)
    {
        // clear any filters we aren't using anymore
        filters [band].clear();
        
        // add a filter for each channel of audio
        for (int channel = 0; channel < numChannels; ++channel)
        {
            filters [band].add (new IIRFilter);
        }
        
        // initialise this band's filter coefficients
        updateFilterCoefficients (band);
    }
}

void PimpinEqAudioProcessor::releaseResources()
{
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool PimpinEqAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void PimpinEqAudioProcessor::processBlock (AudioSampleBuffer& buffer, MidiBuffer& midiMessages)
{
    const int totalNumInputChannels  = getTotalNumInputChannels();
    const int totalNumOutputChannels = getTotalNumOutputChannels();
    const int numSamples = buffer.getNumSamples();
    
    // Loop through and apply the filters
    for (int channel = 0; channel < totalNumInputChannels; ++ channel)
    {
        float *channelData = buffer.getWritePointer (channel);
        
        // Magical range based for loops
        for (auto &band : filters)
        {
            band [channel]->processSamples (channelData, numSamples);
        }
    }

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (int i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
}

//==============================================================================
bool PimpinEqAudioProcessor::hasEditor() const
{
    return false;
}

AudioProcessorEditor* PimpinEqAudioProcessor::createEditor()
{
    return nullptr;
}

//==============================================================================
void PimpinEqAudioProcessor::getStateInformation (MemoryBlock& destData)
{
}

void PimpinEqAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
}

void PimpinEqAudioProcessor::parameterChanged (const String &parameterID, float /*newValue*/)
{
    int band = 0;
    std::stringstream ss;
    
    // Some stringstream mischief to get the band number out of the paramerID
    ss << parameterID.substring (4);
    ss >> band;
    
    // update the coefficients for the band
    updateFilterCoefficients (band);
}

void PimpinEqAudioProcessor::updateFilterCoefficients (int band)
{
    // Get the parameter values
    const float *freq = pluginState.getRawParameterValue (frequencyParamIDs [band]);
    const float *q = pluginState.getRawParameterValue (qParamIDs [band]);
    const float *gain = pluginState.getRawParameterValue (gainParamIDs [band]);

    // calculate the coefficients
    IIRCoefficients coeffs = 
        IIRCoefficients::makePeakFilter (fs, *freq, *q,
                                         Decibels::decibelsToGain (*gain));

    // give them to the filters
    for (auto filter : filters [band])
    {
        filter->setCoefficients (coeffs);
    }
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new PimpinEqAudioProcessor();
}
