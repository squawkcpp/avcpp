#include <iostream>
#include "av/format.h"

int main() {
	av::Format format_( "" ); 
	if( !format_ ) { //Success auto 
		auto audio_codec = std::find_if( format_.begin(), format_.end(), av::is_audio ); 
		std::cout << (*audio_codec) << std::endl;
		std::cout << format_.metadata() << std::endl;
	}
}
