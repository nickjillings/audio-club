#ifndef LOOKANDFEEL_H_INCLUDED
#define LOOKANDFEEL_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

// A class which describes how the JUCE GUI Components are drawn.
class ChristmasLookAndFeel : public LookAndFeel_V2
{
public:
    // A little structure which will contain pointers to 
    // the image data for our sliders.
    struct ImageData
    {
        const void *rotarySliderImageData; // The image data.
        int rotarySliderImageSize; // The number of bytes in the image data.

        // The dimensions of the actual image.
        float rotarySliderImageWidth, rotarySliderImageHeight; 
    };
    
    // The constructor takes a reference to the image data structure.
    ChristmasLookAndFeel (const ImageData& imageData);
    ~ChristmasLookAndFeel();
    
    // Our sliders will call this function to draw themselves.
    void drawRotarySlider (Graphics &g,
                           int x, int y, int width, int height,
                           float sliderPosProportional,
                           float rotaryStartAngle,
                           float rotaryEndAngle,
                           Slider &slider) override;
                           
private:
    // Variables in which to store the image data.
    float rotarySliderWidth, rotarySliderHeight;
    ScopedPointer <Drawable> rotarySliderImage;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ChristmasLookAndFeel)
};

#endif  // LOOKANDFEEL_H_INCLUDED
