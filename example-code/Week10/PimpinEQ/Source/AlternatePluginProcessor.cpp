/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

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
      fs (44100.0)
{
    // Define the ranges of all the parameters.
    const float minFreqs [numBands] = {20.0f, 200.0f, 500.0f};
    const float defaultFreqs [numBands] = {100.0f, 500.0f, 2000.0f};
    const float maxFreqs [numBands] = {500.0f, 2000.0f, 20000.0f};
    const float frequencySkew = 0.25f;
    
    const float minQ = 0.2f, defaultQ = 0.7f, maxQ = 5.0f;
    const float qSkew = 0.5f;
    
    const float minGain = -20.0f, defaultGain = 0.0f, maxGain = 20.0f;
    const float gainSkew = 1.0f;
    
    // Loop through bands to create the parameters
    for (int band = 0; band < numBands; ++band)
    {
        // Make some strings to use in parameter names and IDs
        String bandName = "Band " + String (band + 1);
        String bandID = "band" + String (band);
        
        // A lambda which will update the filter coefficients for this band.
        auto bandCallback = [this, band] () {updateFilterCoefficients (band);};
        
        // Add the frequency parameter
        String frequencyBandName = bandName + " Frequency";
        String frequencyBandID = bandID + "frequency";
        addParameter (frequencyParams [band] = 
                      new ParameterWithCallback (frequencyBandID,
                                                 frequencyBandName,
                                                 minFreqs [band],
                                                 maxFreqs [band],
                                                 defaultFreqs [band],
                                                 frequencySkew,
                                                 bandCallback));
        
        // Add the Q parameter
        String qBandName = bandName + " Q Factor";
        String qBandID = bandID + "q";
        addParameter (qParams [band] = 
                      new ParameterWithCallback (qBandID,
                                                 qBandName,
                                                 minQ,
                                                 maxQ,
                                                 defaultQ,
                                                 qSkew,
                                                 bandCallback));
                                                 
        // Add the gain parameter
        String gainBandName = bandName + " Gain";
        String gainBandID = bandID + "gain";
        addParameter (gainParams [band] = 
                      new ParameterWithCallback (gainBandID,
                                                 gainBandName,
                                                 minGain,
                                                 maxGain,
                                                 defaultGain,
                                                 gainSkew,
                                                 bandCallback));                                                  
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
    // save the sample rate
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

void PimpinEqAudioProcessor::updateFilterCoefficients (int band)
{
    // Generate some filter coefficients from the parameter values
    IIRCoefficients coeffs = 
        IIRCoefficients::makePeakFilter (fs,
                                         *frequencyParams [band],
                                         *qParams [band],
                                         Decibels::decibelsToGain <float> (*gainParams [band]));

    // give the coefficients to the relevant filters
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
