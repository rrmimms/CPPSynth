/*
  ==============================================================================

    Oscillator.h
    Created: 27 Apr 2025 11:23:26am
    Author:  robert

  ==============================================================================
*/

#pragma once
#include <cmath>

const float TWO_PI = 6.2831853071795864f;

class Oscillator
{
    public:
        float amplitude;
        float freq;
        float sampleRate;
        float phaseOffset;
        int sampleIndex;

        void reset(){
            sampleIndex = 0;
        }
    float nextSample()
    {
        float output = amplitude * sin( TWO_PI *  sampleIndex * freq / sampleRate + phaseOffset);
        
        sampleIndex++;
        return output;
    }

};