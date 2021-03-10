//
//  DA278A, L4
//  main.cpp
//
//  CJ Gribel 2018-11-16.
//

#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <cmath>
#include <algorithm>
#include <future>

#include "config.h"
#include "lodepng.h"
#ifndef BRUTEFORCE
#include "threadpool.h"
#endif

using uint = unsigned int;
using uchar = unsigned char;

template<typename T>
inline T clamp(const T& a, const T& min, const T& max)
{
    return std::max<T>(min, std::min<T>(max, a));
}

struct ImgDim
{
    uint width, height, channels;
};

void loadPNG(const std::string& filename,
             ImgDim& meta,
             std::vector<uchar> &buffer)
{
    std::vector<uchar> buffer_raw; //, buffer_decoded;

    std::cout << "Loading " << filename << std::endl;
    LodePNG::loadFile(buffer_raw, filename);
    LodePNG::Decoder decoder;
    decoder.decode(buffer, &buffer_raw[0], (uint)buffer_raw.size());
    
    if(decoder.hasError())
    {
        std::cout << "Error: " << decoder.getError() << std::endl;
        throw std::runtime_error("lodepng failed");
    }
    
    meta = { decoder.getWidth(), decoder.getHeight(), decoder.getChannels() };
    std::cout << "\tWidth " << meta.width;
    std::cout << ", Height " << meta.height;
    std::cout << ", Channels " << meta.channels << std::endl;
    std::cout << "\tEncoded " << (float)buffer_raw.size()/1e6 << " MB" << std::endl;
    std::cout << "\tDecoded " << (float)buffer.size()/1e6 << " MB" << std::endl;
}

void savePNG(std::vector<uchar> &buffer,
             const ImgDim& meta,
             const std::string& filename)
{
    LodePNG::encode(filename, buffer, meta.width, meta.height);
}

inline uint byteindex(uint x, uint y, uint c, const ImgDim& dim)
{
    return 4*(y*dim.width + x) + c;
};

inline uint byteindex_clamp(uint x, uint y, uint c, const ImgDim& dim)
{
    return byteindex(clamp<uint>(x, 0, dim.width-1),
                    clamp<uint>(y, 0, dim.height-1),
                    c,
                    dim);
};

// ITU-R BT.709 grayscale
inline uchar to_grayscale(uchar r, uchar g, uchar b)
{
    return uchar(float(r)*0.2126f + float(g)*.7152 + float(b)*0.0722f);
}

//
// Grayscale (free function)
//
uint GrayscaleFilter(const uchar* buffer_src,
                    uchar* buffer_dest,
                    const ImgDim& dim,
                    uint tile_x,
                    uint tile_y,
                    uint tile_width,
                    uint tile_height,
                    uint tilenbr)
{
    for(uint y = tile_y; y < tile_y + tile_height; ++y)
        for(uint x = tile_x; x < tile_x + tile_width; ++x)
        {
            uint b = byteindex(x, y, 0, dim);
            uchar col = to_grayscale(buffer_src[b+0],
                                     buffer_src[b+1],
                                     buffer_src[b+2]);
            buffer_dest[b+0] = col;
            buffer_dest[b+1] = col;
            buffer_dest[b+2] = col;
        }
    return tilenbr;
}

//
// Invert filter (lambda)
//
auto InvertFilter = [] (const uchar* buffer_src,
                     uchar* buffer_dest,
                     const ImgDim& dim,
                     uint tile_x,
                     uint tile_y,
                     uint tile_width,
                     uint tile_height,
                     uint tilenbr) -> uint
{
    for(uint y = tile_y; y < tile_y + tile_height; ++y)
        for(uint x = tile_x; x < tile_x + tile_width; ++x)
        {
            uint b = byteindex(x, y, 0, dim);
            buffer_dest[b+0] = 0xff - buffer_src[b+0];
            buffer_dest[b+1] = 0xff - buffer_src[b+1];
            buffer_dest[b+2] = 0xff - buffer_src[b+2];
        }
    return tilenbr;
};

//
// Gauss blur filter (functor)
//
class BlurFilter
{
public:
    uint operator() (const uchar* buffer_src,
                     uchar* buffer_dest,
                     const ImgDim& dim,
                     uint tile_x,
                     uint tile_y,
                     uint tile_width,
                     uint tile_height,
                     uint tilenbr) const
    {
        for(uint y = tile_y; y < tile_y + tile_height; ++y)
            for(uint x = tile_x; x < tile_x + tile_width; ++x)
                for (int c = 0; c < 3; ++c)
                {
                    float col = 0;
                    for (int j = -KR; j <= KR; ++j)
                        for (int i = -KR; i <= KR; ++i)
                        {
                            uint b = byteindex_clamp(x+i, y+j, c, dim);
                            col += (float)buffer_src[b] * KERNEL[i+KR] * KERNEL[j+KR];
                        }
                    buffer_dest[ byteindex(x, y, c, dim) ] = (uchar)col;
                }
        return tilenbr;
    }
    
private:
    // Kernel radius and discretized weights in one dimension
#ifdef GAUSS_KERNEL_4
    const int KR = 4;
    const float KERNEL[9] = {
        0.00481007202f,
        0.0286864862f,
        0.102712765f,
        0.220796734f,
        0.284958780f,
        0.220796734f,
        0.102712765f,
        0.0286864862f,
        0.00481007202f };
#endif
#ifdef GAUSS_KERNEL_12
    const int KR = 12;
    const float KERNEL[25] = {
        0.00296902,
        0.0133062,
        0.0219382,
        0.0133062,
        0.00296902,
        0.0133062,
        0.0596343,
        0.0983203,
        0.0596343,
        0.0133062,
        0.0219382,
        0.0983203,
        0.162103,
        0.0983203,
        0.0219382,
        0.0133062,
        0.0596343,
        0.0983203,
        0.0596343,
        0.0133062,
        0.00296902,
        0.0133062,
        0.0219382,
        0.0133062,
        0.00296902 };
#endif
};

