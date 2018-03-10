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
#ifndef CODEC_H
#define CODEC_H

#include <array>
#include <functional>
#include <iostream>
#include <memory>
#include <system_error>

#include "constant.h"
#include "frame.h"
#include "packet.h"
#include "option.h"

///@cond DOC_INTERNAL
class AVFrame;
class AVCodecContext;
class AVFormatContext;

namespace av {
class Packet;
}
///@endcond DOC_INTERNAL

/** @brief av namespace */
namespace av {

typedef std::unique_ptr< AVCodecContext, void(*)(AVCodecContext*) > av_codec_ptr;

/** @brief Media Codec. */
class Codec {
public:

    /** @brief Create new Encoder */
    Codec ( /** The codec id */             CODEC::Enum codec,
            /** The codec sample format */  SampleFormat sample_format,
            /** Codec options */            Options options = Options()
          );
    ~Codec();

    Codec ( const Codec& ) = delete;
    Codec& operator= ( const Codec& ) = delete;
    Codec ( Codec&& codec ) = delete;
    Codec& operator= ( Codec&& ) = delete;

//TODO implement equals

    /** @brief get the codec type */
    CODEC_TYPE::Enum codec_type() const;
    /** @brief get the codec */
    CODEC::Enum codec() const;

    /** @brief The stream index */
    int index() const;
    /** @brief The average bitrate. */
    int bitrate() const;
    /** @brief Samples per second. */
    int sample_rate() const;
    /** @brief Audio sample format */
    SampleFormat sample_fmt() const;
    /** @brief Number of audio channels */
    int channels() const;
    /** @brief Audio channel layout. */
    ChannelLayout::Enum channel_layout() const;
    /** @brief Bits per sample/pixel of internal libavcodec pixel/sample format. */
    int bits_per_sample() const;
    /** @brief Picture width / height. */
    int width() const;
    /** @brief Picture width / height. */
    int height() const;
    /** @brief Number of samples per channel in an audio frame. */
    int frame_size();
    /** @brief Check if the sample format is planar. */
    bool is_planar();

    /** @brief Call function with the frame decoded from the package. */
    std::error_code decode ( /** The package with the encoded data. */          Packet& package,
                             /** Callback function to call with the frame */    std::function< void ( Frame& ) > callback );

    /** @brief Call function with the package encoded from the frame. */
    std::error_code encode ( /** The frame with the raw data. */                Frame& frame,
                             /** Callback function to call with the package */  std::function< void ( Packet& ) > fn  );
    /** @brief Flush the encoder and write Package. */
    std::error_code encode ( /** Callback function to call with the package */  std::function< void ( Packet& ) > fn );

    //TODO
    size_t malloc_image ( uint8_t** video_dst_data, int* video_dst_linesize );
    //    void copy_image(Frame& frame, uint8_t* video_dst_data[4], int video_dst_linesize[4]);
    //TODO
    void copy_image ( Frame& frame, uint8_t* video_dst_data, int video_dst_linesize );

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

    /** @brief write the codec definitions to the output stream.
        @return reference to output stream */
    friend std::ostream& operator<< ( /** The target output stream. */  std::ostream& stream,
                                      /** The codec to print */         const Codec& codec ) {
        stream << name ( codec.codec_type() ) << ":" << name ( codec.codec() ) <<  " (" << codec.bitrate() << " kb/s, " <<
               codec.sample_rate() << " hz, " << codec.channels() << " channel(s), " <<
               codec.bits_per_sample() << " bps, " <<
               codec.width() << "x" << codec.height() << " px)";
        return stream;
    }
    /** @brief write the codec name to the output stream.
        @return reference to output stream */
    friend std::ostream& operator<< ( /** The target output stream. */  std::ostream& stream,
                                      /** The codec to print */         const CODEC::Enum& codec ) {
        stream << name ( codec );
        return stream;
    }
    /** @brief write the codec type to the output stream.
        @return reference to output stream */
    friend std::ostream& operator<< ( /** The target output stream. */  std::ostream& stream,
                                      /** The codec to print */         const CODEC_TYPE::Enum& codec_type ) {
        stream << name ( codec_type );
        return stream;
    }

private:
    friend class Format;
    friend class Resample;

    Codec ( AVFormatContext* format_context, const int index );
    Codec ( AVFormatContext* format_context, CODEC::Enum codec, Options options );

    std::error_code errc_;
    int index_;
    AVCodecContext* codec_context_ = nullptr;
    Frame frame_;
    Packet package_;

    //some helper methods
    static std::array< std::string, 5 > codec_names_;
    static std::array< std::string, 6 > codec_type_names_;
    static std::string name ( CODEC::Enum codec );
    static std::string name ( CODEC_TYPE::Enum codec );
};
}//namespace av
#endif // CODEC_H
