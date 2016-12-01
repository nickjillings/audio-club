/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent
{
public:
    //==============================================================================
    MainContentComponent()
        : phase (0.0f),
          amp (0.2f),
          freq (220.0f),
          phaseInc (0.0f)
    {
        setSize (100, 50);

        // specify the number of input and output channels that we want to open
        setAudioChannels (0, 1);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        phaseInc = freq * 2.0f * float_Pi / sampleRate;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        int startSample = bufferToFill.startSample;
        int numSamples = bufferToFill.numSamples;
        AudioSampleBuffer *buffer = bufferToFill.buffer;
        
        float *channelData = buffer->getWritePointer (0, startSample);
        
        for (int i = 0; i < numSamples; ++i)
        {
            channelData [i] = std::cos (phase);
            phase = std::fmod (phase + phaseInc, 2.0f * float_Pi);
        }
    }

    void releaseResources() override
    {
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::red);


        // You can add your drawing code here!
    }

    void resized() override
    {
        // This is called when the MainContentComponent is resized.
        // If you add any child components, this is where you should
        // update their positions.
    }


private:
    float phase;
    float amp;
    float freq;
    float phaseInc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
