#include "../../include/audio/AudioPlayerBase.h"

AudioPlayerBase::~AudioPlayerBase(){
    unload();
}

void AudioPlayerBase::setSpeed(float speed_){
    speed = speed_;
    if(!loaded) return;
    ndspChnSetRate(channel, rate*speed);
}

void AudioPlayerBase::play(){
    //TODO: Replace with streaming system
    stop();
    ndspChnInitParams(channel);
    ndspChnSetInterp(channel,interp);
    ndspChnSetRate(channel, rate*speed);
    ndspChnSetFormat(channel, NDSP_CHANNELS(channels) | NDSP_ENCODING(encoding));
    ndspChnWaveBufAdd(channel, &waveBuf[0]);

    s32 pr = 0;
    size_t stackSize = 4 * 1024;
	svcGetThreadPriority(&pr, CUR_THREAD_HANDLE);
    threadCreate(waitForEnd,(void*)this, stackSize, pr-1, -2, true);

}

void AudioPlayerBase::stop(){
    ndspChnWaveBufClear(channel);
    ndspChnReset(channel);
}

void AudioPlayerBase::unload(){
    loaded = false;
    linearFree(waveBuf);
    linearFree(data);
}

bool AudioPlayerBase::isLoaded(){
    return loaded;
}

bool AudioPlayerBase::isPlaying(){
    return ndspChnIsPlaying(channel);
}

void AudioPlayerBase::waitForEnd(void* arg){
    AudioPlayerBase *player = (AudioPlayerBase*)arg;
    while (aptMainLoop() && player->isPlaying());
    svcSignalEvent(player->ended);
}