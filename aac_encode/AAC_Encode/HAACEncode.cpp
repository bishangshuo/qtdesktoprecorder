//
//  HAACEncode.cpp
//  AVDemo
//
//  Created by LiuJun on 16/4/12.
//  Copyright © 2016年 frank. All rights reserved.
//
#if defined(__APPLE__)
#include "StdAfx.h"
#include "HAACEncode.h"
#include <MediaControl/EncoderF/EncoderF_Define.h>
#include <MediaControl/FileReadWriteF/FileReadWriteF_Define.h>
#include <MediaControl/DecoderF/DecoderF_Define.h>
#include <BaseClass/BaseFunction.h>
#include <stdio.h>
#include <sstream>
#include <string.h>
#include <stdlib.h>

//extern char g_test_path[MAX_PATH];
extern const char * pathss;

bool IsAACHardwareEncoderAvailable(void)
{
    bool isAvailable = false;
    OSStatus error;
    
    UInt32 encoderSpecifier = kAudioFormatMPEG4AAC;
    UInt32 size;
    
    error = AudioFormatGetPropertyInfo(kAudioFormatProperty_Encoders, sizeof(encoderSpecifier),
                                       &encoderSpecifier, &size);
    if (error) {
        XTRACE("kAudioFormatProperty_Encoders error %d %4.4s\n", (int)error, (char*)&error);
        return false;
    }
    
    UInt32 numEncoders = size / sizeof(AudioClassDescription);
    AudioClassDescription encoderDescriptions[numEncoders];
    
    error = AudioFormatGetProperty(kAudioFormatProperty_Encoders, sizeof(encoderSpecifier),
                                   &encoderSpecifier, &size, encoderDescriptions);
    if (error) {
        XTRACE("kAudioFormatProperty_Encoders error %d %4.4s\n", (int)error, (char*)&error);
        return false;
    }
    
    for (UInt32 i=0; i < numEncoders; ++i) {
        if (encoderDescriptions[i].mSubType == kAudioFormatMPEG4AAC &&
            encoderDescriptions[i].mManufacturer == kAppleSoftwareAudioCodecManufacturer)
            isAvailable = true;
    }
    
    return isAvailable;
}

struct UserData{
    uint8_t* data_;
    int size_;
    int packet_size_;
    AudioStreamPacketDescription *pd_;
};

static const int kSamplesPerFrame = 1024;

HAACEncode::HAACEncode()
:sent_config_(false),channel_count_(1),result_(3){

}

HAACEncode::~HAACEncode(){
    
    AudioConverterDispose(audio_converter_);
    //fclose(outfile);
}

int	HAACEncode::Initialize(const IEncoderF::T_AVCEncoderParam& stEncoderParam)
{
    return 0;
}

int	HAACEncode::Initialize(unsigned char* pData,long lDataLen)
{
    //outfile = fopen(pathss, "wb+");
    //if (!outfile)
    //    XTRACE("====================file open failed!====================");
    
    in_ ={0}, out_ = {0};
    
    in_.mFormatID = kAudioFormatLinearPCM;
    in_.mSampleRate = 44100;
    in_.mBitsPerChannel = 16;
    in_.mFramesPerPacket = 1;
    in_.mBytesPerFrame = 2;
    in_.mBytesPerPacket = in_.mBytesPerFrame * in_.mFramesPerPacket;
    in_.mChannelsPerFrame = 1;
    in_.mFormatFlags = kLinearPCMFormatFlagIsPacked | kLinearPCMFormatFlagIsSignedInteger;
    
    out_.mSampleRate = 44100;
    out_.mFormatID = kAudioFormatMPEG4AAC;
    out_.mFormatFlags = kMPEG4Object_AAC_LC;
    out_.mFramesPerPacket = 1024;
    out_.mChannelsPerFrame = 1;
    
    
    UInt32 outputBitrate = 64000;
    UInt32 propSize = sizeof(outputBitrate);
    UInt32 outputPacketSize = 0;
    
    bool sd = IsAACHardwareEncoderAvailable();
    if (sd) {
        
    }
    else{
        
    }
    
    const OSType subtype = kAudioFormatMPEG4AAC;
    AudioClassDescription requestedCodecs[2] = {
        {
            kAudioEncoderComponentType,
            subtype,
            kAppleSoftwareAudioCodecManufacturer
        },
        {
            kAudioEncoderComponentType,
            subtype,
            kAppleHardwareAudioCodecManufacturer
        }
    };
    
    result_ = AudioConverterNewSpecific(&in_, &out_, 2, requestedCodecs, &audio_converter_);
    if(!result_)
    {

    }

    if(result_ == noErr) {
        
        result_ = AudioConverterSetProperty(audio_converter_, kAudioConverterEncodeBitRate, propSize, &outputBitrate);
    }
    
    if(result_ == noErr) {
        
        result_ = AudioConverterGetProperty(audio_converter_, kAudioConverterPropertyMaximumOutputPacketSize, &propSize, &outputPacketSize);
    }
    
    output_packet_maxsize_ = outputPacketSize;
    output_packet_size_ = propSize;
    makeAsc(4, uint8_t(channel_count_));

    
    
    return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}

