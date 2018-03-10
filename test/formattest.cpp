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

#include <functional>
#include <string>
#include <fstream>
#include <map>

#include "../av/averrc.h"
#include "../av/codec.h"
#include "../av/format.h"
#include "../av/packet.h"

#include <gtest/gtest.h>

namespace av {

static const char* FILE_VIDEO = TESTFILES "Micayala_DivX1080p_ASP.divx";
static const char* FILE_FLAC = TESTFILES "BIS1536-001-flac_16.flac";
static const char* FILE_MP3 = TESTFILES "sample.mp3";

TEST(Format, NoSuchFile) {
    Format format_( "foo" );
    EXPECT_TRUE( !!format_ );
    EXPECT_EQ( "No such file or directory", format_.errc().message() );
}
TEST(Format, OpenFile) {
    Format format_( FILE_FLAC );
    EXPECT_TRUE( !format_ );
    EXPECT_EQ( std::error_code().message(), format_.errc().message() );
}
TEST(Format, packets_iterate ) {
    Format format_( FILE_FLAC );
    int audio_ = 0;
    int other_ = 0;
    auto _errc = format_.read([&](Packet& package ) {
            if( package.stream_index() == 0 ) {
                audio_+=1;
            } else other_+=1;
    });
    ASSERT_EQ( "End of file", _errc.message() );
    EXPECT_EQ( 2217, audio_ );
    EXPECT_EQ( 5, other_ );
}
TEST(Format, decode_audio ) {

    Format format_( FILE_FLAC );

    int audio_ = 0;
    int other_ = 0;

    auto _codec = std::find_if( format_.begin(), format_.end(), is_audio );

    std::error_code _errc = format_.read( [_codec,&audio_,&other_]( av::Packet& package ) {
        if( package.stream_index() == (*_codec)->index() ) {
            audio_+=1;
            (*_codec)->decode( package, []( av::Frame& ) {});
        } else other_+=1;
    });
    ASSERT_EQ( "End of file", _errc.message() );
    EXPECT_EQ( 2217, audio_ );
    EXPECT_EQ( 5, other_ );
}

TEST(Format, decode ) {

    Format format_( FILE_VIDEO );

    int audio_ = 0;
    int video_ = 0;
    int other_ = 0;

    auto _codec = std::find_if( format_.begin(), format_.end(), is_audio );
    auto _audio_codec = std::find_if( format_.begin(), format_.end(), is_video );

    std::error_code _errc = format_.read( [_codec,_audio_codec,&audio_,&video_,&other_]( av::Packet& package ) {
        if( package.stream_index() == (*_codec)->index() ) {
            audio_+=1;
            (*_codec)->decode( package, []( av::Frame& ) {});
        } else if( package.stream_index() == (*_audio_codec)->index() ) {
            video_+=1;
            (*_codec)->decode( package, []( av::Frame& ) {});
        } else other_+=1;
    });
    ASSERT_EQ( "End of file", _errc.message() );
    EXPECT_EQ( 5252, audio_ );
    EXPECT_EQ( 3287, video_ );
    EXPECT_EQ( 0, other_ );
}
}//namespace av
