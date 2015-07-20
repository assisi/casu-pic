/*! \file fft.h
 * \brief Declaration of functions and variables used for Fast Fourier Transform (FFT) computation.
 */

#ifndef FFT_H
#define	FFT_H

#include<dsp.h>
#include<math.h>

/*! Size of the buffer used for FFT. Also the number of samples used for FFT algorithm.
 */
#define FFT_BUFF 256
/*! Logarithmic value of FFT_BUFF in base 2.
 */
#define LOG2_FFT_BUFF 8
/*! Resolution of accelerometer measurements in bits.
 */
#define ACC_RES 11     //resolution of accelerometer

/*! \brief Function computes Fast Fourier Transform on the array of values.
 *
 * Used for computing amplitude and frequency of vibrating CASU body. Microchip DSP library is used for computing FFT algorithm.
 * @param source_array Memory location where the first value of the array is stored.
 * @param amplitude Memory location where the first value of the amplitudes array is stored.
 * @param destination_array
 */
int FastFourierTransform(int *source_array,
                         int *amplitudes,
                         fractcomplex *destination_array,
                         fractcomplex *src_array,
                         fractcomplex *twiddle_P);

#endif	/* FFT_H */

