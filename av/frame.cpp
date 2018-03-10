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
#include "frame.h"

extern "C" {
#include "libavutil/frame.h"
}

#include "averrc.h"

namespace av {
Frame::Frame ( int frame_size, int sample_fmt, int channel_layout ) {
    frame_ = av_frame_alloc();

    if ( ! frame_ ) {
        fprintf ( stderr, "Could not allocate audio data buffers\n" );
        exit ( 1 );
    }

    frame_->nb_samples     = frame_size;
    frame_->format         = sample_fmt;
    frame_->channel_layout = channel_layout;
    frame_->sample_rate    = 44100; //TODO output_codec_context->sample_rate;

    /* allocate the data buffers */
    int ret = av_frame_get_buffer ( frame_, 0 );

    if ( ret < 0 ) {
        fprintf ( stderr, "Could not allocate audio data buffers\n" );
        exit ( 1 );
    }
}

Frame::~Frame() {
    if ( frame_ != nullptr )
    { av_frame_free ( &frame_ ); }
}

int Frame::nb_samples() const
{ return frame_->nb_samples; }
size_t Frame::data_size() const
{ return data_size_; }
uint8_t* Frame::data ( int index ) {
    return frame_->data[index];
}
uint8_t** Frame::extended_data () {
    return frame_->extended_data;
}
int Frame::linesize ( int index ) {
    return frame_->linesize[index];
}
std::error_code Frame::writeable() {
    int ret = av_frame_make_writable ( frame_ );

    if ( ret < 0 ) {
        return make_error_code ( ret );
    }

    return std::error_code();
}
}//namespace av
