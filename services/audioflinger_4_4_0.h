/*
 * Copyright (C) 2015 Jolla Ltd.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 * Authored by: Juho Hämäläinen <juho.hamalainen@jollamobile.com>
 */

#include <media/IAudioFlingerClient.h>
#include "IPrivateAfGlue.h"
#include "common.h"

using namespace android;

class MiniAudioFlinger : public BinderService<MiniAudioFlinger>,
                         public BnAudioFlinger,
                         public IBinder::DeathRecipient
{
    friend class BinderService<MiniAudioFlinger>;

    static sp<IPrivateAfGlue> gPrivateAfGlue;

public:
    static char const *getServiceName() {
        return "media.audio_flinger";
    }

    MiniAudioFlinger() : BnAudioFlinger()
    {
        sp<IPrivateAfGlue> af;
        sp<IServiceManager> sm = defaultServiceManager();
        sp<IBinder> binder;

        binder = sm->getService(String16(DROID_AFGLUE_SERVICE_NAME));
        if (binder == 0) {
            ALOGE("Couldn't find %s service", DROID_AFGLUE_SERVICE_NAME);
        }
        af = interface_cast<IPrivateAfGlue>(binder);

        gPrivateAfGlue = af;
    }

    ~MiniAudioFlinger() {}

    void binderDied(const wp<IBinder>&) {
        // Nothing
    }

    virtual sp<IAudioTrack> createTrack(
                                audio_stream_type_t streamType,
                                uint32_t sampleRate,
                                audio_format_t format,
                                audio_channel_mask_t channelMask,
                                size_t frameCount,
                                track_flags_t *flags,
                                const sp<IMemory>& sharedBuffer,
                                audio_io_handle_t output,
                                pid_t tid,  // -1 means unused, otherwise must be valid non-0
                                int *sessionId,
                                // input: ignored
                                // output: server's description of IAudioTrack for display in logs.
                                // Don't attempt to parse, as the format could change.
                                String8& name,
                                int clientUid,
                                status_t *status) {
        return 0;
    }


#ifdef QCOM_DIRECTTRACK
    /* create a direct audio track and registers it with AudioFlinger.
     * return null if the track cannot be created.
     */
    virtual sp<IDirectTrack> createDirectTrack(
                                pid_t pid,
                                uint32_t sampleRate,
                                audio_channel_mask_t channelMask,
                                audio_io_handle_t output,
                                int *sessionId,
                                IDirectTrackClient* client,
                                audio_stream_type_t streamType,
                                status_t *status) {
        return 0;
    }
#endif

    virtual sp<IAudioRecord> openRecord(
                                audio_io_handle_t input,
                                uint32_t sampleRate,
                                audio_format_t format,
                                audio_channel_mask_t channelMask,
                                size_t frameCount,
                                track_flags_t *flags,
                                pid_t tid,  // -1 means unused, otherwise must be valid non-0
                                int *sessionId,
                                status_t *status) {
        return 0;
    }

    virtual uint32_t sampleRate(audio_io_handle_t output) const {
        return 0;
    }

    virtual audio_format_t format(audio_io_handle_t output) const {
        return AUDIO_FORMAT_DEFAULT;
    }

    virtual size_t frameCount(audio_io_handle_t output) const {
        return 0;
    }

    virtual uint32_t latency(audio_io_handle_t output) const {
        return 0;
    }

    virtual status_t setMasterVolume(float value) {
        return NO_ERROR;
    }
    virtual status_t setMasterMute(bool muted) {
        return NO_ERROR;
    }

    virtual float masterVolume() const {
        return 0;
    }

    virtual bool masterMute() const {
        return false;
    }

    virtual     status_t    setStreamVolume(audio_stream_type_t stream, float value,
                                    audio_io_handle_t output) {
        return NO_ERROR;
    }

    virtual     status_t    setStreamMute(audio_stream_type_t stream, bool muted) {
        return NO_ERROR;
    }

    virtual     float       streamVolume(audio_stream_type_t stream,
                                    audio_io_handle_t output) const {
        return 0;
    }
    virtual     bool        streamMute(audio_stream_type_t stream) const {
        return false;
    }

    virtual     status_t    setMode(audio_mode_t mode) {
        return NO_ERROR;
    }

    virtual     status_t    setMicMute(bool state) {
        return NO_ERROR;
    }

    virtual     bool        getMicMute() const {
        return false;
    }

    virtual     status_t    setParameters(audio_io_handle_t ioHandle,
                                    const String8& keyValuePairs) {
        return gPrivateAfGlue->setParameters(ioHandle, keyValuePairs);
    }

    virtual     String8     getParameters(audio_io_handle_t ioHandle, const String8& keys) const {
        return gPrivateAfGlue->getParameters(ioHandle, keys);
    }

    virtual void registerClient(const sp<IAudioFlingerClient>& client) {}

    virtual size_t getInputBufferSize(uint32_t sampleRate, audio_format_t format,
            audio_channel_mask_t channelMask) const {
        return 0;
    }

    virtual status_t openOutput(audio_module_handle_t module,
                                         audio_devices_t *pDevices,
                                         uint32_t *pSamplingRate,
                                         audio_format_t *pFormat,
                                         audio_channel_mask_t *pChannelMask,
                                         uint32_t *pLatencyMs,
                                         audio_output_flags_t flags,
                                         const audio_offload_info_t *offloadInfo = NULL) {
        return NO_ERROR;
    }

    virtual audio_io_handle_t openDuplicateOutput(audio_io_handle_t output1,
                                    audio_io_handle_t output2) {
        return 0;
    }

    virtual status_t closeOutput(audio_io_handle_t output) {
        return NO_ERROR;
    }

    virtual status_t suspendOutput(audio_io_handle_t output) {
        return NO_ERROR;
    }

    virtual status_t restoreOutput(audio_io_handle_t output) {
        return NO_ERROR;
    }

    virtual status_t openInput(audio_module_handle_t module,
                                        audio_devices_t *pDevices,
                                        uint32_t *pSamplingRate,
                                        audio_format_t *pFormat,
                                        audio_channel_mask_t *pChannelMask) {
        return NO_ERROR;
    }

    virtual status_t closeInput(audio_io_handle_t input) {
        return NO_ERROR;
    }

    virtual status_t setStreamOutput(audio_stream_type_t stream, audio_io_handle_t output) {
        return NO_ERROR;
    }

    virtual status_t setVoiceVolume(float volume) {
        return NO_ERROR;
    }

    virtual status_t getRenderPosition(uint32_t *halFrames, uint32_t *dspFrames,
                                    audio_io_handle_t output) const {
        return NO_ERROR;
    }

    virtual uint32_t getInputFramesLost(audio_io_handle_t ioHandle) const {
        return 0;
    }

    virtual int newAudioSessionId() {
        return 0;
    }

    virtual void acquireAudioSessionId(int audioSession) {}
    virtual void releaseAudioSessionId(int audioSession) {}

    virtual status_t queryNumberEffects(uint32_t *numEffects) const {
        *numEffects = 0;
        return NO_ERROR;
    }

    virtual status_t queryEffect(uint32_t index, effect_descriptor_t *pDescriptor) const {
        return NO_ERROR;
    }

    virtual status_t getEffectDescriptor(const effect_uuid_t *pEffectUUID,
                                        effect_descriptor_t *pDescriptor) const {
        return NO_ERROR;
    }

    virtual sp<IEffect> createEffect(
                                    effect_descriptor_t *pDesc,
                                    const sp<IEffectClient>& client,
                                    int32_t priority,
                                    audio_io_handle_t output,
                                    int sessionId,
                                    status_t *status,
                                    int *id,
                                    int *enabled) {
        return 0;
    }

    virtual status_t moveEffects(int session, audio_io_handle_t srcOutput,
                                    audio_io_handle_t dstOutput) {
        return NO_ERROR;
    }

    virtual audio_module_handle_t loadHwModule(const char *name) {
        return 0;
    }

    virtual uint32_t getPrimaryOutputSamplingRate() {
        return 0;
    }

    virtual size_t getPrimaryOutputFrameCount() {
        return 0;
    }

    virtual status_t setLowRamDevice(bool isLowRamDevice) {
        return NO_ERROR;
    }

};

sp<IPrivateAfGlue> MiniAudioFlinger::gPrivateAfGlue;
