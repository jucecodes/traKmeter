/* ----------------------------------------------------------------------------

   MZ common JUCE
   ==============
   Common classes for use with the JUCE library

   Copyright (c) 2010-2015 Martin Zuther (http://www.mzuther.de/)

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   Thank you for using free software!

---------------------------------------------------------------------------- */

#include "generic_meter_segment.h"


GenericMeterSegment::GenericMeterSegment(float fLowerThreshold, float fDisplayRange, bool bHasHighestLevel)
//
//  The meter segment's state depends on two levels, the "normal" level
//  and the "discrete" level:
//
//  * normal level >= upper threshold
//
//    segment is fully lit
//
//
//  * lower threshold <= discrete level < upper threshold
//
//    segment is fully lit
//
//
//  * normal level < lower threshold
//
//    segment is dark
//
//
//  * otherwise
//
//    level affects segment's brightness
//
//
//  For normal meter segments, if any level peak lies between the
//  upper and lower threshold (or on the lower threshold), the
//  segment's peak marker is lit.
//
//  If there is no meter segment beyond this one, this behaviour is
//  changed.  The segment's peak marker is lit when any level peak
//  reaches or exceeds the lower threshold.
//
{
    // initialise thresholds
    setThresholds(fLowerThreshold, fDisplayRange, bHasHighestLevel);

    // initialise meter segment's brightness (0.0f is dark, 1.0f is
    // fully lit)
    fBrightness = 0.0f;
    fBrightnessOutline = 0.0f;

    // initialise meter segment's hue
    fHue = 0.0f;

    // make sure that segment is drawn after initialisation
    setLevels(-144.0f, -144.0f, -144.0f, -144.0f);
}


GenericMeterSegment::~GenericMeterSegment()
{
}


float GenericMeterSegment::setThresholds(float fLowerThreshold, float fDisplayRange, bool bHasHighestLevel)
{
    // lower level threshold
    lowerThreshold = fLowerThreshold;

    // level range above lower threshold
    thresholdRange = fDisplayRange;

    // upper level threshold
    upperThreshold = lowerThreshold + thresholdRange;

    // peak level marker is hidden
    lightPeakMarker = false;

    // is there a meter segment beyond this?
    hasHighestLevel = bHasHighestLevel;

    return upperThreshold;
}


void GenericMeterSegment::setColour(float fHueNew, const Colour &colPeakNew)
{
    fHue = fHueNew;
    colPeak = colPeakNew;

    // redraw meter segment
    repaint();
}


void GenericMeterSegment::paint(Graphics &g)
{
    // get meter segment's screen dimensions
    int width = getWidth();
    int height = getHeight();

    // initialise meter segment's outline colour from hue and
    // brightness
    g.setColour(Colour(fHue, 1.0f, fBrightnessOutline, 1.0f));

    // outline meter segment with solid colour, but leave a border of
    // one pixel for peak marker
    g.drawRect(1, 1, width - 2, height - 2);

    // initialise meter segment's fill colour from hue and brightness
    g.setColour(Colour(fHue, 1.0f, fBrightness, 1.0f));

    // fill remaining meter segment with solid colour
    g.fillRect(2, 2, width - 4, height - 4);

    // if peak marker is lit, draw a rectangle around meter segment
    // (width: 1 pixel)
    if (lightPeakMarker)
    {
        g.setColour(colPeak);
        g.drawRect(0, 0, width, height);
    }
}


void GenericMeterSegment::visibilityChanged()
{
    // if this function did not exist, the meter segment wouldn't be
    // drawn until the first level change!
}


void GenericMeterSegment::resized()
{
}


// use this only if you completely disregard discrete levels!
void GenericMeterSegment::setNormalLevels(float normalLevel, float normalLevelPeak)
{
    setLevels(normalLevel, -144.0f, normalLevelPeak, -144.0f);
}


// use this only if you completely disregard normal levels!
void GenericMeterSegment::setDiscreteLevels(float discreteLevel, float discreteLevelPeak)
{
    setLevels(-144.0f, discreteLevel, -144.0f, discreteLevelPeak);
}


void GenericMeterSegment::setLevels(float normalLevel, float discreteLevel, float normalLevelPeak, float discreteLevelPeak)
{
    // store old brightness and peak marker values
    float fBrightnessOld = fBrightness;
    bool lightPeakMarkerOld = lightPeakMarker;

    // normal level lies on or above upper threshold, so fully light
    // meter segment
    if (normalLevel >= upperThreshold)
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // discrete level lies within thresholds or on lower threshold, so
    // fully light meter segment
    else if ((discreteLevel >= lowerThreshold) && (discreteLevel < upperThreshold))
    {
        fBrightness = 0.97f;
        fBrightnessOutline = 0.90f;
    }
    // normal level lies below lower threshold, so set meter segment
    // to dark
    else if (normalLevel < lowerThreshold)
    {
        fBrightness = 0.25f;
        fBrightnessOutline = 0.30f;
    }
    // normal level lies within thresholds or on lower threshold, so
    // calculate brightness from current level
    else
    {
        fBrightness = (normalLevel - lowerThreshold) / thresholdRange;
        fBrightnessOutline = fBrightness;

        // to look well, meter segments should be left with some
        // colour and not have maximum brightness
        fBrightness = fBrightness * 0.72f + 0.25f;
        fBrightnessOutline = fBrightnessOutline * 0.60f + 0.30f;
    }

    // there is no meter segment beyond this
    if (hasHighestLevel)
    {
        // normal peak level lies on or above lower threshold, so
        // light peak marker
        if (normalLevelPeak >= lowerThreshold)
        {
            lightPeakMarker = true;
        }
        // discrete peak level lies on or above lower threshold, so
        // light peak marker
        else if (discreteLevelPeak >= lowerThreshold)
        {
            lightPeakMarker = true;
        }
        // otherwise, hide peak marker
        else
        {
            lightPeakMarker = false;
        }
    }
    else
    {
        // normal peak level lies within thresholds or on lower threshold,
        // so light peak marker
        if ((normalLevelPeak >= lowerThreshold) && (normalLevelPeak < upperThreshold))
        {
            lightPeakMarker = true;
        }
        // discrete peak level lies within thresholds or on lower
        // threshold, so light peak marker
        else if ((discreteLevelPeak >= lowerThreshold) && (discreteLevelPeak < upperThreshold))
        {
            lightPeakMarker = true;
        }
        // otherwise, hide peak marker
        else
        {
            lightPeakMarker = false;
        }
    }

    // re-paint meter segment only when brightness or peak marker have
    // changed
    if ((fBrightness != fBrightnessOld) || (lightPeakMarker != lightPeakMarkerOld))
    {
        repaint(getLocalBounds());
    }
}


// Local Variables:
// ispell-local-dictionary: "british"
// End:
