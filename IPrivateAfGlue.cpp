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
 * Authored by: Juho Hämäläinen <juho.hamalainen@jolla.com>
 */

#define LOG_TAG "IPrivateAfGlue"
#include <android/log.h>

#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#include "IPrivateAfGlue.h"
#include <utils/RefBase.h>
#include <utils/Errors.h>
#include <binder/IInterface.h>
#include <media/AudioParameter.h>
#include <utils/String8.h>
#include <binder/Parcel.h>
#include <system/audio.h>

namespace android {

enum {
    SET_PARAMETERS = IBinder::FIRST_CALL_TRANSACTION,
    GET_PARAMETERS,
    REGISTER_HANDLER
};


class BpPrivateAfGlue : public BpInterface<IPrivateAfGlue>
{
public:
    BpPrivateAfGlue(const sp<IBinder>& impl)
        : BpInterface<IPrivateAfGlue>(impl)
    {
    }

    virtual status_t setParameters(audio_io_handle_t ioHandle, const String8& keyValuePairs)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPrivateAfGlue::getInterfaceDescriptor());
        data.writeInt32((int32_t) ioHandle);
        data.writeString8(keyValuePairs);
        remote()->transact(SET_PARAMETERS, data, &reply);
        return reply.readInt32();
    }


    virtual String8 getParameters(audio_io_handle_t ioHandle, const String8& keys) const
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPrivateAfGlue::getInterfaceDescriptor());
        data.writeInt32((int32_t) ioHandle);
        data.writeString8(keys);
        remote()->transact(GET_PARAMETERS, data, &reply);
        return reply.readString8();
    }

    virtual void registerHandler(const sp<IPrivateAfGlueHandler> &handler)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPrivateAfGlue::getInterfaceDescriptor());
        if (handler == NULL)
            data.writeInt32(0);
        else
            data.writeInt32(1);
#if ANDROID_MAJOR >= 6
        data.writeStrongBinder(IInterface::asBinder(handler));
#else
        data.writeStrongBinder(handler->asBinder());
#endif
        remote()->transact(REGISTER_HANDLER, data, &reply);
    }
};

IMPLEMENT_META_INTERFACE(PrivateAfGlue, "android.media.IPrivateAfGlue");

status_t BnPrivateAfGlue::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case SET_PARAMETERS: {
            CHECK_INTERFACE(IPrivateAfGlue, data, reply);
            audio_io_handle_t ioHandle = (audio_io_handle_t) data.readInt32();
            String8 keyValuePairs(data.readString8());
            reply->writeInt32(setParameters(ioHandle, keyValuePairs));
            return NO_ERROR;
        } break;
        case GET_PARAMETERS: {
            CHECK_INTERFACE(IPrivateAfGlue, data, reply);
            audio_io_handle_t ioHandle = (audio_io_handle_t) data.readInt32();
            String8 keys(data.readString8());
            reply->writeString8(getParameters(ioHandle, keys));
            return NO_ERROR;
        } break;
        case REGISTER_HANDLER: {
            CHECK_INTERFACE(IPrivateAfGlue, data, reply);
            int enable = data.readInt32();
            if (enable) {
                sp<IPrivateAfGlueHandler> handler = interface_cast<IPrivateAfGlueHandler>(
                        data.readStrongBinder());
                registerHandler(handler);
            } else
                registerHandler(0);
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
};

};
