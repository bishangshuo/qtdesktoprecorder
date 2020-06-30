#include "StdAfx.h"
#include "AudioEncoder_AAC.h"
#include <MediaControl/EncoderF/EncoderF_Define.h>
#include <MediaControl/FileReadWriteF/FileReadWriteF_Define.h>
#include <MediaControl/DecoderF/DecoderF_Define.h>
#include <BaseClass/BaseFunction.h>
#include <string.h>
#include <stdlib.h>

AudioEncoder_AAC::AudioEncoder_AAC()
{
	m_hEncoder = NULL;
	m_BufferSize = 0;
	m_nMaxInputBytes = 0;
	memset(m_PcmBuffer,0,sizeof(m_PcmBuffer));
}

AudioEncoder_AAC::~AudioEncoder_AAC()
{
}

int		AudioEncoder_AAC::Initialize(const IEncoderF::T_AVCEncoderParam& stEncoderParam)
{
    return 0;
}

//------------------------------------------------------------
//功能：  初始化视频解码器
//参数：  pData:数据				lDataLen:数据长度
//返回：  错误代码
//------------------------------------------------------------
int	AudioEncoder_AAC::Initialize(unsigned char* pData,long lDataLen)
{
	if(NULL != m_hEncoder)
		return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
	if(lDataLen != sizeof(FileReadWriteFDefine::S_FileReadWriteF_FileInfo_Audio))
		return EncoderFDefine::E_EncoderF_ErrorCode_InvalidParameter;

	FileReadWriteFDefine::S_FileReadWriteF_FileInfo_Audio* pAudio = (FileReadWriteFDefine::S_FileReadWriteF_FileInfo_Audio*)pData;

	int nSampleRate = pAudio->nSamplesPerSec;	// 采样率
	int nChannels = pAudio->nChannels;			// 声道数
	int nBit = pAudio->nBitsPerSample;			// 单样本位数
	unsigned long nInputSamples = 0;			// 输入样本数
	unsigned long nMaxOutputBytes = 0;			// 输出所需最大空间
	unsigned long nMaxInputBytes=0;				// 输入最大字节

	m_hEncoder = faacEncOpen(nSampleRate, nChannels, &nInputSamples, &nMaxOutputBytes);//初始化aac句柄，同时获取最大输入样本，及编码所需最小字节
	nMaxInputBytes=nInputSamples*nBit/8;//计算最大输入字节,跟据最大输入样本数
	m_nMaxInputBytes = nMaxInputBytes;


	faacEncConfigurationPtr pConfiguration = NULL;				//aac设置指针
	pConfiguration = faacEncGetCurrentConfiguration(m_hEncoder);//获取配置结构指针
	pConfiguration->inputFormat = FAAC_INPUT_16BIT;
	pConfiguration->outputFormat = 0;//0:RAW——STREAM	1:ADTS-STREAM
	pConfiguration->useTns = true;
	pConfiguration->useLfe = false;
	pConfiguration->aacObjectType = LOW;
	pConfiguration->shortctl = SHORTCTL_NORMAL;
	pConfiguration->quantqual = 100;
	pConfiguration->bandWidth = 0;
	pConfiguration->bitRate = 0;
	pConfiguration->mpegVersion = MPEG4;
	pConfiguration->version = MPEG4;
	
	int nResult = faacEncSetConfiguration(m_hEncoder, pConfiguration);//设置配置，根据不同设置，耗时不一样

//	unsigned char* pConfigBuffer = NULL;
//	unsigned long ulConfigLength = 0;
//	faacEncGetDecoderSpecificInfo(m_hEncoder,&pConfigBuffer, &ulConfigLength);

	m_nBitsPerSample = nBit;

	return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}