void HAACEncode::makeAsc(uint8_t sampleRateIndex, uint8_t channelCount)
{
    
    asc_[0] = 0x10 | ((sampleRateIndex>>1) & 0x3);
    asc_[1] = ((sampleRateIndex & 0x1)<<7) | ((channelCount & 0xF) << 3);
}


OSStatus HAACEncode::ioProc(AudioConverterRef inAudioConverter,
                                                                UInt32* ioNumberDataPackets,
                                                                AudioBufferList* ioData,
                                                                AudioStreamPacketDescription** ioDataPacketDescription,
                                                                void* inUserData)
{
    UserData* ud = static_cast<UserData*>(inUserData);
    
    UInt32 maxPackets = ud->size_ / ud->packet_size_;
    
    *ioNumberDataPackets = std::min(maxPackets, *ioNumberDataPackets);
    
    ioData->mBuffers[0].mData = ud->data_;
    ioData->mBuffers[0].mDataByteSize = ud->size_;
    ioData->mBuffers[0].mNumberChannels = 1;
    
    return noErr;
}

int HAACEncode::SendAdtsAACPacket(unsigned char* data, int size)
{
    mutex_.lock();
  
    int adtsLength = 7;
    char *packet = (char*)malloc(sizeof(char) * adtsLength);
    int profile = 2;
    int freqIdx = 4;
    int chanCfg = 1;
    int fullLength = adtsLength + size;

    packet[0] = (char)0xFF;
    packet[1] = (char)0xF9;
    packet[2] = (char)(((profile-1)<<6) + (freqIdx<<2) +(chanCfg>>2));
    packet[3] = (char)(((chanCfg&3)<<6) + (fullLength>>11));
    packet[4] = (char)((fullLength&0x7FF) >> 3);
    packet[5] = (char)(((fullLength&7)<<5) + 0x1F);
    packet[6] = (char)0xFC;

    memcpy(m_pAudioBuffer, packet, 7);
    memcpy(m_pAudioBuffer+7, data, size);
    
    free(packet);
    
    mutex_.unlock();
    return 0;
}

int HAACEncode::Encode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType)
{
    if(!IsInitialize())
        return EncoderFDefine::E_EncoderF_ErrorCode_UnInitialize;
    //fwrite(pSourceData, 1, lSourceDataLength, outfile);//pcm
    const size_t sampleCount = lSourceDataLength / 2;//2  4
    const size_t aac_packet_count = sampleCount / kSamplesPerFrame;
    const size_t required_bytes = aac_packet_count * output_packet_maxsize_;
    if(output_buffer_.total() < (required_bytes)) {
        output_buffer_.resize(required_bytes);
    }
    uint8_t* p = output_buffer_();
    uint8_t* p_out = (uint8_t*)pSourceData;
    AudioBufferList l;
    for ( size_t i = 0 ; i < aac_packet_count ; ++i ) {
        UInt32 num_packets = 1;
        
        
        l.mNumberBuffers=1;
        l.mBuffers[0].mDataByteSize = output_packet_maxsize_ * num_packets;
        l.mBuffers[0].mData = p;
        
        std::unique_ptr<UserData> ud(new UserData());
        ud->size_ = static_cast<int>(kSamplesPerFrame * 2);//2  4
        ud->data_ = const_cast<uint8_t*>(p_out);
        ud->packet_size_ = 2;//static_cast<int>(2);//2  4
        
        AudioStreamPacketDescription output_packet_desc[num_packets];
        mutex_.lock();
        int ret = AudioConverterFillComplexBuffer(audio_converter_, ioProc, ud.get(), &num_packets, &l, output_packet_desc);
//        XTRACE("========AudioConverterFillComplexBuffer ret:==========:%d\n", ret);
        mutex_.unlock();
        
        p += output_packet_desc[0].mDataByteSize;
        p_out += kSamplesPerFrame * 2;
    }
    const size_t totalBytes = p - output_buffer_();

    if(totalBytes) {
        
        *npOutLength = ((int)(totalBytes));
        memcpy(pOutputData, output_buffer_(), *npOutLength);
        //SendAdtsAACPacket(output_buffer_(), (int)totalBytes);
        //fwrite(m_pAudioBuffer, 1, totalBytes+7, outfile);
        if(NULL != npFrameType)
            *npFrameType = DecoderFDefine::E_DecoderF_FrameType_Audio;
    }
    
    return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}


int	HAACEncode::ReleaseEncoder()
{
    return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}

bool HAACEncode::IsInitialize()
{
    if(result_ == 3)
        return false;
    return true;
}

void HAACEncode::Release()
{
    delete this;
}

int	HAACEncode::GetInformationData(unsigned char* pBuffer,EncoderFDefine::E_EncoderF_Information_Type eType)
{
    ::memcpy(pBuffer, &asc_, sizeof(asc_));
    return sizeof(asc_);
}

int	HAACEncode::VideoEncode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t lTimestamp)
{
    return 0;
}
int		HAACEncode::VideoCopyEncode(void* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp)
{
    return 0;
}


int HAACEncode::SetYUVAngle(int nAngle)
{
    return 0;
}

void HAACEncode::SetDataCallback(EncoderFDefine::OutputData_Callback pOutputDataFunction, long uUserData)
{
    
}

void HAACEncode::SetBitrate(int bitrate){
    
}

#endif
