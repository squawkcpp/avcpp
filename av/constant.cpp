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
#include "constant.h"

extern "C" {
#include "libavformat/avformat.h"
#include "libavcodec/avcodec.h"
#include "libavutil/avstring.h"
}

namespace av {

int64_t ChannelLayout::get ( ChannelLayout::Enum format ) {
    switch ( format ) {
    case CH_LAYOUT_MONO:
        return ( AV_CH_FRONT_CENTER );

    case CH_LAYOUT_STEREO:
        return ( AV_CH_FRONT_LEFT|AV_CH_FRONT_RIGHT );

    case CH_LAYOUT_2POINT1:
        return ( AV_CH_LAYOUT_STEREO|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_2_1:
        return ( AV_CH_LAYOUT_STEREO|AV_CH_BACK_CENTER );

    case CH_LAYOUT_SURROUND:
        return ( AV_CH_LAYOUT_STEREO|AV_CH_FRONT_CENTER );

    case CH_LAYOUT_3POINT1:
        return ( AV_CH_LAYOUT_SURROUND|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_4POINT0:
        return ( AV_CH_LAYOUT_SURROUND|AV_CH_BACK_CENTER );

    case CH_LAYOUT_4POINT1:
        return ( AV_CH_LAYOUT_4POINT0|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_2_2:
        return ( AV_CH_LAYOUT_STEREO|AV_CH_SIDE_LEFT|AV_CH_SIDE_RIGHT );

    case CH_LAYOUT_QUAD:
        return ( AV_CH_LAYOUT_STEREO|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT );

    case CH_LAYOUT_5POINT0:
        return ( AV_CH_LAYOUT_SURROUND|AV_CH_SIDE_LEFT|AV_CH_SIDE_RIGHT );

    case CH_LAYOUT_5POINT1:
        return ( AV_CH_LAYOUT_5POINT0|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_5POINT0_BACK:
        return ( AV_CH_LAYOUT_SURROUND|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT );

    case CH_LAYOUT_5POINT1_BACK:
        return ( AV_CH_LAYOUT_5POINT0_BACK|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_6POINT0:
        return ( AV_CH_LAYOUT_5POINT0|AV_CH_BACK_CENTER );

    case CH_LAYOUT_6POINT0_FRONT:
        return ( AV_CH_LAYOUT_2_2|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER );

    case CH_LAYOUT_HEXAGONAL   :
        return ( AV_CH_LAYOUT_5POINT0_BACK|AV_CH_BACK_CENTER );

    case CH_LAYOUT_6POINT1:
        return ( AV_CH_LAYOUT_5POINT1|AV_CH_BACK_CENTER );

    case CH_LAYOUT_6POINT1_BACK:
        return ( AV_CH_LAYOUT_5POINT1_BACK|AV_CH_BACK_CENTER );

    case CH_LAYOUT_6POINT1_FRONT:
        return ( AV_CH_LAYOUT_6POINT0_FRONT|AV_CH_LOW_FREQUENCY );

    case CH_LAYOUT_7POINT0:
        return ( AV_CH_LAYOUT_5POINT0|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT );

    case CH_LAYOUT_7POINT0_FRONT:
        return ( AV_CH_LAYOUT_5POINT0|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER );

    case CH_LAYOUT_7POINT1:
        return ( AV_CH_LAYOUT_5POINT1|AV_CH_BACK_LEFT|AV_CH_BACK_RIGHT );

    case CH_LAYOUT_7POINT1_WIDE:
        return ( AV_CH_LAYOUT_5POINT1|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER );

    case CH_LAYOUT_7POINT1_WIDE_BACK:
        return ( AV_CH_LAYOUT_5POINT1_BACK|AV_CH_FRONT_LEFT_OF_CENTER|AV_CH_FRONT_RIGHT_OF_CENTER );

    case CH_LAYOUT_OCTAGONAL   :
        return ( AV_CH_LAYOUT_5POINT0|AV_CH_BACK_LEFT|AV_CH_BACK_CENTER|AV_CH_BACK_RIGHT );

    case CH_LAYOUT_HEXADECAGONAL:
        return ( AV_CH_LAYOUT_OCTAGONAL|AV_CH_WIDE_LEFT|AV_CH_WIDE_RIGHT|AV_CH_TOP_BACK_LEFT|AV_CH_TOP_BACK_RIGHT|AV_CH_TOP_BACK_CENTER|AV_CH_TOP_FRONT_CENTER|AV_CH_TOP_FRONT_LEFT|AV_CH_TOP_FRONT_RIGHT );

    case CH_LAYOUT_STEREO_DOWNMIX:
        return ( AV_CH_STEREO_LEFT|AV_CH_STEREO_RIGHT );
    }
}
}//namespace av
