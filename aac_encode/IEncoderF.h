  
/*
版权所有 (c) 2014-2016,
V 1.0
作者: wujm <wfranco@sina.com,15018916607>
日期: 2014年09月22日
描述: 编码接口类
历史:
1、2014年09月22日 初步创建

使用说明：
*/

#ifndef _IEncoderF_h_
#define _IEncoderF_h_

#pragma once
#include "EncoderF_Define.h"

class IEncoderF
{
public:
	enum E_COLORFORMAT {
		E_COLOR_YUV420SP_NV12 = 0,
		E_COLOR_YUV420SP_NV21 = 1,
		E_COLOR_YUV420PLANNAR = 2,
	};

	typedef struct T_AVCEncoderParam
	{
//		OMX_COLOR_FORMATTYPE eFormat;//支持输入格式
//		OMX_VIDEO_AVCPROFILETYPE eAvcProfile;//适配出的profile
//		OMX_VIDEO_AVCLEVELTYPE eAvcLevel;//适配出的level
		E_COLORFORMAT eFormat;
		int eAvcProfile;
		int eAvcLevel;
		int iFrameInterval;
		int	iWidth;
		int iHeight;
		int iFrameRate;
		int iMaxBitRate;//编码器支持得最大码率
		int iBitRate;
		int iReserved1;
		int iReserved2;
		int iReserved3;
		T_AVCEncoderParam()
		{

		}
		T_AVCEncoderParam& operator=(const T_AVCEncoderParam& stParam)
		{
			eFormat = stParam.eFormat;
			eAvcProfile = stParam.eAvcProfile;
			eAvcLevel = stParam.eAvcLevel;
			iFrameInterval = stParam.iFrameInterval;
			iReserved1 = stParam.iReserved1;
			iReserved2 = stParam.iReserved2;
			iReserved3 = stParam.iReserved3;
			return *this;
		}

		T_AVCEncoderParam(const T_AVCEncoderParam& stParam)
		{
			eFormat = stParam.eFormat;
			eAvcProfile = stParam.eAvcProfile;
			eAvcLevel = stParam.eAvcLevel;
			iFrameInterval = stParam.iFrameInterval;
			iReserved1 = stParam.iReserved1;
			iReserved2 = stParam.iReserved2;
			iReserved3 = stParam.iReserved3;
		}
	};
public:

	//------------------------------------------------------------
	//功能：  初始化视频解码器
	//参数：  pData:数据				lDataLen:数据长度
	//返回：  错误代码
	//------------------------------------------------------------
	virtual int		Initialize(unsigned char* pData,long lDataLen) = 0;
    virtual int		Initialize(const IEncoderF::T_AVCEncoderParam& stEncoderParam)
    {
    	return 0;
    }
	//------------------------------------------------------------
	//功能：  编码
	//参数：  pSourceData:原始数据		lSourceDataLength:数据长度
	//		  pOutputData:编码后数据		npOutLength:编码后数据长度
	//		  npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
	//返回：  错误代码
	//------------------------------------------------------------
    virtual int		Encode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType) = 0;

    //------------------------------------------------------------
    //功能：  编码
    //参数：  pSourceData:原始数据		lSourceDataLength:数据长度
    //		pOutputData:编码后数据		npOutLength:编码后数据长度
    //		npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
    //		isUVInterlace:是否u和v向量交错
    //返回：  错误代码
    //------------------------------------------------------------
    virtual int		VideoCopyEncode(void* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp = 0){ return 0;}
    virtual int		VideoEncode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp) = 0;
	//------------------------------------------------------------
	//功能：  释放编码器
	//参数：  无
	//返回：  错误代码
	//------------------------------------------------------------
	virtual int		ReleaseEncoder() = 0;

	//------------------------------------------------------------
	//功能：  获取解码是否初始化状态
	//参数：  无
	//返回：  是否初始化
	//------------------------------------------------------------
	virtual bool	IsInitialize() = 0;
	
	//-------------------------------------------------------------------------------------
	// 功能：释放
	// 参数：无
	// 返回：无
	//-------------------------------------------------------------------------------------
	virtual void	Release() = 0;

	//-------------------------------------------------------------------------------------
	// 功能：获取信息数据
	// 参数：pBuffer[out]:需要复制信息的缓冲区，参考EncoderFDefine::E_EncoderF_Information_Type对应的结构体
	//		eType[in]:需要获取的信息数据类型
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int		GetInformationData(unsigned char* pBuffer,EncoderFDefine::E_EncoderF_Information_Type eType) = 0;
    
	//-------------------------------------------------------------------------------------
	// 功能：设置YUV旋转角度
	// 参数：nAngle  旋转角度值
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
    virtual int SetYUVAngle(int nAngle)
    {
    	return 0;
    }
    
    virtual void SetDataCallback(EncoderFDefine::OutputData_Callback pOutputDataFunction, long uUserData)
    {
    	return;
    }
    
    virtual void SetBitrate(int bitrate)
    {
    	return;

    }
  
};

#endif
