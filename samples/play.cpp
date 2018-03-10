#include <iostream>

#include "../av/format.h"
#include "../av/frame.h"
#include "../av/packet.h"

extern "C" {
#include "SDL/SDL.h"
};

int main(int argc, char* argv[]) {

    av::Format format( argv[1] );
    if( !!format ) {
        std::cout << "Error: " << format.errc().message() << std::endl;
        exit(1);
    }
    auto _codec = std::find_if( format.begin(), format.end(), av::is_audio );
    auto _codec_video = std::find_if( format.begin(), format.end(), av::is_video );


    /** configure SDL */
    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
      fprintf(stderr, "Could not initialize SDL - %s\n", SDL_GetError());
      exit(1);
    }

    SDL_Surface *screen;
    screen = SDL_SetVideoMode((*_codec_video)->width(), (*_codec_video)->height(), 0, 0);
    if(!screen) {
      fprintf(stderr, "SDL: could not set video mode - exiting\n");
      exit(1);
    }

    SDL_Overlay     *bmp = NULL;
    struct SWSContext *sws_ctx = NULL;

//    bmp = SDL_CreateYUVOverlay(pCodecCtx->width, pCodecCtx->height,
//                               SDL_YV12_OVERLAY, screen);

//    // initialize SWS context for software scaling
//    sws_ctx = sws_getContext(pCodecCtx->width,
//                             pCodecCtx->height,
//                 pCodecCtx->pix_fmt,
//                 pCodecCtx->width,
//                 pCodecCtx->height,
//                 PIX_FMT_YUV420P,
//                 SWS_BILINEAR,
//                 NULL,
//                 NULL,
//                 NULL
//                 );




















//    int screen_w=0,screen_h=0;
//    SDL_Window *screen;
//    static SDL_Renderer* sdlRenderer;
//    static SDL_Texture* sdlTexture;
//    static SDL_Rect sdlRect;

//    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_TIMER)) {
//        printf( "Could not initialize SDL - %s\n", SDL_GetError());
//        return -1;
//    }
//    screen_w = (*_codec_video)->width();
//    screen_h = (*_codec_video)->height();
//    //SDL 2.0 Support for multiple windows
//    screen = SDL_CreateWindow("Simplest ffmpeg player's Window", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
//        screen_w, screen_h,
//        SDL_WINDOW_OPENGL);

//    if(!screen) {
//        printf("SDL: could not create window - exiting:%s\n",SDL_GetError());
//        return -1;
//    }

//    sdlRenderer = SDL_CreateRenderer(screen, -1, 0);
//        //IYUV: Y + U + V  (3 planes)
//        //YV12: Y + V + U  (3 planes)

//    sdlTexture = SDL_CreateTexture(sdlRenderer, SDL_PIXELFORMAT_IYUV, SDL_TEXTUREACCESS_STREAMING,(*_codec_video)->width(),(*_codec_video)->height());


//    static uint8_t *video_dst_data[4] = {NULL};
//    static int      video_dst_linesize[4];
//    static size_t img_buf_size = (*_codec_video)->malloc_image( video_dst_data, video_dst_linesize );

//    std::error_code errc = format.read( [_codec,_codec_video]( av::Packet& package ) {
//        if( package.stream_index() == (*_codec)->index() ) {
//            (*_codec)->decode( package, [_codec]( av::Frame& frame ) {
//                //write to out file
//                if( (*_codec)->is_planar() ) {
//                    for( int i = 0; i < frame.nb_samples(); i++ )
//                        for( int ch = 0; ch < (*_codec)->channels(); ch++ ) {}
////                            outfile.write( (char*)(frame.data(ch) + frame.data_size()*i), frame.data_size() );
//                } else {
////                    outfile.write( (char*)frame.extended_data(0), frame.linesize(0) );
//                }
//            });
//        } else if( package.stream_index() == (*_codec_video)->index() ) {
//            (*_codec_video)->decode( package, [_codec_video]( av::Frame& frame ) {
//                (*_codec_video)->copy_image( frame, frame.data(0), frame.linesize(0) );

////                outfile_video.write( (char*)video_dst_data[0], img_buf_size );
////                SDL_UpdateTexture( sdlTexture, NULL, video_dst_data[0], video_dst_linesize[0] );

//                SDL_UpdateYUVTexture(sdlTexture, &sdlRect,
//                video_dst_data[0], video_dst_linesize[0],
//                video_dst_data[1], video_dst_linesize[1],
//                video_dst_data[2], video_dst_linesize[2]);

//                SDL_RenderClear( sdlRenderer );
//                SDL_RenderCopy( sdlRenderer, sdlTexture,  NULL, &sdlRect);
//                SDL_RenderPresent( sdlRenderer );
//                //SDL End-----------------------
//                //Delay 40ms
//                SDL_Delay(40);

//            });
//        }
//    });
//    std::cout << errc.message() << std::endl;

    return 0;
}
