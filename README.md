## C++ wrapper for ffmpeg.

[![Build Status](https://travis-ci.org/squawkcpp/avcpp.svg?branch=master)](https://travis-ci.org/squawkcpp/avcpp)
[ ![Download](https://api.bintray.com/packages/squawkcpp/conan-cpp/avcpp%3Aconan-cpp/images/download.svg) ](https://bintray.com/squawkcpp/conan-cpp/avcpp%3Aconan-cpp)

C++ wrapper for the [ffmpeg](http://www.ffmpeg.org) multimedia library project.

## Usage

### Open media file and get information

To open a media file the av::Format is created with a filename as std::string or with std::iostream. The write flag
indicates the i/o mode of the av::Format. Default is READ. The av::Options parameter contains default options for the format.
or codec to load.

The av::Format class provides std::system_error reporting the internal state. When av::format is in read
mode the automatically loaded av::Codec classes can be iteratored. This class can read or write av::Packages.

'''
enum Format::Mode { READ, WRITE };

av::Format ( const std::string& filename,   /** The path to the file to open */
             av::Format::Mode mode = READ,  /** Set the format i/o mode to READ or WRITE. */
             Options options = Options()    /** Set the AV option for the format. */
);

'''

#### Open a file and get the codecs.

'''
#include "format.h"

av::Format format_( "<path to mediafile>" );
if( !format_ ) {
    //Success
    auto audio_codec = std::find_if( format_.begin(), format_.end(), is_audio );
    ... or
    for(auto& codec : format_ )
        ....

} else {
    //failure
}
'''

#### Read av::Package.

'''
#include "format.h"

av::Format format_( "<path to mediafile>" );
if( !format_ ) {
    //Success
    auto audio_codec = std::find_if( format_.begin(), format_.end(), is_audio );
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx

} else {
    //failure
}
'''

### transcode audiofile.

'''
'''

### transode media file

### Use filter in chain


## Include the conan package

avcpp can be used as a conan package. the library need to be linked against the ffmpeg shared or static libraries.

### Basic setup

    $ conan install avcpp/0.1.1@conan-cpp/latest

### Project setup

If you handle multiple dependencies in your project is better to add a *conanfile.txt*

    [requires]
    avcpp/0.1.1@conan-cpp/latest

    [options]
    avcpp:shared=[True, False]

    [generators]
    cmake

Complete the installation of requirements for your project running:

    $ mkdir build && cd build && conan install ..

Note: It is recommended that you run conan install from a build directory and not the root of the project directory.  This is because conan generates *conanbuildinfo* files specific to a single build configuration which by default comes from an autodetected default profile located in ~/.conan/profiles/default .  If you pass different build configuration options to conan install, it will generate different *conanbuildinfo* files.  Thus, they should not be added to the root of the project, nor committed to git.

## For Users: Use this library

The usage is similar to the process in ffmpeg. The class av::Format is used to read from or write to a file/stream. av:Packages are iterated
from the av::Format. the av::Format is passed to the codec, which will create av::Frame for successfully decoded media. Encoding media
works the other way arround, from Frames to Packages. The error message are wrapped in a std::error_code object.

Some complete examples are in the samples directory of the source tree.

## License
[Boost](www.boost.org/LICENSE_1_0.txt)
