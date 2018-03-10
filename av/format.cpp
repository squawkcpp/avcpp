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
#include "format.h"

#include <mutex>

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avstring.h"
}

#include "_iocontext.h"
#include "averrc.h"

namespace av {

//int nb_channels( const ChannelLayout::Enum ch_layout ) {return 0;}
//TODO
//int nb_channels( const ChannelLayout::Enum ch_layout ) {
//    //return av_get_channel_layout_nb_channels( ChannelLayout::get( ch_layout ) );
//    return 0;
//}

static std::once_flag _register_flag;
static bool av_init ( LOG_LEVEL log = LOG_LEVEL::QUIET ) {
    std::call_once ( _register_flag, [&log]() {
        av_register_all ();
        av_log_set_flags ( AV_LOG_SKIP_REPEATED );
        av_log_set_level ( static_cast< int > ( log ) );
    } );
    return true;
}
static bool codecs_loaded = av_init ( LOG_LEVEL::QUIET );

Format::Format ( const std::string& filename, Mode mode, Options options ) {

    if ( mode == Mode::WRITE ) {
        //open file for writing
        int error;

        //Open the output file to write to it.
        AVIOContext* _output_io_context = nullptr;

        if ( ( error = avio_open ( &_output_io_context, filename.c_str(), AVIO_FLAG_WRITE ) ) < 0 )
        { errc_ = make_error_code ( error ); return; }

        if ( ! ( format_context_  = avformat_alloc_context() ) )
        { errc_ = make_error_code ( error ); return; }

        //Associate the output file (pointer) with the container format context.
        format_context_->pb = _output_io_context ;

        //Guess the desired container format based on the file extension.
        if ( ! ( format_context_ ->oformat = av_guess_format ( NULL/*short name*/, filename.c_str(), NULL/*mime_type*/ ) ) )
        { errc_ = make_error_code ( AV_ENCODER_NOT_FOUND ); return; }

        av_strlcpy ( format_context_->filename, filename.c_str(), sizeof ( format_context_->filename ) );

    } else {
        //open file for reading
        int error_;

        //Open the input file to read from it.
        if ( ( error_ = avformat_open_input ( &format_context_, filename.c_str(), NULL, NULL ) ) < 0 ) {
            format_context_ = NULL;
            errc_ = make_error_code ( error_ );
            return;
        }

        //Get information on the input file (number of streams etc.).
        if ( ( error_ = avformat_find_stream_info ( format_context_, NULL ) ) < 0 ) {
            errc_ = make_error_code ( error_ );
            return;
        }

        //load the codecs
        codecs_.clear();

        for ( unsigned short i=0; i<format_context_->nb_streams; ++i ) {
            auto _codec = std::shared_ptr< Codec > ( new Codec ( format_context_, i ) );

            if ( !*_codec )
            { codecs_.push_back ( _codec ); }

            else { errc_ = make_error_code ( AV_DECODER_NOT_FOUND ); }
        }
    }
}
Format::Format ( std::iostream* stream, Mode mode, Options options ) : io_context_ ( std::make_unique< IoContext >() ) {

    if ( mode == Mode::WRITE ) {

        //TODO STREAM NOT SET
        //open stream for writing
        format_context_ = avformat_alloc_context();
        format_context_->pb = io_context_->av_io_context_;
        format_context_->flags |= AVFMT_FLAG_CUSTOM_IO;

        AVOutputFormat* of = av_guess_format ( "flac", NULL, NULL ); //TODO

        if ( of == NULL )
        { errc_ = make_error_code ( AV_ENCODER_NOT_FOUND ); return; }

        format_context_->oformat = of;

        if ( of->audio_codec == AV_CODEC_ID_NONE )
        { errc_ = make_error_code ( AV_ENCODER_NOT_FOUND ); return; }

        io_context_->init_output_format_context ( format_context_ );

    } else {
        //open stream for reading
        format_context_ = avformat_alloc_context();
        io_context_->init_input_format_context ( format_context_, stream );

        int _error = 0;

        if ( ( _error = avformat_open_input ( &format_context_, "", nullptr, nullptr ) ) != 0 )
        { errc_ = make_error_code ( _error ); return; };

        if ( ( _error = avformat_find_stream_info ( format_context_, nullptr ) ) < 0 )
        { errc_ = make_error_code ( _error ); return; };

        //load the codecs
        codecs_.clear();

        for ( unsigned short i=0; i<format_context_->nb_streams; ++i ) {
            auto _codec = std::shared_ptr< Codec > ( new Codec ( format_context_, i ) );

            if ( !*_codec )
            { codecs_.push_back ( _codec ); }

            else { errc_ = make_error_code ( AV_DECODER_NOT_FOUND ); }
        }
    }
}

Format::~Format() {
    //free ressources
//TODO    if( format_context_ ) {
//        avio_closep(&format_context_->pb);
//        avformat_free_context(format_context_);
//        format_context_ = nullptr;
//    }
}

const Format::iterator Format::begin()
{ return codecs_.begin(); }
const Format::iterator Format::end()
{ return codecs_.end(); }
const std::shared_ptr< Codec > Format::at ( const std::size_t& position ) const
{ return codecs_.at ( position ); }
std::size_t Format::size() const
{ return codecs_.size(); }

av::Metadata Format::metadata() const {
    av::Metadata _metadata;
    AVDictionaryEntry *tag = NULL;

    tag = av_dict_get ( format_context_->metadata, "title", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "album", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "artist", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "track", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "composer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "performer", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "comment", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "date", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( "year", tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "year", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "disc", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "genre", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    tag = av_dict_get ( format_context_->metadata, "publisher", nullptr, AV_DICT_IGNORE_SUFFIX );

    if ( tag ) { _metadata.set ( tag->key, tag->value ); }

    return _metadata;
}

uint64_t Format::playtime() const {
    uint64_t _millis = 0;

    if ( format_context_->duration != AV_NOPTS_VALUE ) {
        uint64_t duration = format_context_->duration +
                            ( format_context_->duration <= INT64_MAX - 5000 ? 5000 : 0 );
        _millis = duration / AV_TIME_BASE;
    }

    return _millis;
}

std::error_code Format::read ( std::function< void ( Packet& ) > callback ) {
    Packet _package;
    int _err = 0;

    while ( true ) {
        if ( ( _err  = av_read_frame ( format_context_, _package.packet_ ) ) < 0 ) {
            return make_error_code ( _err );

        } else if ( _err != 0 ) {
            return make_error_code ( _err );
        }

        callback ( _package );
    }

    //flush the codec
    _package.packet_->data = NULL;
    _package.packet_->size = 0;
    callback ( _package );

    return make_error_code ( _err );
}

std::error_code Format::write ( Packet& packet ) {

    if( !_header_written ) {
        _header_written = true;
        int error;
        if( ( error = avformat_write_header( format_context_, NULL ) ) < 0 )
        {return make_error_code( error );}
    }
    int error;

    if ( ( error = av_write_frame ( format_context_, packet.packet_ ) ) < 0 )
    { return make_error_code ( error ); }
    av_packet_unref ( packet.packet_ );

    return std::error_code();
}

bool Format::operator!() const
{ return !errc_ ; }
bool Format::good()
{ return errc_.value() == 0; }
bool Format::eof()
{ return errc_.value() == AV_EOF; }
bool Format::fail()
{ return !errc_ && errc_.value() != AV_EOF; }
std::error_code Format::errc ()
{ return errc_; }
}//namespace av
