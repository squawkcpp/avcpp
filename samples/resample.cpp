#include <iostream>

#include <cmath>

#include "../av/format.h"
#include "../av/resample.h"

/**
 * Fill dst buffer with nb_samples, generated starting from t.
 */ //TODO CLEANUP
static void fill_samples(double *dst, int nb_samples, int nb_channels, int sample_rate, double *t)
{
    int i, j;
    double tincr = 1.0 / sample_rate;
    double *dstp = dst;
    const double c = 2 * M_PI * 440.0;

    /* generate sin tone with 440Hz frequency and duplicated channels */
    for (i = 0; i < nb_samples; i++) {
        *dstp = sin(c * *t);
        for (j = 1; j < nb_channels; j++)
            dstp[j] = dstp[0];
        dstp += nb_channels;
        *t += tincr;
    }
}

int main(int argc, char* argv[]) {

    if (argc <= 1) {
        fprintf(stderr, "Usage: %s <output file>\n", argv[0]);
        exit(0);
    }

    FILE* outfile;
    outfile = fopen( argv[1], "wb" );
    if (!outfile) {
        exit(1);
    }

//    uint8_t /***src_data = NULL, **dst_data = NULL;
    int src_rate = 48000, dst_rate = 44100;
    av::ChannelLayout::Enum src_nb_channels = av::ChannelLayout::CH_LAYOUT_STEREO, dst_nb_channels = av::ChannelLayout::CH_LAYOUT_SURROUND;
    int src_linesize = 0; //, dst_linesize = 0;
    int src_nb_samples = 1024; //, dst_nb_samples; //, max_dst_nb_samples;
    //enum av::SampleFormat src_sample_fmt = av::SampleFormat::SAMPLE_FMT_DBL, dst_sample_fmt = av::SampleFormat::SAMPLE_FMT_S16;

    av::Resample resample( src_nb_channels, av::SampleFormat::SAMPLE_FMT_DBL, src_rate,
                           dst_nb_channels, av::SampleFormat::SAMPLE_FMT_S16, dst_rate);

    if( !!resample ) {
        std::cerr << resample.errc().message();
        return 1;
    }

    uint8_t** src_data = av::make_sample_buffer( src_nb_channels, src_nb_samples, av::SampleFormat::SAMPLE_FMT_DBL, &src_linesize );
//    auto dst_data = av::make_sample_buffer( dst_nb_channels, dst_nb_samples, av::SampleFormat::SAMPLE_FMT_S16, &dst_linesize );
    //nullptr; // = av::make_sample_buffer( dst_nb_channels, dst_nb_samples, av::SampleFormat::SAMPLE_FMT_S16 );

    double t = 0;
    do {
        /* generate synthetic audio */
        fill_samples((double *)src_data[0], src_nb_samples, 2 /*src_nb_channels*/, src_rate, &t);
        std::error_code ret = resample.resample( (uint8_t**)src_data, &src_nb_samples, [&](uint8_t** dst_data, const int buffer_size ) {

            printf("t:%f in:%d out:%d\n", t, src_nb_samples, buffer_size );
            fwrite( dst_data, 1, buffer_size, outfile );
        } );
        if( !!ret ) {
            std::cout << "error resampling " << ret.message() << std::endl;
        }
    } while (t < 10);

//    if ((ret = get_format_from_sample_fmt(&fmt, dst_sample_fmt)) < 0)
//        goto end;
//    fprintf(stderr, "Resampling succeeded. Play the output file with the command:\nffplay -f %s -channel_layout %"PRId64" -channels %d -ar %d %s\n",
//            fmt, dst_ch_layout, dst_nb_channels, dst_rate, dst_filename);
}
