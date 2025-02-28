/**
 * Copyright (C) 2023, Advanced Micro Devices. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following disclaimer in the documentation
 * and/or other materials provided with the distribution.
 * 3. Neither the name of the copyright holder nor the names of its
 * contributors may be used to endorse or promote products derived from this
 * software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
 
 /** @file zstd_gtest.cc
 *  
 *  @brief Test cases for ZSTD algo.
 *
 *  This file contains the test cases for ZSTD method
 *  testing the API level functions of ZSTD.
 *
 *  @author Partiksha
 */

#include <string>
#include <climits>
#include "gtest/gtest.h"

#ifndef AOCL_EXCLUDE_ZSTD
#define ZSTD_STATIC_LINKING_ONLY
#include "algos/zstd/lib/zstd.h"
#endif

#define DEFAULT_OPT_LEVEL 2 // system running gtest must have AVX support

/* This base class can be used for all fixtures
 * that require dynamic dispatcher setup */
class AOCL_setup_zstd : public ::testing::Test {
public:
    AOCL_setup_zstd() {
        int optLevel = DEFAULT_OPT_LEVEL;
        aocl_setup_zstd_encode(0, optLevel, 0, 0, 0);
        aocl_setup_zstd_decode(0, optLevel, 0, 0, 0);
    }
};


#define ZSTD_COMPRESS_HEAPMODE

class TestLoad_2
{
    //source buffer (original data which we intend to compress).
    char *orig_data;
    size_t orig_sz = 0;
    //destination buffer (kdata obtained after compression).
    char *compressed_data;
    size_t compressed_sz=0;

public:
    // Constructor functions, creates `sz` size of source data.
    TestLoad_2(int sz)
    {
        this->orig_sz = sz;
        orig_data = (char *)malloc(sz);

        // generating random data inside `orig_data` buffer.
        for (int i = 0; i < sz; i++)
        {
            orig_data[i] = rand() % 255;
        }

        // Provides the maximum size that ZSTD compression may output in a "worst case".
        compressed_sz = ZSTD_compressBound(sz);
        compressed_data = (char *)malloc(compressed_sz);
    }
    // Returns pointer to source buffer.
    char *getOrigData()
    {
        return orig_data;
    }
    // Returns size of source buffer.
    size_t getOrigSize()
    {
        return orig_sz;
    }
    // Returns pointer to destination buffer (data obtained after compression).
    char *getCompressedBuff()
    {
        return compressed_data;
    }
    // Returns size of destination data.
    size_t getCompressedSize()
    {
        return compressed_sz;
    }
    // Destructor function.
    ~TestLoad_2()
    {
        free(orig_data);
        free(compressed_data);
    }
};

bool zstd_check_uncompressed_equal_to_original(char *src, unsigned srcSize, char *compressed, unsigned compressedLen)
{
    int uncompressedLen = srcSize + 10;
    char* uncompressed = (char*)calloc(uncompressedLen, sizeof(char));
    
    
    ZSTD_DCtx* const dctx = ZSTD_createDCtx();
    int uncompressedLenRes = ZSTD_decompressDCtx(dctx, uncompressed, uncompressedLen, compressed, compressedLen);

    if (uncompressedLenRes < 0) {//error code
        free(uncompressed);
        return false;
    }

    if (!(srcSize == (unsigned)uncompressedLenRes)) {
        free(uncompressed);
        return false;
    }

    bool ret = (memcmp(src, uncompressed, srcSize) == 0);
    free(uncompressed);
    return ret;
}

// Test wrapper function for API ZSTD_versionNumber()
unsigned Test_ZSTD_versionNumber()
{
    return ZSTD_versionNumber();
}

// Test wrapper function for API ZSTD_versionString()
const char* Test_ZSTD_versionString()
{
    return ZSTD_versionString();
}

// Test wrapper function for API ZSTD_compress()
size_t Test_ZSTD_compress(void* dst, size_t dstCapacity,
               const void* src, size_t srcSize,
                     int compressionLevel)
{
    return ZSTD_compress(dst, dstCapacity, src, srcSize, compressionLevel);
}

