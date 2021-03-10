//
//  DA278A, L4
//  config.h
//
//  CJ Gribel 2018-12-06.
//

#ifndef config_h
#define config_h

/*
 Source and destination PNG paths
 */
#define SRCFILE ("tronlegacy.png")
#define DESTFILE ("tronsobel.png")

/*
 Threading strategy
 -BRUTEFORCE: Call the filter function directly without using a thread pool
 -THREADPOOL: Use thread pool with one job per tile
 */
//#define BRUTEFORCE
#define THREADPOOL

/*
 Thread count
 */
#define NBRTHREADS_AUTO
//#define NBRTHREADS 64

/*
 Tile size
 */
#define TILESIZE 32

/*
 Filter type
 -INVERT: Invert colors component-wise.
 -GAUSS BLUR: Convolve an area around each pixel with a Gauss kernel.
 -GRAYSCALE: Set each color component to G(RGB).
 -SOBEL (edge detection): Convolve an area around the pixel with a Sobel kernel.
 */
//#define FILTER_GRAYSCALE
//#define FILTER_INVERT
//#define FILTER_BLUR
#define FILTER_SOBEL

/*
 Gauss kernel size (applies only to FILTER_BLUR)
 Larger kernels blur more and are more expensive.
 */
#define GAUSS_KERNEL_4
//#define GAUSS_KERNEL_12

/*
 Sobel cut-off intensity, range 0-255 (applies only to FILTER_SOBEL).
 Higher accentuates edges more (discriminates smaller gradients).
 */
#define SOBEL_THRESHOLD 70

#endif /* config_h */
