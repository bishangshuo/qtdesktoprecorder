//
//  AACBuffer.h
//  AVDemo
//
//  Created by LiuJun on 16/4/12.
//  Copyright © 2016年 frank. All rights reserved.
//
#if defined(__APPLE__)
#ifndef AACBuffer_h
#define AACBuffer_h

#include <memory>
#include <vector>
#include <string>
#include <stdint.h>
#include <mutex>

template<typename T>
struct MasterBuffer
{
    MasterBuffer() : total_(0), size_(0) {};
    
    MasterBuffer(size_t I) : total_(I), size_(0) {
        //memset(&m_buffer[0], 0, I);
    };
    
    virtual size_t put(uint8_t* buf, size_t size)
    {
        if(size > total_) size = total_;
        
        memcpy(&buffer_[0], buf, size);
        size_ = size;
        return size;
    }
    virtual size_t read(uint8_t** buf, size_t size) {
        if(size > size_) size = size_;
        
        *buf = &buffer_[0];
        
        return size;
    }
    virtual void setSize(size_t size) { size_ = size; };
    virtual size_t resize(size_t size) =0;
    virtual size_t size() const { return size_; };
    virtual size_t total() const { return total_; };
    virtual void clear() { size_ = 0; };
    
protected:
    T buffer_;
    size_t total_;
    size_t size_;
};

struct HAACBuffer : MasterBuffer<std::unique_ptr<uint8_t[]> >
{
    HAACBuffer() : MasterBuffer() {};
    HAACBuffer(size_t I) : MasterBuffer(I)
    {
        resize(I);
    };
    virtual size_t resize(size_t size) {
        if(size > 0) {
            buffer_.reset();
            
            buffer_.reset(new uint8_t[size]);
        } else {
            buffer_.reset();
        }
        total_ = size;
        size_ = 0;
        
        return size;
    }
    virtual uint8_t* operator ()() const { return &buffer_[0]; };
};

#endif /* AACBuffer_h */

#endif
