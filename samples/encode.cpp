//#include <iostream>

//#include <cmath>

//#include "../av/format.h"
//#include "../av/frame.h"
//#include "../av/package.h"

//int main(int argc, char* argv[]) {

//    if (argc <= 1) {
//        fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
//        exit(0);
//    }

//    FILE* outfile;
//    outfile = fopen( argv[1], "wb" );
//    if (!outfile) {
//        exit(1);
//    }

//    std::error_code errc;
//    av::Codec _codec( av::CODEC::MP2 );

//    std::cout << "Encode: " << _codec << std::endl;

//    av::Package _package;
//    av::Frame _frame( _codec.frame_size(), _codec.sample_fmt(), _codec.channel_layout() );

//    /* encode a single tone sound */
//    float t, tincr;
//    uint16_t *samples;

//    t = 0;
//    tincr = 2 * 3.14159265358979323846 * 440.0 / _codec.sample_rate();
//    for( int i = 0; i < 200; i++ ) {

//        if( !! ( errc = _frame.writeable() ) ) {
//            std::cerr << errc.message() << std::endl;
//        }

//        samples = (uint16_t*)_frame.data(0);

//        for (int j = 0; j < _codec.frame_size(); j++) {
//            samples[2*j] = (int)(sin(t) * 10000);

//            for (int k = 1; k < _codec.channels(); k++)
//                samples[2*j + k] = samples[2*j];
//            t += tincr;
//        }
//        while( ! ( errc = _codec.encode( _frame, _package ) ) ) {
//            fwrite(_package.data(), 1, _package.size(), outfile);
//            _package.unref();
//        }
//    }
//    /* flush the encoder */
//    while( ! ( errc = _codec.encode( _package ) ) ) {
//        fwrite(_package.data(), 1, _package.size(), outfile);
//        _package.unref();
//    }
//    std::cout << errc.message() << std::endl;
//    fclose(outfile);
//    return 0;
//}
