#include <fstream>
#include <iostream>

#include "../av/format.h"
#include "../av/frame.h"
#include "../av/packet.h"

int main (int argc, char **argv) {

    if (argc != 4 && argc != 5) {
        fprintf(stderr, "usage: %s [-refcount] input_file video_output_file audio_output_file\n"
                "API example program to show how to read frames from an input file.\n"
                "This program reads frames from a file, decodes them, and writes decoded\n"
                "video frames to a rawvideo file named video_output_file, and decoded\n"
                "audio frames to a rawaudio file named audio_output_file.\n\n"
                "If the -refcount option is specified, the program use the\n"
                "reference counting frame system which allows keeping a copy of\n"
                "the data for longer than one decode call.\n"
                "\n", argv[0]);
        exit(1);
    }

    av::Format format( argv[1] );
    if( !!format ) {
        std::cout << "Error: " << format.errc().message() << std::endl;
        exit(1);
    }

    static uint8_t *video_dst_data[4] = {NULL};
    static int      video_dst_linesize[4];

    std::ofstream outfile ( argv[3] );
    std::ofstream outfile_video ( argv[2] );
    auto _codec = std::find_if( format.begin(), format.end(), av::is_audio );
    auto _codec_video = std::find_if( format.begin(), format.end(), av::is_video );
    static size_t img_buf_size = (*_codec_video)->malloc_image( video_dst_data, video_dst_linesize );
    std::error_code errc = format.read( [_codec,_codec_video,&outfile,&outfile_video]( av::Packet& package ) {
        if( package.stream_index() == (*_codec)->index() ) {
            (*_codec)->decode( package, [_codec,&outfile]( av::Frame& frame ) {
                //write to out file
                if( (*_codec)->is_planar() ) {
                    for( int i = 0; i < frame.nb_samples(); i++ )
                        for( int ch = 0; ch < (*_codec)->channels(); ch++ )
                            outfile.write( (char*)(frame.data(ch) + frame.data_size()*i), frame.data_size() );
                } else {
                    outfile.write( (char*)frame.extended_data()[0], frame.linesize(0) );
                }
            });
        } else if( package.stream_index() == (*_codec_video)->index() ) {
            (*_codec_video)->decode( package, [_codec_video,&outfile_video]( av::Frame& frame ) {
                (*_codec_video)->copy_image( frame, frame.data(0), frame.linesize(0) );
                outfile_video.write( (char*)video_dst_data[0], img_buf_size );
            });
        }
    });
    outfile.close();
    outfile_video.close();
    std::cout << errc.message() << std::endl;
    return 0;

}
