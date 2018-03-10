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
#include "metadata.h"

#include <strings.h>
#include <cstring>

namespace av {

const static char* _TITLE = "title";
const static char* _ALBUM = "album";
const static char* _ARTIST = "artist";
const static char* _COMPOSER = "composer";
const static char* _PERFORMER = "performer";
const static char* _COMMENT = "comment";
const static char* _YEAR = "year";
const static char* _TRACK = "track";
const static char* _DISC = "disc";
const static char* _GENRE = "genre";
const static char* _PUBLISHER = "publisher";

static std::array< std::string, 11 > _tag_names {
    { _TITLE, _ALBUM, _ARTIST, _COMPOSER, _PERFORMER, _COMMENT, _YEAR, _TRACK, _DISC, _GENRE, _PUBLISHER }
};
void Metadata::set ( const char* name, const char* value ) {

    if ( strcasecmp ( _TITLE, name ) == 0 )
    { tags[ TITLE ] = value; }

    else if ( strcasecmp ( _ALBUM, name ) == 0 )
    { tags[ ALBUM ] = value; }

    else if ( strcasecmp ( _ARTIST, name ) == 0 )
    { tags[ ARTIST ] = value; }

    else if ( strcasecmp ( _COMPOSER, name ) == 0 )
    { tags[ COMPOSER ] = value; }

    else if ( strcasecmp ( _PERFORMER, name ) == 0 )
    { tags[ PERFORMER ] = value; }

    else if ( strcasecmp ( _COMMENT, name ) == 0 )
    { tags[ COMMENT ] = value; }

    else if ( strcasecmp ( _YEAR, name ) == 0 ) {
        std::string _year = value;

        if ( _year.size() > 4 ) {
            if ( 4U == _year.find ( "-" ) ) {
                _year = _year.substr ( 0, 4 );

            } else { std::cout << "unknown date format: " << _year << std::endl; }
        }

        tags[ YEAR ] = _year;
    }

    else if ( strcasecmp ( _TRACK, name ) == 0 )
    { tags[ TRACK ] = value; }

    else if ( strcasecmp ( _DISC, name ) == 0 )
    { tags[ DISC ] = value; }

    else if ( strcasecmp ( _GENRE, name ) == 0 )
    { tags[ GENRE ] = value; }

    else if ( strcasecmp ( _PUBLISHER, name ) == 0 )
    { tags[ PUBLISHER ] = value; }

    //extra values are skipped
}
std::string& Metadata::get ( const Metadata::Enum& key ) {
    return tags[ key ];
}
std::vector< Metadata::Enum > Metadata::tag_names () {
    std::vector< Enum > _keys;

    for ( auto const& k: tags )
    { _keys.push_back ( k.first ); }

    return _keys;
}
std::string Metadata::name ( Enum tag ) {
    return _tag_names[ static_cast< unsigned int > ( tag )];
}
}//namespace libav
