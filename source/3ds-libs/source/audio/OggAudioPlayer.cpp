#include "../../include/audio/OggAudioPlayer.h"

OggAudioPlayer::OggAudioPlayer(char* filePath){
    load(filePath);
}

AudioResult OggAudioPlayer::load(char* filePath){
    FILE * file = fopen(filePath, "rb");

    if(file == 0) return AudioResult::file_not_found;
    if(ov_open(file, &ovf, NULL, 0) < 0){
        fclose(file);
        return AudioResult::codec_error;
    }

    vorbis_info * info = ov_info(&ovf, -1);
    if(info == NULL){
        fclose(file);
        return AudioResult::no_info;
    }

    channels = (u32)info->channels;
    samples = (u32)ov_pcm_total(&ovf, -1);
    size = samples * channels * 2;

    if(linearSpaceFree() < size){
        ov_clear(&ovf);
        fclose(file);
        return AudioResult::not_enough_memory;
    }

    rate = (float)info->rate;
    data = (char*)linearAlloc(size);
    if(data == 0) return AudioResult::stream_no_data;

    int offset = 0;
    int eof = 0;
	int current;
    while (!eof) {
		long ret = ov_read(&ovf, &data[offset], AUDIO_BUFFER_SIZE, &current);
		if (ret == 0) eof = 1;
		else if (ret < 0) {
			ov_clear(&ovf);
			linearFree(data);
            return AudioResult::stream_error;
		}
		else offset += ret;
	}

    memset(&waveBuf[0], 0, sizeof(ndspWaveBuf));
    waveBuf[0].data_vaddr = data;
	waveBuf[0].nsamples = samples;
	waveBuf[0].looping = loop;
	waveBuf[0].status = NDSP_WBUF_FREE;

    DSP_FlushDataCache(data, size);

    ov_clear(&ovf);
    fclose(file);
    loaded = true;
    return AudioResult::success;
}