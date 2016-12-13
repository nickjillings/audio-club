#include "PluginProcessor.h"
#include "PluginEditor.h"

ChristmasMetronomeAudioProcessorEditor::ChristmasMetronomeAudioProcessorEditor (ChristmasMetronomeAudioProcessor& owner)
    // Initialise everything, anything added as a binary resource in the Projucer will
    // be made a static member of the BinaryData class.
    : AudioProcessorEditor (&owner), 
      processor (owner),
      background (Drawable::createFromImageData (BinaryData::BellsBackground_svg,
                                                 BinaryData::BellsBackground_svgSize)),
      width (630), height (333),
      gainLook ({BinaryData::AmplitudeDial_svg,
                 BinaryData::AmplitudeDial_svgSize,
                 80.0f, 80.0f}),
      lpfLook ({BinaryData::LPFDial_svg,
                BinaryData::LPFDial_svgSize,
                80.0f, 80.0f}),
      hpfLook ({BinaryData::HPFDial_svg,
                BinaryData::HPFDial_svgSize,
                80.0f, 80.0f})
{
    // Set the editor's size.
    setSize (width, height);
    
    // Find the midpoint.
    int midPoint = width / 2;

    // Define the size of our sliders.
    int sliderWidth = 110;
    int sliderHeight = 110;
    
    // Work out the y positions of the sliders.
    int titleBottom = 70;
    int topRowY = titleBottom + (height - titleBottom) / 4 - sliderWidth / 2; 
    int bottomRowY = titleBottom + 3 * (height - titleBottom) / 4 - sliderWidth / 2; ;
    
    // Work out the x positions of the sliders.
    int gainX = (midPoint - sliderWidth) / 2;
    int lpfX = midPoint / 4 - sliderWidth / 2;
    int hpfX = 3 * midPoint / 4 - sliderWidth / 2;

    // Loop through and add and position the sliders.
    for (int i = 0; i < ChristmasMetronomeAudioProcessor::numVoices; ++i)
    {
        int offset = midPoint * i;
        
        gainSliders [i] = new ParameterSlider (*owner.gainParams [i], &gainLook);
        addAndMakeVisible (gainSliders [i]);
        gainSliders [i]->setBounds (offset + gainX, topRowY, sliderWidth, sliderHeight);
        
        lpfSliders [i] = new ParameterSlider (*owner.lpfParams [i], &lpfLook);
        addAndMakeVisible (lpfSliders [i]);
        lpfSliders [i]->setBounds (offset + lpfX, bottomRowY, sliderWidth, sliderHeight);
        
        hpfSliders [i] = new ParameterSlider (*owner.hpfParams [i], &hpfLook);
        addAndMakeVisible (hpfSliders [i]);
        hpfSliders [i]->setBounds (offset + hpfX, bottomRowY, sliderWidth, sliderHeight);
    }
}

ChristmasMetronomeAudioProcessorEditor::~ChristmasMetronomeAudioProcessorEditor()
{ 
}

//==============================================================================
void ChristmasMetronomeAudioProcessorEditor::paint (Graphics &g)
{
    // Draw the background image.
    background->drawWithin (g, getBounds().toFloat(),
                            RectanglePlacement::stretchToFit, 1.0f);
}
