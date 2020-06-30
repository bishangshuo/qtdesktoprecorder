//
//  HAACEncode.h
//  AVDemo
//
//  Created by LiuJun on 16/4/12.
//  Copyright © 2016年 frank. All rights reserved.
//
#if defined(__APPLE__)
#ifndef HAACEncode_h
#define HAACEncode_h

#include <iostream>
#include "HAACBuffer.h"
#include <MediaControl/EncoderF/IEncoderF.h>

#import <AudioToolbox/AudioToolbox.h>

class HAACEncode : public IEncoderF{
public:
    HAACEncode();
    virtual ~HAACEncode();

    virtual int	    Initialize(unsigned char* pData,long lDataLen);
    virtual int	    Initialize(const IEncoderF::T_AVCEncoderParam& stEncoderParam);
    
    int             Encode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType);
    
    virtual int		VideoEncode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace = TRUE,int64_t lTimestamp = 0);
    virtual int		VideoCopyEncode(void* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp = 0);
   
    virtual int	    ReleaseEncoder();
    
    virtual bool	IsInitialize();
    
    virtual void	Release();
    
    virtual int	    GetInformationData(unsigned char* pBuffer,EncoderFDefine::E_EncoderF_Information_Type eType);
    
    virtual int     SetYUVAngle(int nAngle) ;
    
    virtual void    SetDataCallback(EncoderFDefine::OutputData_Callback pOutputDataFunction, long uUserData);
    
    int             SendAdtsAACPacket(unsigned char* data, int size);
    
    virtual void SetBitrate(int bitrate);
private:
    static OSStatus ioProc(AudioConverterRef audioConverter, UInt32 *ioNumDataPackets, AudioBufferList* ioData, AudioStreamPacketDescription** ioPacketDesc, void* inUserData );
    
    void makeAsc(uint8_t sampleRateIndex, uint8_t channelCount);
    
protected:
private:
    int                          bit_rate_;
    AudioStreamBasicDescription  in_, out_;
    AudioStreamBasicDescription  unit_description_;
    AudioBuffer                  *_converter_currentBuffer_;
    AudioStreamBasicDescription  _converter_currentInputDescription_;
    std::mutex                   mutex_;
    AudioConverterRef            audio_converter_;
    size_t                       bytes_persample_;
    uint32_t                     output_packet_maxsize_;
    int                          output_packet_size_;
    HAACBuffer                   output_buffer_;
    uint8_t                      asc_[2];
    char	                     m_pAudioBuffer[3000];
    bool                         sent_config_;
    int                          frequency_inHz_;
    int                          channel_count_;
    int                          average_bitrate_;
    OSStatus                     result_;
    FILE                         *out_file_;
};

#endif /* HAACEncode_h */
#endif
