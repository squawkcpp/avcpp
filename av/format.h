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
#ifndef FORMAT_H
#define FORMAT_H

#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>
#include <string>
#include <system_error>
#include <vector>

///@cond DOC_INTERNAL
class AVFormatContext;
///@endcond DOC_INTERNAL

#include "codec.h"
#include "packet.h"
#include "metadata.h"

/** @brief av namespace */
namespace av {

class IoContext;

/** @brief Media file as av format. */
class Format {
public:
    typedef std::vector< std::shared_ptr< Codec > > codec_list;
    typedef codec_list::iterator iterator;

    /** @brief The i/o mode */
    enum Mode { READ, WRITE };

    /** @brief Open mediafile with a file by path */
    Format ( /** The path to the file to open */                const std::string& filename,
             /** Set the format i/o mode to READ or WRITE. */   Mode mode = READ,
             /** Set the AV option for the format. */           Options options = Options()
           );
    /** @brief Open mediafile from stream */
    Format ( /** The stream to open */                          std::iostream* s,
             /** Set the format i/o mode to READ or WRITE. */   Mode mode = READ,
             /** Set the AV option for the format. */           Options options = Options()
           );

    /** @brief Free all ressources. */
    ~Format();

    Format ( const Format& ) = delete;
    Format& operator= ( const Format& ) = delete;
    Format ( Format&& ) = delete;
    Format& operator= ( Format&& ) = delete;

    /** @brief Returns an iterator to the first codec of the format. */
    const iterator begin();
    /** @brief Returns an iterator to the last codec of the format. */
    const iterator end();
    /** @brief Returns a reference to the codec at specified location pos, with bounds checking. */
    const std::shared_ptr< Codec > at ( /** Position of the element to return */ const std::size_t& position ) const;
    /** @brief Returns the number of codecs in the format. */
    std::size_t size() const;
    /**
     * @brief play length of this media format in milliseconds.
     * @return play length in milliseconds.
     */
    uint64_t playtime() const;

    /** @brief Returns the metadata of the associated format context. */
    av::Metadata metadata() const ;

    /** @brief Read packet from input. */
    std::error_code read ( /** callback. */ std::function< void ( Packet& ) > callback  );
    /** @brief Write packet to output. */
    std::error_code write ( /** Packet to write. */ Packet& packet );

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
    friend class Packet;
    bool _header_written = false;
    AVFormatContext* format_context_ = nullptr;
    std::shared_ptr < IoContext > io_context_ = nullptr;
    std::error_code errc_;
    codec_list codecs_;
};

static bool is_audio ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::AUDIO;
}
static bool is_video ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::VIDEO;
}
static bool is_data ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::DATA;
}
static bool is_subtitle ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::SUBTITLE;
}
static bool is_attachement ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::ATTACHMENT;
}
static bool is_nb ( std::shared_ptr< Codec > codec ) {
    return codec->codec_type() == CODEC_TYPE::NB;
}
}//namespace av
#endif // FORMAT_H
