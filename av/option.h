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
#ifndef OPTION_H
#define OPTION_H

//#include <map>
#include <string>
#include <vector>

//#include <memory>
#include <stdexcept>

///@cond DOC_INTERNAL
extern "C" {
    struct AVDictionary;
}
///@endcond DOC_INTERNAL

/** @brief av namespace */
namespace av {

/** @brief av option */
class Option {
public:
    Option() {}
    enum TYPE {STRING, INT };
    Option ( const std::string& key, const std::string& val ) : type_ ( STRING ), key_ ( key ), str_val_ ( val ) {}
    Option ( const std::string& key, const int& val ) : type_ ( INT ), key_ ( key ), int_val_ ( val ) {}

    TYPE type() const
    {return type_;}

    const char* key() const
    {return key_.c_str();}

    const char* c_str() const
    {return str_val_.c_str();}

    int c_int() const
    {return int_val_;}

private:
    TYPE type_;
    std::string key_, str_val_;
    int int_val_;
};

/** @brief av option list. */
class Options {
public:
    /** @brief create empty options */
    Options() {}
    /** @brief create options with the values */
    Options ( std::vector< Option >&& values );
    ~Options();

    /** @brief test if options contain an entry with key */
    bool contains ( const std::string& x );
    /** @brief Get the entry with the key */
    Option get ( const std::string& x );
    /** @brief Set the entry */
    void set ( const Option& option );

private:
    friend struct Codec;
    friend struct Format;
    friend struct Resample;

    AVDictionary* opts = nullptr;
    AVDictionary** av_options();
    void store_option( const Option& __option );
};
}//namespace av
#endif // OPTION_H