//------------------------------------------------------------
//功能：  编码
//参数：  pSourceData:原始数据		lSourceDataLength:数据长度
//		  pOutputData:编码后数据		npOutLength:编码后数据长度
//		  npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
//返回：  错误代码
//------------------------------------------------------------
int	AudioEncoder_AAC::Encode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType)
{
	if(!IsInitialize())
			return EncoderFDefine::E_EncoderF_ErrorCode_UnInitialize;

	    //计算实际输入样本数
		int nInputSamples = lSourceDataLength/ (m_nBitsPerSample / 8);
		int nEncodedLength = 0;
		unsigned int nMaxOutLength = *npOutLength;
		for(;;)
		{
			nEncodedLength = faacEncEncode(m_hEncoder, (int*)pSourceData, nInputSamples, pOutputData, nMaxOutLength);
			if (nEncodedLength < 1)
				continue;

			break;
		}

		if(NULL != npOutLength)
			*npOutLength = nEncodedLength;

		if(NULL != npFrameType)
			*npFrameType = DecoderFDefine::E_DecoderF_FrameType_Audio;


//    XTRACE("aac encode:%d", *npOutLength);

	return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}

//------------------------------------------------------------
//功能：  释放编码器
//参数：  无
//返回：  错误代码
//------------------------------------------------------------
int	AudioEncoder_AAC::ReleaseEncoder()
{
	if(NULL == m_hEncoder)
		return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;

	int nResult = faacEncClose(m_hEncoder);

	return EncoderFDefine::E_EncoderF_ErrorCode_Succeed;
}

//------------------------------------------------------------
//功能：  获取解码是否初始化状态
//参数：  无
//返回：  是否初始化
//------------------------------------------------------------
bool	AudioEncoder_AAC::IsInitialize()
{
	if(NULL == m_hEncoder)
		return false;

	return true;
}

//-------------------------------------------------------------------------------------
// 功能：释放
// 参数：无
// 返回：无
//-------------------------------------------------------------------------------------
void	AudioEncoder_AAC::Release()
{
	delete this;
}

//-------------------------------------------------------------------------------------
// 功能：获取信息数据
// 参数：pBuffer[out]:需要复制信息的缓冲区，参考EncoderFDefine::E_EncoderF_Information_Type对应的结构体
//		eType[in]:需要获取的信息数据类型
// 返回：错误代码
//-------------------------------------------------------------------------------------
int	AudioEncoder_AAC::GetInformationData(unsigned char* pBuffer,EncoderFDefine::E_EncoderF_Information_Type eType)
{
    if(NULL == m_hEncoder)
        return -1;
    
    unsigned char* pConfigBuffer = NULL;
    unsigned long ulConfigLength = 0;
    faacEncGetDecoderSpecificInfo(m_hEncoder, &pConfigBuffer, &ulConfigLength);
	XTRACE( "ulConfigLength=%d data=%x %x", ulConfigLength, pConfigBuffer[0], pConfigBuffer[1]);

    ::memcpy(pBuffer, pConfigBuffer, ulConfigLength);
    free(pConfigBuffer);
    
	return ulConfigLength;
}

//------------------------------------------------------------
//功能：  编码
//参数：  pSourceData:原始数据		lSourceDataLength:数据长度
//		pOutputData:编码后数据		npOutLength:编码后数据长度
//		npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
//		isUVInterlace:是否u和v向量交错
//返回：  错误代码
//------------------------------------------------------------
int		AudioEncoder_AAC::VideoEncode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t lTimestamp)
{

	return 0;
}

int		AudioEncoder_AAC::VideoCopyEncode(void* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp)
{
    return 0;
}

//-------------------------------------------------------------------------------------
// 功能：设置YUV旋转角度
// 参数：nAngle旋转角度值
// 返回：错误代码
//-------------------------------------------------------------------------------------
 int AudioEncoder_AAC::SetYUVAngle(int nAngle)
 {
	 return 0;
 }
//---------------------------------------------------------------------------------------------
//说明：设置编码数据输出回调
//参数：
//		pOutputDataFunction：回调函数指针
//		ulUserData：用户数据
//返回：无
//---------------------------------------------------------------------------------------------
void AudioEncoder_AAC::SetDataCallback(EncoderFDefine::OutputData_Callback pOutputDataFunction, long uUserData)
{
    
}

void AudioEncoder_AAC::SetBitrate(int bitrate){
    
}
