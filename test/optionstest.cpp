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

#include "../av/option.h"

#include <gtest/gtest.h>

namespace av {

TEST( OptionsTest, add_options ) {

    Options option;
    option.set( Option( "option1", "value1" ) );
    option.set( Option( "option2", "value2" ) );
    option.set( Option( "option3", "value3" ) );

    std::string str1 = option.get( "option1" ).c_str();
    EXPECT_EQ( "value1", str1 );

    std::string str2 = option.get( "option2").c_str();
    EXPECT_EQ( "value2", str2 );

    std::string str3 = option.get( "option3").c_str();
    EXPECT_EQ( "value3", str3 );
}
TEST( OptionsTest, construct_with_options ) {

    Options option( { { "option1", "value1" }, {"option2", "value2"}, {"option3", "value3" } } );

    std::string str1 = option.get( "option1").c_str();
    EXPECT_EQ( "value1", str1 );

    std::string str2 = option.get( "option2").c_str();
    EXPECT_EQ( "value2", str2 );

    std::string str3 = option.get( "option3").c_str();
    EXPECT_EQ( "value3", str3 );
}


}//namespace av
