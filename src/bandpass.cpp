#include "bandpass.h"
#include <math.h>
#include "GraphUtils.h"

#include <stdio.h>
#include <string.h>

#define SampleRate 44100
#define Freq 15000
#define Freqrange 100

namespace dsp
{



Bandpass::Bandpass(unsigned int signalSize):_signalSize(signalSize)
{
    _data1        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _data2        = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    //  memset(_data1,0,sizeof( fftw_complex ) * signalSize);
    //  memset(_data2,0,sizeof( fftw_complex ) * signalSize);

    _fft_result1  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _fft_result2  = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _ifft_result1 = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );
    _ifft_result2 = ( fftw_complex* ) fftw_malloc( sizeof( fftw_complex ) * signalSize );

    _plan_forward1  = fftw_plan_dft_1d( signalSize, _data1, _fft_result1, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_forward2  = fftw_plan_dft_1d( signalSize, _data2, _fft_result2, FFTW_FORWARD, FFTW_ESTIMATE );
    _plan_backward1 = fftw_plan_dft_1d( signalSize, _fft_result1, _ifft_result1, FFTW_BACKWARD, FFTW_ESTIMATE );
    _plan_backward2 = fftw_plan_dft_1d( signalSize, _fft_result2, _ifft_result2, FFTW_BACKWARD, FFTW_ESTIMATE );
}

Bandpass::~Bandpass()
{
    fftw_destroy_plan( _plan_forward1 );
    fftw_destroy_plan( _plan_forward2 );
    fftw_destroy_plan( _plan_backward1 );
    fftw_destroy_plan( _plan_backward2 );
 
    fftw_free( _data1 );
    fftw_free( _fft_result1 );
    fftw_free( _data2 );
    fftw_free( _fft_result2 );
    fftw_free( _ifft_result1 );
    fftw_free( _ifft_result2 );
}

Bandpass::returnS Bandpass::calculate(const fftw_data_type *ref,const fftw_data_type *sig)
{
    double na=0;
    // not optimmal but well, will maybe optimiezed in the future..
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data1[i][0] = ref[i];
        _data1[i][1] = 0.0;
        na += ref[i] *ref[i];
    }
    double nb=0;
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        _data2[i][0] = sig[i];
        _data2[i][1] = 0.0;
         nb += sig[i] *sig[i];
    }
    
    fftw_execute( _plan_forward1 );
    fftw_execute( _plan_forward2);

    int chunk_size = (SampleRate/2) / _signalSize;
    
    for(unsigned int i = 0 ; i < _signalSize; i++ )
    {
        if ((i + 1) * chunk_size < Freq - Freqrange && (i + 1) * chunk_size > Freq + Freqrange )
        _fft_result1[i][0] = 0.0;
        _fft_result1[i][1] = 0.0;
        
        _fft_result2[i][0] = 0.0;
        _fft_result2[i][1] = 0.0;
    }

    fftw_execute( _plan_backward1 );
    fftw_execute( _plan_backward2 );
    

    returnS ret;
    memcpy(ret._ifft_result1, _ifft_result1, _signalSize);
    memcpy(ret._ifft_result2, _ifft_result2, _signalSize);

    return ret;
}

}