// Test wrapper function for API ZSTD_decompress()
size_t Test_ZSTD_decompress(void* dst, size_t dstCapacity, const void* src, size_t srcSize)
{
    return ZSTD_decompress(dst, dstCapacity, src, srcSize);
}

// Test wrapper function for ZSTD_isError()
size_t Test_ZSTD_isError(size_t len)
{
    return ZSTD_isError(len);
}

// Test wrapper function for ZSTD_decompressedbound()
unsigned long long Test_ZSTD_decompressBound(const void *src, size_t srcLen)
{
    return ZSTD_decompressBound(src, srcLen); 
}

// Test wrapper function for ZSTD_frameHeaderSize()
size_t Test_ZSTD_frameHeaderSize(const void* src, size_t srcSize)
{
    return ZSTD_frameHeaderSize(src, srcSize);
}


/***********************************************
 * "Begin" of Version Tests
 ***********************************************/

TEST(ZSTD_versionNumber, AOCL_Compression_zstd_ZSTD_versionNumber_common_1) // Number
{
    EXPECT_EQ(Test_ZSTD_versionNumber(), ZSTD_VERSION_NUMBER);
}

TEST(ZSTD_versionString, AOCL_Compression_zstd_ZSTD_versionString_common_2) // String
{
    EXPECT_STREQ(Test_ZSTD_versionString(), ZSTD_VERSION_STRING);
}
/*********************************************
 * End of Version Tests
 *********************************************/


/***********************************************
 * "Begin" of ZSTD_compress
 ***********************************************/
class ZSTD_ZSTD_compress : public AOCL_setup_zstd {
};

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_1) // compress_FAIL_src_is_NULL
{
    TestLoad_2 d(8000);
    
    EXPECT_EQ(Test_ZSTD_compress(d.getCompressedBuff(), d.getCompressedSize(), NULL, d.getOrigSize(), 1), -1);
}

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_2) // Compress_FAIL_dst_is_NULL
{
    TestLoad_2 d(800);
    EXPECT_EQ(Test_ZSTD_compress(NULL, d.getCompressedSize(), d.getOrigData(), d.getOrigSize(), 1), -1);
}

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_3 ) // compress_PASS
{
    for(int cLevel=1; cLevel<=22; cLevel++) {
        TestLoad_2 d(8000);
        size_t outLen = Test_ZSTD_compress(d.getCompressedBuff(), d.getCompressedSize(), d.getOrigData(),d.getOrigSize(), cLevel);
        EXPECT_TRUE(zstd_check_uncompressed_equal_to_original(d.getOrigData(), d.getOrigSize(), d.getCompressedBuff(), outLen));
    }
}

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_4) // compress_FAIL_dst_size_not_enough
{
    TestLoad_2 d(800);
    size_t outLen = Test_ZSTD_compress(d.getCompressedBuff(), d.getOrigSize() / 20, d.getOrigData(), d.getOrigSize(), 1);
    EXPECT_TRUE(Test_ZSTD_isError(outLen));
}

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_5) // Compression_level_less_than_minimum_limit
{
    TestLoad_2 d(800);
    int cLevel = -1;
    /* For levels < 1, compression parameters are set to 0th entry of the table `ZSTD_defaultCParameters[4][ZSTD_MAX_CLEVEL+1]`
     * while other levels uses the corresponding entry to set compression parameters.
     */
    size_t outLen = Test_ZSTD_compress(d.getCompressedBuff(), d.getCompressedSize(), d.getOrigData(), d.getOrigSize(), cLevel);
    EXPECT_TRUE(zstd_check_uncompressed_equal_to_original(d.getOrigData(), d.getOrigSize(), d.getCompressedBuff(), outLen));
}

