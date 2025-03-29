#include "AudioPlayerBase.h"
#include <ogg/ogg.h>
#include <tremor/ivorbisfile.h>

#define AUDIO_BUFFER_SIZE 4096
#define STACKSIZE (4 * 1024)

class OggAudioPlayer : public AudioPlayerBase {
    public:
        OggAudioPlayer(char* filePath);
        OggAudioPlayer() = default;
        virtual ~OggAudioPlayer() = default;

        OggVorbis_File ovf;

        /** 
        * Loads OGG vorbis audio from the specified file path.
        * @param filePath Path to the .ogg audio file.
        */
        virtual AudioResult load(char* filePath);
};