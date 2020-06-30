
/*
版权所有 (c) 2014-2016, 深圳市万佳安物联科技股份有限公司
保留所有权利.

V 1.0
作者: wujm <wujiangmin@wanjiaan.com, 15018916607>
日期: 2014年09月30日
描述: AAC音频编解码对象
历史:
1、2014年09月30日 初步创建

*/

#ifndef _AudioEncoder_AAC_h_
#define _AudioEncoder_AAC_h_

#pragma once

#include <MediaControl/EncoderF/IEncoderF.h>
#include "libfaac/faac.h"

class AudioEncoder_AAC: public IEncoderF
{
public:
	AudioEncoder_AAC();
	virtual ~AudioEncoder_AAC();

public:
	//------------------------------------------------------------
	//功能：  初始化视频解码器
	//参数：  pData:数据				lDataLen:数据长度
	//返回：  错误代码
	//------------------------------------------------------------
	virtual int	Initialize(unsigned char* pData,long lDataLen);
    virtual int		Initialize(const IEncoderF::T_AVCEncoderParam& stEncoderParam);

	//------------------------------------------------------------
	//功能：  编码
	//参数：  pSourceData:原始数据		lSourceDataLength:数据长度
	//		  pOutputData:编码后数据		npOutLength:编码后数据长度
	//		  npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
	//返回：  错误代码
	//------------------------------------------------------------
	virtual int		Encode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType);
    //------------------------------------------------------------
    //功能：  编码
    //参数：  pSourceData:原始数据		lSourceDataLength:数据长度
    //		pOutputData:编码后数据		npOutLength:编码后数据长度
    //		npFrameType:帧类型（参考DecoderFDefine::E_DecoderF_FrameType）
    //		isUVInterlace:是否u和v向量交错
    //返回：  错误代码
    //------------------------------------------------------------
    virtual int		VideoEncode(unsigned char* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace = TRUE,int64_t lTimestamp = 0);
    virtual int		VideoCopyEncode(void* pSourceData,long lSourceDataLength,unsigned char* pOutputData,int* npOutLength,int* npFrameType, bool bUVInterlace, int64_t llTimestamp = 0);

	//------------------------------------------------------------
	//功能：  释放编码器
	//参数：  无
	//返回：  错误代码
	//------------------------------------------------------------
	virtual int	ReleaseEncoder();

	//------------------------------------------------------------
	//功能：  获取解码是否初始化状态
	//参数：  无
	//返回：  是否初始化
	//------------------------------------------------------------
	virtual bool	IsInitialize();

	//-------------------------------------------------------------------------------------
	// 功能：释放
	// 参数：无
	// 返回：无
	//-------------------------------------------------------------------------------------
	virtual void	Release();

	//-------------------------------------------------------------------------------------
	// 功能：获取信息数据
	// 参数：pBuffer[out]:需要复制信息的缓冲区，参考EncoderFDefine::E_EncoderF_Information_Type对应的结构体
	//		eType[in]:需要获取的信息数据类型
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int	GetInformationData(unsigned char* pBuffer,EncoderFDefine::E_EncoderF_Information_Type eType);
 
	//-------------------------------------------------------------------------------------
	// 功能：设置YUV旋转角度
	// 参数：nAngle旋转角度值
	// 返回：错误代码
	//-------------------------------------------------------------------------------------
	virtual int SetYUVAngle(int nAngle) ;
    
    //---------------------------------------------------------------------------------------------
    //说明：设置编码数据输出回调
    //参数：
    //		pOutputDataFunction：回调函数指针
    //		ulUserData：用户数据
    //返回：无
    //---------------------------------------------------------------------------------------------
    virtual void SetDataCallback(EncoderFDefine::OutputData_Callback pOutputDataFunction, long uUserData);
    virtual void SetBitrate(int bitrate);
private:
    faacEncHandle					m_hEncoder;			//编码句柄
    int								m_nBitsPerSample;	//单样本位数
    char					        m_PcmBuffer[1024 * 50];
    int 							m_BufferSize;
    unsigned long 					m_nMaxInputBytes;
};

#endif