TEST_F(ZSTD_ZSTD_compress, AOCL_Compression_ZSTD_ZSTD_compress_common_6) // Compression_level_greater_than_maximum_limit
{
    TestLoad_2 d(800);
    int cLevel = 23;
    // For level > maximum possible level, level will be set to ZSTD_MAX_CLEVEL, which is 22.
    size_t outLen = Test_ZSTD_compress(d.getCompressedBuff(), d.getCompressedSize(), d.getOrigData(),d.getOrigSize(), cLevel);
    EXPECT_TRUE(zstd_check_uncompressed_equal_to_original(d.getOrigData(), d.getOrigSize(), d.getCompressedBuff(), outLen));
}

/*********************************************
 * End of ZSTD_compress
 *********************************************/


/***********************************************
 * "Begin" of ZSTD_decompress
 ***********************************************/

class ZSTD_ZSTD_decompress : public AOCL_setup_zstd
{
public:
    TestLoad_2 *d = NULL;
    
    // Compressed data is stored in the buffer `src`.
    char *src = NULL;
    int srcLen;
    
    // Original data which we are about to compress is contained in the buffer `original`.
    char *original = NULL;
    int origLen;
    
    // Decompressed data will be stored in the buffer `output`.
    char *output = NULL;
    int outLen;

    //compression Level 
    int cLevel = ZSTD_CLEVEL_DEFAULT;

    // By default the `original` buffer of size 800 is initialized upon testcase initialization.
    void SetUp() override
    {
        d = new TestLoad_2(800);
        original = d->getOrigData();
        origLen = d->getOrigSize();
        src = d->getCompressedBuff();

        // Compress data from `original` buffer to `src` buffer.
        srcLen = Test_ZSTD_compress(src, d->getCompressedSize(), original, origLen, cLevel);
        outLen = d->getOrigSize()*2 + 10;
        output = (char *)malloc(outLen);
    }
    
    // By default the `original` buffer size is set to 800
    // we can use `setOrigSz(int sz)` function to reset it size to sz.
    void setOrigSz(int sz)
    {
        if (d != NULL)
            delete d;
        d = new TestLoad_2(sz);
        original = d->getOrigData();
        origLen = d->getOrigSize();
        srcLen = d->getCompressedSize();
        src = d->getCompressedBuff();
        srcLen = Test_ZSTD_compress(src, srcLen, original, origLen, cLevel);
    }

    // Reset `output` buffer size to `sz`.
    void setDstSz(int sz)
    {
        if (output)
            free(output);
        outLen = sz;
        output = (char *)malloc(outLen);
    }

    // set compression level
    void set_clevel(int level)
    {
        cLevel = level;
    }

    // Destructor function.
    ~ZSTD_ZSTD_decompress()
    {
        if(d)
            delete d;
        if(output)
            free(output);
    }
};

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_1) // src_NULL
{
    EXPECT_EQ(Test_ZSTD_decompress(output, Test_ZSTD_decompressBound(src, srcLen), NULL, srcLen), -1);
}

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_2) // dst_NULL
{
    EXPECT_EQ(Test_ZSTD_decompress(NULL, Test_ZSTD_decompressBound(src, srcLen), src, srcLen), -1);
}

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_3) // successfull_decompression
{
    int decLen = Test_ZSTD_decompress(output, Test_ZSTD_decompressBound(src, srcLen), src, srcLen);
    ASSERT_EQ(origLen, decLen);
    EXPECT_EQ(0, memcmp(output, original, decLen));
}

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_4) // decompression_buffer_inadequate
{
    size_t decompressedLen = Test_ZSTD_decompress(output, srcLen / 20, src, srcLen);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen));
}

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_5) // compressed_size_zero
{
    EXPECT_EQ(Test_ZSTD_decompress(output, Test_ZSTD_decompressBound(src, srcLen), src, 0), 0);
}