//
// Sobel filter (functor)
//
class SobelFilter
{
public:
    uint operator() (const uchar* buffer_src,
                     uchar* buffer_dest,
                     const ImgDim& dim,
                     uint tile_x,
                     uint tile_y,
                     uint tile_width,
                     uint tile_height,
                     uint tilenbr) const
    {
        for(uint y = tile_y; y < tile_y + tile_height; ++y)
            for(uint x = tile_x; x < tile_x + tile_width; ++x)
            {
                // Kernel
                float xcol = 0, ycol = 0;
                for (int j = -1; j <= 1; ++j)
                    for (int i = -1; i <= 1; ++i)
                    {
                        // Transform to grayscale: inefficient to do within the
                        // kernel; ideally the image is preprocessed with a
                        // grayscale filter.
                        uint b = byteindex_clamp(x+i, y+j, 0, dim);
                        uchar col = to_grayscale(buffer_src[b+0],
                                                 buffer_src[b+1],
                                                 buffer_src[b+2]);
                        // Pick red channel (assume grayscale)
                        //uchar col = buffer_src[byteindex_clamp(x+j, y+j, 0, dim)];
                        
                        xcol += col * Gx[(j+1)*3+(i+1)];
                        ycol += col * Gy[(j+1)*3+(i+1)];
                    }
                float norm = sqrtf(xcol*xcol + ycol*ycol);
                uchar col = uchar( norm < SOBEL_THRESHOLD ? 0.0f : norm );
                
                uint b = byteindex(x, y, 0, dim);
                buffer_dest[b+0] = col;
                buffer_dest[b+1] = col;
                buffer_dest[b+2] = col;
            }
        return tilenbr;
    }
    
private:
    // Sobel-Feldman horizontal and vertical kernels
    const float Gx[9] = { -1, 0, 1, -2, 0, 2, -1, 0, 1 };
    const float Gy[9] = { -1, -2, -1, 0, 0, 0, 1, 2, 1 };
};

int main(int argc, const char * argv[])
{
    std::cout << "Supported (hyper)threads on current CPU: " << std::thread::hardware_concurrency() << std::endl;
    
    ImgDim imgdim;
    std::vector<uchar> buffer_src;
    loadPNG(SRCFILE, imgdim, buffer_src);
    std::vector<uchar> buffer_dest(buffer_src.size(), 255);
    
    // Create thread pool and result vector
#ifdef THREADPOOL
#ifdef NBRTHREADS_AUTO
    Threadpool tp(std::thread::hardware_concurrency());
#else
    Threadpool tp(NBRTHREADS);
#endif
    std::vector< std::future<uint> > results;
#endif

    // Assign filter callables
#ifdef FILTER_GRAYSCALE
    auto tile_filter_callable = std::ref(GrayscaleFilter);
#endif
#ifdef FILTER_INVERT
    auto tile_filter_callable = std::ref(InvertFilter);
#endif
#ifdef FILTER_BLUR
    BlurFilter blurFilter;
    auto tile_filter_callable = std::ref(blurFilter);
#endif
#ifdef FILTER_SOBEL
    SobelFilter sobelFilter;
    auto tile_filter_callable = std::ref(sobelFilter);
#endif
    
    // Start timings
    std::cout << "Applyng filter ..." << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
#ifdef BRUTEFORCE
    tile_filter_callable(&buffer_src[0],
                         &buffer_dest[0],
                         imgdim,
                         0,
                         0,
                         imgdim.width,
                         imgdim.height,
                         0);
#endif
    
#ifdef THREADPOOL
    int nbrtiles = 0;
    for (uint i = 0; i < imgdim.width; i += TILESIZE)
        for (uint j = 0; j < imgdim.height; j += TILESIZE)
        {
            ++nbrtiles;
            uint tile_x = i;
            uint tile_y = j;
            uint tile_width = std::min<uint>(i + TILESIZE, imgdim.width) - i;
            uint tile_height = std::min<uint>(j + TILESIZE, imgdim.height) - j;

            results.emplace_back(tp.enqueue(tile_filter_callable,
                                            &buffer_src[0],
                                            &buffer_dest[0],
                                            imgdim,
                                            tile_x,
                                            tile_y,
                                            tile_width,
                                            tile_height,
                                            nbrtiles)
                                 );
        }
#endif
    
#ifdef THREADPOOL
    std::cout << "Waiting for jobs (tiles) to finish ..." << std::endl;
    for(auto && result: results)
        std::cout << result.get() << ' ';
    std::cout << std::endl;
#endif
    
    // Show timings
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = end - start;
    std::cout << std::chrono::duration <double, std::milli> (diff).count() << " ms" << std::endl;
    
    // Encode the filtered PNG
    savePNG(buffer_dest, imgdim, DESTFILE);
    std::cout << "Wrote " << DESTFILE << std::endl;
    
    return 0;
}
