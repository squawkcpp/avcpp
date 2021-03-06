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
#ifndef METADATA_H
#define METADATA_H

#include <map>
#include <iostream>
#include <string>
#include <sstream>
#include <vector>

/** @brief av namespace */
namespace av {

/**
 * @brief The Metadata struct
 *
 * <p>Media tag description and comparison table from http://age.hobba.nl/audio/tag_frame_reference.html</p>
 *
 * @ htmlinclude tag_frame_reference.html
 */
struct Metadata {
public:
    /** @brief enum with metadata elements for read and write access. */
    enum Enum {
        TITLE, ALBUM, ARTIST, COMPOSER, PERFORMER,
        COMMENT, YEAR, TRACK, DISC, GENRE, PUBLISHER
    };
    /** @brief set metadata with value by key. */
    void set (
        const char* name /** @param name the metadata key */,
        const char* value /** @param value the value for the metadata */
    );
    /** @brief get the metadata key type as string.
        @return the ffmpeg key as string. */
    static std::string name ( Enum tag /** @param tag the key to get as string. */ );
    /** @brief get the metadata value by key.
        @return the value referenced by key. */
    std::string& get ( const Enum& key /** @param key the key for the metadata element. */ );
    /** @brief get keys of valid metadata elments.
        @return the keys of the defined metadata elements */
    std::vector< Enum > tag_names ();

    friend std::ostream& operator<< ( std::ostream& stream, const Metadata& meta_data ) {
        for ( auto& __tag : meta_data.tags ) {
            stream << name ( __tag.first ) << ": \t\t" << __tag.second << "\n";
        }

        return stream;
    }
private:
    std::map< Enum, std::string > tags;
};
}//namespace av
#endif // METADATA_H
