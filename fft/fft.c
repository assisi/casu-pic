#include <math.h>
#include "../pic-to-beaglebone/pic2beaglebone.h"
#include "fft.h"


int FastFourierTransform(int *source_array,
                         int *amplitudes,
                         fractcomplex *destination_array,
                         fractcomplex *src_array,
                         fractcomplex *twiddle_P,
                         float *freq,
                         float *amp)
{
    int k = 0;
    float amp_temp = 0, freq_temp = 0;

    for (k = 0; k <= FFT_BUFF - 1; k++){
        /*
        if( *(source_array + k) >= 0)
             (*(src_array + k)).real =  (fractional) (((*(source_array + k)) << (16 - ACC_RES)  ) >> 1 );
        else
             (*(src_array + k)).real = (fractional) (((((*(source_array + k)) << (16 - ACC_RES)  ) >> 1 )) | 0x8000);
        */
        (*(src_array + k)).real =  (fractional) ((*(source_array + k)) << (16 - ACC_RES - 1));
        (*(src_array + k)).imag     = (fractional) 0x0000;

    }
    
    FFTComplexIP(LOG2_FFT_BUFF, src_array, twiddle_P, COEFFS_IN_DATA);

    BitReverseComplex (LOG2_FFT_BUFF, src_array);

    // compute amplitudes of the first half of spectrum (the second half is symmetrical
    for (k = 0; k < FFT_BUFF / 2; k++) {
        
        if ((*(src_array + k)).real < 0)
            (*(destination_array + k)).real = (((( (int) (*(src_array + k)).real) << 1) >> (16 - ACC_RES)) | 0xFC00);
        else
            (*(destination_array + k)).real =  ((( (int) (*(src_array + k)).real) << 1) >> (16 - ACC_RES));
        
        if ((*(src_array + k)).imag < 0)
            (*(destination_array + k)).imag = (((( (int) (*(src_array + k)).imag) << 1) >> (16 - ACC_RES)) | 0xFC00);
        else
            (*(destination_array + k)).imag =  ((( (int)(*(src_array + k)).imag) << 1) >> (16 - ACC_RES));
         
    }

    for (k = 0; k < FFT_BUFF / 2; k++){
        (*(amplitudes + k)) = 2 * ((int)sqrt((float)(pow(((int)(*(destination_array + k)).real), 2.0)+
                                                     pow(((int)(*(destination_array + k)).imag), 2.0))));
        if (k == 0) *amplitudes = (*amplitudes)/2;
        if ((*(amplitudes + k)) > amp_temp){
            amp_temp = *(amplitudes + k);
            freq_temp = k;
        }
    }
    *amp = amp_temp * 4; //in mg (mili g)
    *freq = freq_temp * 2000.0 / 256; // 2000 hz rate and 256 samples

    return 1;
}