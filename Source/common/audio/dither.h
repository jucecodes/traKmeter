/* ----------------------------------------------------------------------------

   FrutJUCE
   ========
   Common classes for use with the JUCE library

   Copyright (c) 2010-2016 Martin Zuther (http://www.mzuther.de/)

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

#ifndef __FRUT_AUDIO_DITHER_H__
#define __FRUT_AUDIO_DITHER_H__


//==============================================================================
/**
*/
class Dither
{
public:
    Dither(int number_of_bits, double noise_shaping = 0.5);

    void initialise(int number_of_bits, double noise_shaping = 0.5);
    float dither(double input);

private:
    JUCE_LEAK_DETECTOR(Dither);

    int nRandomNumber_1;
    int nRandomNumber_2;

    double dErrorFeedback_1;
    double dErrorFeedback_2;

    double dDcOffset;
    double dDitherAmplitude;
    double dNoiseShaping;
    double dWordLength;
    double dWordLengthInverted;
};


#endif  // __FRUT_AUDIO_DITHER_H__


// Local Variables:
// ispell-local-dictionary: "british"
// End:
