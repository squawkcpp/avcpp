#include <iostream>

extern "C" {
#include <ao/ao.h>
}

#include "../av/format.h"
#include "../av/frame.h"
#include "../av/package.h"

int main(int argc, char* argv[]) {

    av::Format format( argv[1] );

    auto metadata_ = format.metadata();
    std::cout << "Play: " << metadata_.get( av::Metadata::TITLE ) << ", " <<
                 "Artist: " << metadata_.get( av::Metadata::ARTIST ) << ", " <<
                 "playtime: " << format.playtime() << std::endl;

    auto _codec = std::find_if( format.begin(), format.end(), av::is_audio );

    // To initalize libao for playback
    ao_initialize();

    int driver = ao_default_driver_id();

    // The format of the decoded PCM samples
    ao_sample_format sample_format;
    sample_format.bits = 16;
    sample_format.channels = 2;
    sample_format.rate = 44100;
    sample_format.byte_format = AO_FMT_NATIVE;
    sample_format.matrix = 0;

    ao_device* device = ao_open_live(driver, &sample_format, NULL);

    av::Package _package;
    av::Frame _frame;
    std::error_code errc;
    while( ! ( errc = format.read( _package ) ) ) {
        if( _package.stream_index() == _codec->index() ) {
            while( ! ( errc = _codec->decode( _package, _frame ) ) ) {
                if( _codec->is_planar() ) {
                    for( int i = 0; i < _frame.nb_samples(); i++ )
                        for( int ch = 0; ch < _codec->channels(); ch++ )
                            ao_play(device, (char*)(_frame.data(ch) + _frame.data_size()*i), _frame.data_size() );
                } else {
                    ao_play(device, (char*)_frame.extended_data(0), _frame.linesize(0) );
                }
            }
        }
    }
    std::cout << std::endl;
    std::cout << errc.message() << std::endl;
    return 0;
}
