#pragma once

class NoiseGenerator{
    public:
    void reset(){
        noiseSeed = 22222;
    }

    float nextValue(){
        noiseSeed = noiseSeed * 196314165 + 908633515;

        int temp = int(noiseSeed >> 7) - 16777216;

        return float(temp) / 16777216.0f;

    }

    private:
        unsigned int noiseSeed;

}