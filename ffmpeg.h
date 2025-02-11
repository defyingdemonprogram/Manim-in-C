#ifndef FFMPEG_H_
#define FFMPEG_H_

#include <stddef.h>  // Include stddef.h for size_t
#include <stdbool.h>

typedef struct FFMPEG FFMPEG;

FFMPEG *ffmpeg_start_rendering(size_t width, size_t height, size_t fps);
bool ffmpeg_send_frame_flipped(FFMPEG *ffmpeg, void *data, size_t width, size_t height);
bool ffmpeg_end_rendering(FFMPEG *ffmpeg, bool cancel);

#endif // FFMPEG_H_