TEST_F(ZSTD_ZSTD_decompress, AOCL_Compression_zstd_ZSTD_decompress_common_6) // compressed_data_contains_errors
{
    // case 1: Introducing errors in case where input size is small
    const int origLen_1 = 100;
    char orig[origLen_1];
    char dst[100];
    int dstCapacity = ZSTD_compressBound(origLen_1);
    int level = 8;

    for (int i = 0; i < 100; i++)
        orig[i] = 'a';
    

    dstCapacity = Test_ZSTD_compress(dst, dstCapacity, orig, origLen_1, level);
    int frameHeaderSize = Test_ZSTD_frameHeaderSize(dst, dstCapacity);
    size_t decompress_bound = Test_ZSTD_decompressBound(dst, dstCapacity);
    dst[frameHeaderSize + 2] = 2;

    size_t decompressedLen = Test_ZSTD_decompress(output, decompress_bound, dst, dstCapacity);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen));


    // case 2: modifying compressed data of class to introduce error.
    frameHeaderSize = Test_ZSTD_frameHeaderSize(src, srcLen);
    decompress_bound = Test_ZSTD_decompressBound(src, srcLen);
    this->src[frameHeaderSize+1] = 'e';
    this->src[frameHeaderSize+2] = 'r';
    this->src[frameHeaderSize+3] = 'r';
    this->src[frameHeaderSize+4] = 'o';
    this->src[frameHeaderSize+5] = 'r';

    size_t decompressedLen_2 = Test_ZSTD_decompress(output, decompress_bound, src, srcLen);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen_2));
}

/*********************************************
 * End of ZSTD_decompress
 *********************************************/

/*********************************************
 * Begin of ZSTD_getframeContentSize
 *********************************************/

#define ZSTD_CONTENTSIZE_UNKNOWN (0ULL - 1)
#define ZSTD_CONTENTSIZE_ERROR   (0ULL - 2)
class ZSTD_getframeContentSize: public ZSTD_ZSTD_decompress
{
protected:
    // Test wrapper function for API ZSTD_getFrameContentSize()
    unsigned long long Test_ZSTD_getFrameContentSize(const void *src, size_t srcSize)
    {
        return ZSTD_getFrameContentSize(src, srcSize);
    }
};

TEST_F(ZSTD_getframeContentSize, AOCL_Compression_zstd_ZSTD_getFrameContentSize_common_1)   // pass- size >= `ZSTD_frameHeaderSize_max`
{
    unsigned long long val = Test_ZSTD_getFrameContentSize(src, srcLen);
    int decLen = Test_ZSTD_decompress(output, Test_ZSTD_decompressBound(src, srcLen), src, srcLen);
    EXPECT_EQ(val, decLen);
}

TEST_F(ZSTD_getframeContentSize, AOCL_Compression_zstd_ZSTD_getFrameContentSize_common_2)   // Fail- size < `ZSTD_frameHeaderSize_min i.e, 1`
{
    unsigned long long val = Test_ZSTD_getFrameContentSize(src, 1);
    EXPECT_EQ(val, ZSTD_CONTENTSIZE_ERROR);
}

/*********************************************
 * End of ZSTD_getframeContentSize
 *********************************************/


/*********************************************
 * Begin of ZSTD_getDecompressedSize
 *********************************************/

class ZSTD_ZSTD_getDecompressedSize: public ZSTD_ZSTD_decompress
{
protected:
    // Test wrapper function for API ZSTD_getDecompressedSize()
    unsigned long long Test_ZSTD_getDecompressedSize(const void* src, size_t srcSize)
    {
        return ZSTD_getDecompressedSize(src, srcSize);
    }
};

TEST_F(ZSTD_ZSTD_getDecompressedSize, AOCL_Compression_zstd_ZSTD_getDecompressedSize_common_1)   // pass- size is valid
{
    unsigned long long val = Test_ZSTD_getDecompressedSize(src, srcLen);
    int decLen = Test_ZSTD_decompress(output, Test_ZSTD_decompressBound(src, srcLen), src, srcLen);
    EXPECT_EQ(val, decLen);
}

TEST_F(ZSTD_ZSTD_getDecompressedSize, AOCL_Compression_zstd_ZSTD_getDecompressedSize_common_2)   // fail- size = 1
{
    EXPECT_EQ(Test_ZSTD_getDecompressedSize(src, 1), 0);
}

/*********************************************
 * End of ZSTD_getDecompressedSize
 *********************************************/


