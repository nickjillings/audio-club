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
        : phase (0.0f), // Always initialise member variables in the initialisation list.
          amp (0.2f),
          freq (220.0f),
          phaseInc (0.0f)
    {
        // this sets the size of the window our program runs in
        setSize (100, 50);

        // our program will use 0 input channels and 1 output channel
        setAudioChannels (0, 1);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    // This function is called before the system starts calling our audio callback.
    // Here we can do any calculations / resource acquisition we need to do prior to 
    // running the audio callback. The system tells us the estimated buffer size which 
    // will be used and the sampling frequency which is being used.
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
        // Here we calculate the phase increment between samples
        // in a sine wave of the our frequency (freq) at the sampling 
        // frequency provided by the system.
        phaseInc = freq * 2.0f * float_Pi / sampleRate;
    }

    // This is our audio callback. This function gets called periodically by the system
    // providing us with frames of the audio input and requesting frames for the audio
    // output.
    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
        // bufferToFill is a reference to an AudioSourceChannelInfo object.
        // You can find details of the internals of this class in the JUCE documentaton.
        
        // Because it is a reference (denoted by the ampersand [&] after the type name
        // [AudioSourceChannelInfo]) it refers to a variable outside our function. Changes
        // we make to the audio buffer via bufferToFill change the variable outside the
        // function.
        
        // Here we copy the contents bufferToFill into local variables to make using it
        // easier.
        int startSample = bufferToFill.startSample; // The sample to start filling the buffer up from.
        int numSamples = bufferToFill.numSamples; // The number of samples to fill the buffer up with.
        AudioSampleBuffer *buffer = bufferToFill.buffer; // The audio buffer.
        
        // buffer now points to an AudioSampleBuffer object which we need to fill with our
        // output signal.
        
        // First we get a pointer to the first (0th) channel in the buffer starting at the startSample.
        float *channelData = buffer->getWritePointer (0, startSample);
        
        // Loop through the audio channel filling it up with samples of a sine wave.
        for (int i = 0; i < numSamples; ++i)
        {
            channelData [i] = std::cos (phase);

            // Here we increment the phase for the next sample. We wrap it to a value
            // between 0 and 2*pi using std::fmod() (the floating point modulus function).
            phase = std::fmod (phase + phaseInc, 2.0f * float_Pi);
        }

        // If our audio callback takes too long to execute we will get a buffer underflow.
        // This makes horrible clicks and pops in the output. Uncomment the next line to
        // experience this.
        
        // Thread::sleep (50);
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
    // Here are our member variable. These are persistent across calls to the audio
    // callback allowing us to keep track of out phase across audio buffers.
    float phase;
    float amp;
    float freq;
    float phaseInc;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED
