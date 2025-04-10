#include <stdio.h>
#include <stdlib.h>
#include <cstdint>
#include <string.h>

using namespace std;

struct SMinimalWaveFileHeader{
    unsigned char m_szChunkID[4];
    uint32_t m_nChunkSize;
    unsigned char m_szFormat[4];

    unsigned char m_szSubChunk1ID[4];
    uint32_t m_nSubChunk1Size;
    uint16_t m_nAudioFormat;
    uint16_t m_nNumChannels;
    uint32_t m_nSampleRate;
    uint32_t m_nByteRate;
    uint16_t m_nBlockAlign;
    uint16_t m_nBitsPerSample;

    unsigned char m_szSubChunk2ID[4];
    uint32_t m_nSubChunk2Size;

};

bool WriteWaveFile(const char *szFileName, void *pData, int32_t nDataSize, int16_t nNumChannels, int32_t nSampleRate, int32_t nBitsPerSample)
{
    FILE *File = fopen(szFileName, "w+b");
    if(!File)
    {
        return false;
    }

    SMinimalWaveFileHeader waveHeader;

    memcpy(waveHeader.m_szChunkID, "RIFF", 4);
    waveHeader.m_nChunkSize = nDataSize + 36;
    memcpy(waveHeader.m_szFormat, "WAVE", 4);

    memcpy(waveHeader.m_szSubChunk1ID, "fmt ", 4);
    waveHeader.m_nSubChunk1Size = 16;
    waveHeader.m_nAudioFormat = 1;
    waveHeader.m_nNumChannels = nNumChannels;
    waveHeader.m_nSampleRate = nSampleRate;
    waveHeader.m_nByteRate = nSampleRate * nNumChannels * nBitsPerSample / 8;
    waveHeader.m_nBlockAlign = nNumChannels * nBitsPerSample / 8;
    waveHeader.m_nBitsPerSample = nBitsPerSample;

    memcpy(waveHeader.m_szSubChunk2ID, "data", 4);
    waveHeader.m_nSubChunk2Size = nDataSize;

    fwrite(&waveHeader, sizeof(SMinimalWaveFileHeader), 1, File);

    fwrite(pData, nDataSize, 1, File);

    fclose(File);
    return true;

}

int main(int argc, char* argv[]){

    int nSampleRate = 44100;
    int nNumSeconds = 4;
    int nNumChannels = 1;

    int nNumSamples = nSampleRate * nNumChannels * nNumSeconds;
    int32_t *pData = new int32_t[nNumSamples];

    int nValue = 0;
    for(int nIndex = 0; nIndex < nNumSamples; ++nIndex){
        nValue += 8000000;
        pData[nIndex] = nValue;
    }

    WriteWaveFile("outmono.wav", pData, nNumSamples *sizeof(pData[0]), nNumChannels, nSampleRate, sizeof(pData[0] * 8));
    delete[] pData;
}