/*********************************************
 * Begin of ZSTD_findFrameCompressedSize
 *********************************************/

class ZSTD_ZSTD_findFrameCompressedSize: public ZSTD_ZSTD_decompress
{
protected:
    // Test wrapper function for API ZSTD_findFrameCompressedSize()
    size_t Test_ZSTD_findFrameCompressedSize(const void *src, size_t srcSize)
    {
        return ZSTD_findFrameCompressedSize(src, srcSize);
    }
};

TEST_F(ZSTD_ZSTD_findFrameCompressedSize, AOCL_Compression_zstd_ZSTD_findFrameCompressedSize_common_1)   // srcLen >= at least as large as the frame contained
{
    while(srcLen >= 5){
        unsigned long long frameSrcSize = Test_ZSTD_findFrameCompressedSize(src, srcLen);
        src = src + frameSrcSize;
        srcLen -= frameSrcSize;
    }
    EXPECT_EQ(0, srcLen);
}

TEST_F(ZSTD_ZSTD_findFrameCompressedSize, AOCL_Compression_zstd_ZSTD_findFrameCompressedSize_common_2)   // srcLen < the size that frame contained
{
    srcLen = 5;
    unsigned long long frameSrcSize = Test_ZSTD_findFrameCompressedSize(src, srcLen);
    EXPECT_TRUE(Test_ZSTD_isError(frameSrcSize)); 
}

/*********************************************
 * End of ZSTD_findFrameCompressedSize
 *********************************************/


/*********************************************
 * Begin of ZSTD_compress_advanced
 *********************************************/

class ZSTD_ZSTD_compressed_advanced: public AOCL_setup_zstd
{
public:
    // dctx is a paramter in `ZSTD_ZSTD_decompressDCtx`
    ZSTD_CCtx* cctx;
    ZSTD_parameters param;
    
    // constructor to create dctx
    void SetUp() override
    {
        cctx = ZSTD_createCCtx();
    }

    // Test wrapper function for API ZSTD_compress_advanced()
    size_t Test_ZSTD_compress_advanced (ZSTD_CCtx* cctx,
                               void* dst, size_t dstCapacity,
                         const void* src, size_t srcSize,
                         const void* dict,size_t dictSize,
                               ZSTD_parameters params)
    {
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
        return ZSTD_compress_advanced(cctx, dst, dstCapacity, src, srcSize, dict, dictSize, params);
#pragma GCC diagnostic pop
    }

    ~ZSTD_ZSTD_compressed_advanced()
    {
        ZSTD_freeCCtx(cctx);
    }

};

TEST_F(ZSTD_ZSTD_compressed_advanced, AOCL_Compression_zstd_ZSTD_compress_advanced_common_1)
{
    TestLoad_2 d(8000);
    int level = 9;

    param = ZSTD_getParams(level, d.getOrigSize(), 0);
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, level);

    size_t outLen_1 = Test_ZSTD_compress_advanced(cctx, d.getCompressedBuff(), d.getCompressedSize(), d.getOrigData(), d.getOrigSize(), NULL, 0, param);

    aocl_setup_zstd_encode(1, 0, 0, 0, 0);   // optoff = 1
    
    param = ZSTD_getParams(level, d.getOrigSize(), 0);
    ZSTD_CCtx_setParameter(cctx, ZSTD_c_compressionLevel, level);

    size_t outLen_2 = Test_ZSTD_compress_advanced(cctx, d.getCompressedBuff(), d.getCompressedSize(), d.getOrigData(), d.getOrigSize(), NULL, 0, param);

    EXPECT_EQ(outLen_1, outLen_2);
    EXPECT_TRUE(zstd_check_uncompressed_equal_to_original(d.getOrigData(), d.getOrigSize(), d.getCompressedBuff(), outLen_1));
    EXPECT_TRUE(zstd_check_uncompressed_equal_to_original(d.getOrigData(), d.getOrigSize(), d.getCompressedBuff(), outLen_2));
}

/*********************************************
 * End of ZSTD_compress_advanced
 *********************************************/


