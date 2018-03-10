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
#ifndef RESAMPLE_H
#define RESAMPLE_H

#include <system_error>

///@cond DOC_INTERNAL
struct SwrContext;
struct AVAudioFifo;
///@endcond DOC_INTERNAL

#include "format.h"
#include "option.h"

namespace av {

uint8_t** make_sample_buffer ( ChannelLayout::Enum channel_layout, int nb_samples, SampleFormat sample_format, int* linesize );

//static int init_converted_samples(uint8_t ***converted_input_samples,
//                                  AVCodecContext *output_codec_context,
//                                  int frame_size);

class Resample {
public:

    Resample ( ChannelLayout::Enum source_channels, SampleFormat source_sample_fmt, int source_sample_rate,
               ChannelLayout::Enum target_channels, SampleFormat target_sample_fmt, int target_sample_rate );
    ~Resample();

    std::error_code resample ( uint8_t **src_data, int* src_linesize, std::function< void ( uint8_t**, const int ) > fn );
    std::error_code resample ( Frame& frame, int frame_size, std::function< void ( Frame& ) > fn );

    int dst_buffer_size();

    /**
     * @brief checks if an error has occurred.
     * Returns true if an error has occurred on the associated format context.
     * @return true if an error has occurred, false otherwise.
     */
    bool operator!() const;
    /** @brief checks if no error has occurred i.e. I/O operations are available */
    bool good();
    /** @brief checks if end-of-file has been reached */
    bool eof();
    /** @brief checks if an error has occurred */
    bool fail();
    /**
     * @brief return the actual error state of the assiciated format context.
     * @return return the error state as std::error_code.
     */
    std::error_code errc ();

private:
    std::error_code errc_;

    const ChannelLayout::Enum source_channels_;
    const ChannelLayout::Enum target_channels_;
    const SampleFormat source_sample_format_;
    const SampleFormat target_sample_format_;
    const int source_sample_rate_;
    const int target_sample_rate_;

    SwrContext* resample_context_ = nullptr;
    AVAudioFifo* fifo_ = nullptr;

    int src_nb_samples_ = 1024; //TODO
    int dst_nb_samples = 0, max_dst_nb_samples = 0;
    int dst_bufsize_ = 0;
    int dst_nb_channels = 0;

    int dst_linesize = 0;
    uint8_t **dst_data = nullptr;
};
}// namespace av

#endif // RESAMPLE_H
