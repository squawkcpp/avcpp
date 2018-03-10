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
#include "option.h"

#include <algorithm>

extern "C" {
#include "libavutil/opt.h"
}

namespace av {

void Options::store_option( const Option& __option ) {
    switch ( __option.type() ) {
    case Option::INT:
        av_dict_set_int ( &opts, __option.key(), __option.c_int(), 0 );
        break;

    case Option::STRING:
        av_dict_set ( &opts, __option.key(), __option.c_str(), 0 );
        break;
    }
}

Options::Options ( std::vector< Option >&& values ) {
    for ( auto& __option : values )
    {store_option(__option);}
}

Options::~Options() {
    if ( opts )
    { av_dict_free ( &opts ); }
}

bool Options::contains ( const std::string& x ) {
    AVDictionaryEntry *t = NULL;
    while ( (  t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX ) ) ) {
        if( strcmp( t->key, x.c_str() ) ) return true;
    }
    return false;
}
Option Options::get ( const std::string& x ) {
    AVDictionaryEntry *t = NULL;
    while ( ( t = av_dict_get(opts, "", t, AV_DICT_IGNORE_SUFFIX ) ) ) {
        if( strcmp( t->key, x.c_str() ) == 0 )
        {return Option(t->key, t->value);}
    }
    throw( std::out_of_range( "dictionary entry not found:" + x ) );
}
void Options::set ( const Option& __option )
{store_option(__option);}

AVDictionary** Options::av_options()
{ return &opts; }
}//namespace av
