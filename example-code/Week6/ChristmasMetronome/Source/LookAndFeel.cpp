#include "LookAndFeel.h"

ChristmasLookAndFeel::ChristmasLookAndFeel (const ImageData& imageData)
    // We initialise all the image data in the initialisation list.
    : rotarySliderWidth (imageData.rotarySliderImageWidth),  
      rotarySliderHeight (imageData.rotarySliderImageHeight),
      rotarySliderImage (Drawable::createFromImageData (imageData.rotarySliderImageData,
                                                        imageData.rotarySliderImageSize))
{
}

ChristmasLookAndFeel::~ChristmasLookAndFeel()
{
}
    
void ChristmasLookAndFeel::drawRotarySlider (Graphics &g,
                                             int x, int y, int width, int height,
                                             float sliderPosProportional,
                                             float rotaryStartAngle,
                                             float rotaryEndAngle,
                                             Slider& /*slider*/)
{
    // Work out the angle of rotation to apply to the slider image.
    float angleRange = rotaryEndAngle - rotaryStartAngle;
    float angle = sliderPosProportional * angleRange + rotaryStartAngle;
    
    // Find the center of the region we are drawing into.
    float centreX = x + width / 2.0f;
    float centreY = y + height / 2.0f;
    
    // Define a transform which will scale our slider image to
    // correct size, move it to the correct position and rotate it to the correct angle.
    AffineTransform transform = AffineTransform::scale (width / rotarySliderWidth, 
                                                        height / rotarySliderHeight);
    transform = transform.translated (static_cast <float> (x), 
                                      static_cast <float> (y));
    transform = transform.rotated (angle, centreX, centreY);
        
    // Draw the slider image with the transform applied.
    rotarySliderImage->draw (g, 1.0f, transform);
}