/*********************************************
 * Begin of ZSTD_decompressDCtx
 *********************************************/
class ZSTD_ZSTD_decompressDCtx: public ZSTD_ZSTD_decompress
{
public:
    // dctx is a paramter in `ZSTD_ZSTD_decompressDCtx`
    ZSTD_DCtx* dctx;
    
    // constructor to create dctx
    ZSTD_ZSTD_decompressDCtx()
    {
        dctx = ZSTD_createDCtx();
    }

    // Test wrapper function for API ZSTD_decompressDCtx()
    size_t Test_ZSTD_decompressDCtx(ZSTD_DCtx* dctx, void* dst, size_t dstCapacity, const void* src, size_t srcSize)
    {
        return ZSTD_decompressDCtx(dctx, dst, dstCapacity, src, srcSize);
    }

    ~ZSTD_ZSTD_decompressDCtx()
    {
        ZSTD_freeDCtx(dctx);
    }

};

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_1) // dctx_NULL
{
    EXPECT_EQ(Test_ZSTD_decompressDCtx(NULL, output, Test_ZSTD_decompressBound(src, srcLen), src, srcLen), -1);
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_2) // src_NULL
{
    EXPECT_EQ(Test_ZSTD_decompressDCtx(dctx, output, Test_ZSTD_decompressBound(src, srcLen), NULL, srcLen), -1);
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_3) // dst_NULL
{
    EXPECT_EQ(Test_ZSTD_decompressDCtx(dctx, NULL, Test_ZSTD_decompressBound(src, srcLen), src, srcLen), -1);
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_4) // successfull_decompression
{
    int decLen = Test_ZSTD_decompressDCtx(dctx, output, Test_ZSTD_decompressBound(src, srcLen), src, srcLen);
    ASSERT_EQ(origLen, decLen);
    EXPECT_EQ(0, memcmp(output, original, decLen));
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_5) // decompression_buffer_inadequate
{
    size_t decompressedLen = Test_ZSTD_decompressDCtx(dctx, output, srcLen / 20, src, srcLen);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen));
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_6) // compressed_size_zero
{
    EXPECT_EQ(Test_ZSTD_decompressDCtx(dctx, output, Test_ZSTD_decompressBound(src, srcLen), src, 0), 0);
}

TEST_F(ZSTD_ZSTD_decompressDCtx, AOCL_Compression_zstd_ZSTD_decompressDCtx_common_7) // compressed_data_contains_errors
{
    // case 1: Introducing errors in case where input size is small
    const int origLen_1 = 100;
    char orig[origLen_1];
    char dst[100];
    int dstCapacity = ZSTD_compressBound(origLen_1);
    int level = 8;

    for (int i = 0; i < 100; i++)
        orig[i] = 'a';
    

    dstCapacity = Test_ZSTD_compress(dst, dstCapacity, orig, origLen_1, level);
    int frameHeaderSize = Test_ZSTD_frameHeaderSize(dst, dstCapacity);
    size_t decompress_bound = Test_ZSTD_decompressBound(dst, dstCapacity);
    dst[frameHeaderSize + 2] = 2;

    size_t decompressedLen = Test_ZSTD_decompressDCtx(dctx, output, decompress_bound, dst, dstCapacity);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen));


    // case 2: modifying compressed data of class to introduce error.
    frameHeaderSize = Test_ZSTD_frameHeaderSize(src, srcLen);
    decompress_bound = Test_ZSTD_decompressBound(src, srcLen);
    this->src[frameHeaderSize+1] = 'e';
    this->src[frameHeaderSize+2] = 'r';
    this->src[frameHeaderSize+3] = 'r';
    this->src[frameHeaderSize+4] = 'o';
    this->src[frameHeaderSize+5] = 'r';

    size_t decompressedLen_2 = Test_ZSTD_decompressDCtx(dctx, output, decompress_bound, src, srcLen);
    EXPECT_TRUE(Test_ZSTD_isError(decompressedLen_2));

}

/*********************************************
 * End of ZSTD_decompressDCtx
 *********************************************/



