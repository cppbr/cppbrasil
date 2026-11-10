/*
Copyright 2022 João Elson
jetmorju@gmail.com

Web-site: https://cppbrasil.com.br

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#include "cppzip.h"

CppZip::CppZip()
{

}

bool CppZip::compress(const QByteArray &input, QByteArray &output)
{
    if (input.isEmpty())
        return false;

    const int BUFSIZE = 128 * 1024;
    std::vector<char> tmpBuf(BUFSIZE);

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)input.data();
    strm.avail_in = uInt(input.size());
    strm.next_out = reinterpret_cast<Bytef *>(tmpBuf.data());
    strm.avail_out = BUFSIZE;

    // windowBits = 15 + 16 to enable gzip
    // From the zlib manual: windowBits can also be greater than 15 for optional gzip encoding. Add 16 to windowBits
    // to write a simple gzip header and trailer around the compressed data instead of a zlib wrapper.
    int result = deflateInit2(&strm, Z_DEFAULT_COMPRESSION, Z_DEFLATED, (15 + 16), 9, Z_DEFAULT_STRATEGY);
    if (result != Z_OK)
       return false;

    output.reserve(deflateBound(&strm, input.size()));

    // feed to deflate
    while (strm.avail_in > 0)
    {
        result = deflate(&strm, Z_NO_FLUSH);

        if (result != Z_OK)
        {
            deflateEnd(&strm);
            return false;
        }

        output.append(tmpBuf.data(), (BUFSIZE - strm.avail_out));
        strm.next_out = reinterpret_cast<Bytef *>(tmpBuf.data());
        strm.avail_out = BUFSIZE;
    }

    // flush the rest from deflate
    while (result != Z_STREAM_END)
    {
        result = deflate(&strm, Z_FINISH);

        output.append(tmpBuf.data(), (BUFSIZE - strm.avail_out));
        strm.next_out = reinterpret_cast<Bytef *>(tmpBuf.data());
        strm.avail_out = BUFSIZE;
    }

    deflateEnd(&strm);

    return true;
}

bool CppZip::decompress(const QByteArray &input, QByteArray &output)
{
    if (input.isEmpty())
        return false;

    const int BUFSIZE = 1024 * 1024;
    std::vector<char> tmpBuf(BUFSIZE);

    z_stream strm;
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    strm.next_in = (Bytef *)input.data();
    strm.avail_in = uInt(input.size());
    strm.next_out = reinterpret_cast<Bytef *>(tmpBuf.data());
    strm.avail_out = BUFSIZE;

    // windowBits must be greater than or equal to the windowBits value provided to deflateInit2() while compressing
    // Add 32 to windowBits to enable zlib and gzip decoding with automatic header detection
    int result = inflateInit2(&strm, (15 + 32));
    if (result != Z_OK)
        return false;

    // from lzbench, level 9 average compression ratio is: 31.92%, which decompression ratio is: 1 / 0.3192 = 3.13
    output.reserve(input.size() * 3);

    // run inflate
    while (true)
    {
        result = inflate(&strm, Z_NO_FLUSH);

        if (result == Z_STREAM_END)
        {
            output.append(tmpBuf.data(), (BUFSIZE - strm.avail_out));
            break;
        }

        if (result != Z_OK)
        {
            inflateEnd(&strm);
            return false;
        }

        output.append(tmpBuf.data(), (BUFSIZE - strm.avail_out));
        strm.next_out = reinterpret_cast<Bytef *>(tmpBuf.data());
        strm.avail_out = BUFSIZE;
    }

    inflateEnd(&strm);

    return true;
}
