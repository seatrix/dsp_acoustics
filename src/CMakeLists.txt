
rock_library(dsp_acoustics
    SOURCES empty.cpp xcorr.cpp writeWave.cpp GraphUtils.cpp bandpass.cpp normalizer.cpp
    HEADERS FitPolynom.h FIRFilter.h xcorr.h zcr.h writeWave.h GraphUtils.h ExtractTarget.h SonarBeamToSonarScan.h CircularBuffer.h bandpass.h normalizer.h
    DEPS_PKGCONFIG fftw3f opencv gsl #fftw3f fftw3
    DEPS_PKGCONFIG sndfile
    )


