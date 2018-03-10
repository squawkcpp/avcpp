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
#include "packet.h"

extern "C" {
#include "libavformat/avformat.h"
}

#include "format.h"

namespace av {

Packet::Packet()
{packet_ = av_packet_alloc();}
Packet::Packet ( Format* format ) : format_ ( format->format_context_ )
{packet_ = av_packet_alloc();}
Packet::~Packet()
{av_packet_free ( &packet_ );}

int64_t Packet::pts()
{return packet_->pts;}
int64_t Packet::dts()
{return packet_->dts;}
uint8_t* Packet::data()
{return packet_->data;}
int Packet::size()
{return packet_->size;}
size_t Packet::stream_index() const
{return packet_->stream_index;}
int Packet::flags()
{return packet_->flags;}
void Packet::unref()
{av_packet_unref ( packet_ );}
}//namespace av
