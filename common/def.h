#ifndef DEF_H
#define DEF_H

typedef struct _GLNK_VideoDataFormat
{
    unsigned int codec;
    unsigned int bitrate;
    unsigned short width;
    unsigned short height;
    unsigned char framerate;
    unsigned char colorDepth;
    unsigned char frameInterval;
    unsigned char reserve;

} GLNK_VideoDataFormat;

typedef struct _GLNK_AudioDataFormat
{
    unsigned int samplesRate;
    unsigned int bitrate;
    unsigned short waveFormat;
    unsigned short channelNumber;
    unsigned short blockAlign;
    unsigned short bitsPerSample;
    unsigned short frameInterval;
    unsigned short reserve;
} GLNK_AudioDataFormat;

#endif // DEF_H
