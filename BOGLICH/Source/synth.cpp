/*
  ==============================================================================

    synth.cpp
    Created: 27 Apr 2025 11:22:31am
    Author:  robert

  ==============================================================================
*/

#include "Synth.h"
#include "Utils.h"

Synth::Synth()
{
    sampleRate = 44100.0f;
}

void Synth::allocateResources(double sampleRate_, int samplesPerBlock)
{
    sampleRate = static_cast<float>(sampleRate_);
}

void Synth::deallocateResources()
{
}

void Synth::reset()
{
    voice.reset();
    noiseGen.reset();
}

void Synth::render(float **outputBuffers, int sampleCount)
{
    float *outputBufferLeft = outputBuffers[0];
    float *outputBufferRight = outputBuffers[1];

    for (int sample = 0; sample < sampleCount; ++sample)
    {
        float noise = noiseGen.nextValue();

        float output = 0.0f;

        if (voice.note > 0)
        {
            output = voice.render();
        }

        outputBufferLeft[sample] = output;
        if (outputBufferRight != nullptr)
        {
            outputBufferRight[sample] = output;
        }
    }

    protectYourEars(outputBufferLeft, sampleCount);
    protectYourEars(outputBufferRight, sampleCount);
}

void Synth::midiMessage(uint8_t data0, uint8_t data1, uint8_t data2)
{
    switch (data0 & 0xF0)
    {
    case 0x80:
        noteOff(data1 & 0x7f);
        break;

    case 0x90:
        uint8_t note = data1 & 0x7F;
        uint8_t velo = data2 & 0x7F;

        if (velo > 0)
        {
            noteOn(note, velo);
        }
        else
        {
            noteOff(note);
        }
        break;
    }
}
void Synth::noteOn(int note, int velocity)
{
    voice.note = note;

    voice.osc.amplitude = (velocity / 127.0f) * 0.5f;
    voice.osc.freq = 261.63f;
    voice.osc.sampleRate = sampleRate;
    voice.osc.phaseOffset = 0.0f;
    voice.osc.reset();
}

void Synth::noteOff(int note)
{
    if (voice.note == note)
    {
        voice.note = 0;
    }
}
    