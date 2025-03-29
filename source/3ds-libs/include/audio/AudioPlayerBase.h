#include <3ds.h>
#include "../Classified.h"
#include "../debug.h"

enum AudioResult {
    success,
    file_not_found,
    not_enough_memory,
    codec_error,
    no_info,
    stream_no_data,
    stream_error,
};

class AudioPlayerBase : public Classified {
    friend class OggAudioPlayer;

    public:
        AudioPlayerBase() = default;
        virtual ~AudioPlayerBase();

        Handle ended;

        ndspWaveBuf waveBuf[1];
        int channel = 0;
        bool loop = false;

        /** Plays the audio */
        virtual void play();
        /** 
         * Loads audio from the specified file path.
         * @param filePath Path to the audio file.
         */
        virtual AudioResult load(char* filePath) = 0;
        /** Unloads the loaded audio file. */
        virtual void unload();
        /** Stops the audio. */
        void stop();
        /** Checks if an audio file is loaded. */
        bool isLoaded();
        /** Checks if the audio is playing. */
        bool isPlaying();
        /** 
         * Sets audio playback speed. 
         * @param speed_ The speed you want to set. 1 is default */
        void setSpeed(float speed_);
        /** Returns the audio playback speed. */
        float getSpeed(){return speed;}
    private:
        bool loaded = false;
        float speed = 1;
    	float rate;
        char* data;
        ndspInterpType interp = NDSP_INTERP_NONE;
        u32 encoding = NDSP_ENCODING_PCM16;
	    u32 channels;
	    u32 samples;
	    u32 size;

        static void waitForEnd(void* arg);
    protected:
        static int occupiedChannels;
};