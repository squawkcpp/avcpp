/*
    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/

#include <cmath>
#include <string>
#include <fstream>
#include <map>

#include "../av/codec.h"
#include "../av/format.h"
#include "../av/metadata.h"
#include "../av/packet.h"
#include "../av/resample.h"

#include <gtest/gtest.h>

namespace av {

int compareFileResample(FILE* f1, FILE* f2) {
  int N = 10000;
  char buf1[N];
  char buf2[N];

  do {
    size_t r1 = fread(buf1, 1, N, f1);
    size_t r2 = fread(buf2, 1, N, f2);

    if (r1 != r2 ||
        memcmp(buf1, buf2, r1)) {
      return 0;  // Files are not equal
    }
  } while (!feof(f1) && !feof(f2));

  return feof(f1) && feof(f2);
}

static const char* FILE_FLAC = TESTFILES "BIS1536-001-flac_16.flac";

/**
 * Fill dst buffer with nb_samples, generated starting from t.
 */
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

TEST( ResampleTest, resample ) {

    FILE* outfile;
    outfile = fopen( "/tmp/resample.out", "wb" );
    if (!outfile) {
        exit(1);
    }

    int src_nb_samples = 1024;
    const int src_rate = 48000;
    const int dst_rate = 44100;
    const av::ChannelLayout::Enum src_channel_layout = av::ChannelLayout::CH_LAYOUT_STEREO;
    const av::ChannelLayout::Enum dst_channel_layout = av::ChannelLayout::CH_LAYOUT_SURROUND;
    int src_nb_channels = 2; //TODO nb_channels( src_channel_layout );
    int src_linesize = 0;

    av::Resample resample( src_channel_layout, av::SampleFormat::SAMPLE_FMT_DBL, src_rate,
                           dst_channel_layout, av::SampleFormat::SAMPLE_FMT_S16, dst_rate );
    ASSERT_TRUE( !resample );

    uint8_t** src_data = av::make_sample_buffer(
                src_channel_layout, src_nb_samples,
                av::SampleFormat::SAMPLE_FMT_DBL, &src_linesize );
    EXPECT_EQ( 16384, src_linesize );

    double t = 0;
    do {
        /* generate synthetic audio */
        fill_samples( (double *)src_data[0], src_nb_samples, src_nb_channels, src_rate, &t );
        std::error_code ret = resample.resample( (uint8_t**)src_data, &src_nb_samples, [&](uint8_t** dst_data, const int buffer_size ) {

            printf("t:%f in:%d out:%d\n", t, src_nb_samples, buffer_size );
            fwrite( dst_data[0], 1, buffer_size, outfile );
        } );
        ASSERT_TRUE( !ret );
    } while (t < 10);
    fclose(outfile);

    //compare files
    FILE* orig_file;
    orig_file = fopen( TESTFILES "/samples/resample.out", "rb" );

    FILE* new_file;
    new_file = fopen( "/tmp/resample.out", "rb" );

    EXPECT_TRUE( compareFileResample( orig_file, new_file ) );
}
}//namespace av
