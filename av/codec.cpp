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
#include "codec.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/imgutils.h"
}

#include "averrc.h"
#include "packet.h"

namespace av {

inline CODEC::Enum __codec ( AVCodecID codec_id ) {
    switch ( codec_id ) {
    case AV_CODEC_ID_MP3:
        return CODEC::MP3;

    case AV_CODEC_ID_FLAC:
        return CODEC::FLAC;

    case AV_CODEC_ID_AAC:
        return CODEC::AAC;

    case AV_CODEC_ID_VORBIS:
        return CODEC::VORBIS;

    default:
        return CODEC::NONE;
    }
}
inline AVCodecID __codec ( CODEC::Enum codec ) {
    switch ( codec ) {
    case CODEC::AAC:
        return AV_CODEC_ID_AAC;

    case CODEC::FLAC:
        return AV_CODEC_ID_FLAC;

    case CODEC::MP2:
        return AV_CODEC_ID_MP2;

    case CODEC::MP3:
        return AV_CODEC_ID_MP3;

    case CODEC::VORBIS:
        return AV_CODEC_ID_VORBIS;

    case CODEC::NONE:
        break;
    }

    return AV_CODEC_ID_NONE;
}
inline CODEC_TYPE::Enum __codec ( AVMediaType codec_type ) {
    switch ( codec_type ) {
    case AVMEDIA_TYPE_VIDEO:
        return CODEC_TYPE::VIDEO;

    case AVMEDIA_TYPE_AUDIO:
        return CODEC_TYPE::AUDIO;

    case AVMEDIA_TYPE_DATA:
        return CODEC_TYPE::DATA;

    default:
        return CODEC_TYPE::NONE;
    }
}

std::array< std::string, 5 > Codec::codec_names_ {{"mp2", "mp3", "flac", "aac", "vorbis"} }; //TODO
std::string Codec::name ( CODEC::Enum codec ) {
    if ( codec == CODEC::NONE ) { return "none"; }

    else { return codec_names_[ static_cast< size_t > ( codec )]; }
}
std::array< std::string, 6 > Codec::codec_type_names_ {{"VIDEO", "AUDIO", "DATA", "SUBTITLE", "ATTACHEMENT", "NB"}};
std::string Codec::name ( CODEC_TYPE::Enum codec ) {
    if ( codec == CODEC_TYPE::NONE )
    { return "none"; }

    else
    { return codec_type_names_[ static_cast< size_t > ( codec )]; }
}

Codec::Codec ( AVFormatContext* format_context, const int index ) : index_ ( index ) {

    AVCodec *input_codec;

    //Find a decoder for the stream.
    if ( ! ( input_codec = avcodec_find_decoder ( format_context->streams[index]->codecpar->codec_id ) ) ) {
        errc_ = make_error_code ( AV_DECODER_NOT_FOUND );
        avformat_close_input ( &format_context );
        return;
    }

    //allocate a new decoding context
    codec_context_ = avcodec_alloc_context3 ( input_codec );

    if ( !codec_context_ ) {
        errc_ = make_error_code ( ENOMEM );
        avformat_close_input ( &format_context );
        return;
    }

    int error_;

    //initialize the stream parameters with demuxer information
    error_ = avcodec_parameters_to_context ( codec_context_, ( format_context )->streams[index]->codecpar );

    if ( error_ < 0 ) {
        avformat_close_input ( &format_context );
        avcodec_free_context ( &codec_context_ );
        codec_context_ = nullptr;
        errc_ = make_error_code ( error_ );
        return;
    }

    //Open the decoder for the audio stream to use it later.
    if ( ( error_ = avcodec_open2 ( codec_context_, input_codec, NULL ) ) < 0 ) {
        avcodec_free_context ( &codec_context_ );
        codec_context_ = nullptr;
        avformat_close_input ( &format_context );

        errc_ = make_error_code ( error_ );
        return;
    }
}

Codec::Codec ( CODEC::Enum codec, SampleFormat sample_format, Options options ) {

    if ( !options.contains ( "ac" ) ) {
        std::cout << "channels not specified." << std::endl;
        return;
    }

    const AVCodec* _codec = avcodec_find_encoder ( __codec ( codec ) );

    if ( !_codec ) {
        errc_ = make_error_code ( AV_ENCODER_NOT_FOUND );
        return;
    }

    codec_context_ = avcodec_alloc_context3 ( _codec );

    if ( !codec_context_ ) {
        errc_ = make_error_code ( ENOMEM );
        return;
    }

    codec_context_->sample_fmt = static_cast< AVSampleFormat > ( sample_format );
    codec_context_->channel_layout = av_get_default_channel_layout ( atoi ( options.get("ac").c_str() ) );//TODO get int

    //open it
    int ret;

    if ( ( ret = avcodec_open2 ( codec_context_, _codec, options.av_options() ) ) < 0 ) {
        errc_ = make_error_code ( ret );
        return;
    }
}

Codec::~Codec() {
    if ( codec_context_ != nullptr )
    { avcodec_free_context ( &codec_context_ ); }
}

int Codec::index() const {
    return index_;
}
CODEC_TYPE::Enum Codec::codec_type() const
{ return __codec ( codec_context_->codec_type ); }
CODEC::Enum Codec::codec() const
{ return __codec ( codec_context_->codec_id ); }
int Codec::bitrate() const
{ return codec_context_->bit_rate; }
int Codec::sample_rate() const
{ return codec_context_->sample_rate; }
SampleFormat Codec::sample_fmt() const
{ return static_cast< SampleFormat > ( codec_context_->sample_fmt ); }
int Codec::channels() const
{ return codec_context_->channels; }
ChannelLayout::Enum Codec::channel_layout() const
{ return static_cast< ChannelLayout::Enum > ( codec_context_->channel_layout ); }
int Codec::bits_per_sample() const
{ return codec_context_->bits_per_raw_sample; }
int Codec::width() const
{ return codec_context_->width; }
int Codec::height() const
{ return codec_context_->height; }
int Codec::frame_size()
{ return codec_context_->frame_size; }
bool Codec::is_planar()
{ return av_sample_fmt_is_planar ( codec_context_->sample_fmt ); }

std::error_code Codec::decode ( Packet& package, std::function< void ( Frame& ) > callback ) {

    //send package to codec
    int ret;

    if ( ( ret = avcodec_send_packet ( codec_context_, package.packet_ ) ) < 0 ) {
        return make_error_code ( ret );
    }

    //receive frame from the codec
    if ( !frame_.frame_ ) {
        if ( ! ( frame_.frame_ = av_frame_alloc() ) ) {
            fprintf ( stderr, "Could not allocate audio frame\n" );
            { return make_error_code ( AV_EXIT ); }
        }
    }

    while ( ret >= 0 ) {
        ret = avcodec_receive_frame ( codec_context_, frame_.frame_ );

        if ( ret == AVERROR ( EAGAIN ) || ret == AVERROR_EOF )
        { return make_error_code ( ret ); }

        else if ( ret < 0 ) {
            fprintf ( stderr, "Error during decoding\n" );
            return make_error_code ( AV_EXIT );
        }

        frame_.data_size_ = av_get_bytes_per_sample ( codec_context_->sample_fmt );

        if ( frame_.data_size_ < 0 ) {
            //This should not occur, checking just for paranoia
            fprintf ( stderr, "Failed to calculate data size\n" );
            return make_error_code ( AV_EXIT );
        }

        callback ( frame_ );
    }

    return make_error_code ( ret );
}

std::error_code Codec::encode ( std::function< void ( Packet& ) > fn ) {

    int ret;

    if ( ( ret = avcodec_send_frame ( codec_context_, nullptr ) ) >= 0 ) {
        do {
            ret = avcodec_receive_packet ( codec_context_, package_.packet_ );

            if ( ret >= 0 ) { fn ( package_ ); }
        } while ( ret >= 0 );
    }

    return make_error_code ( ret );
}

std::error_code Codec::encode ( Frame& frame, std::function< void ( Packet& ) > fn ) {

    int ret;

    if ( ( ret = avcodec_send_frame ( codec_context_, frame.frame_ ) ) >= 0 ) {
        do {
            ret = avcodec_receive_packet ( codec_context_, package_.packet_ );

            if ( ret >= 0 ) { fn ( package_ ); }
        } while ( ret >= 0 );
    }

    return make_error_code ( ret );
}

//TODO
size_t Codec::malloc_image ( uint8_t** video_dst_data, int* video_dst_linesize ) {
    return av_image_alloc ( video_dst_data, video_dst_linesize, codec_context_->width, codec_context_->height, codec_context_->pix_fmt, 1 );

}
//TODO
void Codec::copy_image ( Frame& frame, uint8_t* video_dst_data, int video_dst_linesize ) {
    av_image_copy ( &video_dst_data, &video_dst_linesize,
                    ( const uint8_t ** ) ( frame.frame_->data ), frame.frame_->linesize,
                    codec_context_->pix_fmt, codec_context_->width, codec_context_->height );
}

bool Codec::operator!() const
{ return !errc_ ; }
bool Codec::good()
{ return errc_.value() == 0; }
bool Codec::eof()
{ return errc_.value() == AV_EOF; }
bool Codec::fail()
{ return !errc_ && errc_.value() != AV_EOF; }
std::error_code Codec::errc ()
{ return errc_; }

}//namespace av
