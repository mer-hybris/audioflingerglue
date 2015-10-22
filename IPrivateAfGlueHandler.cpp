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


#define LOG_TAG "IPrivateAfGlueHandler"
#include <android/log.h>

#include <stdint.h>
#include <sys/types.h>

#include <binder/Parcel.h>
#include <binder/IInterface.h>
#include <media/AudioParameter.h>
#include <utils/String8.h>

#include "IPrivateAfGlueHandler.h"

namespace android {

enum {
    HANDLE_SET_PARAMETERS = IBinder::FIRST_CALL_TRANSACTION,
    HANDLE_GET_PARAMETERS
};

class BpPrivateAfGlueHandler : public BpInterface<IPrivateAfGlueHandler>
{
public:
    BpPrivateAfGlueHandler(const sp<IBinder>& impl)
        : BpInterface<IPrivateAfGlueHandler>(impl)
    {
    }

    virtual status_t handleSetParameters(audio_io_handle_t ioHandle, const String8& keyValuePairs)
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPrivateAfGlueHandler::getInterfaceDescriptor());
        data.writeInt32((int32_t) ioHandle);
        data.writeString8(keyValuePairs);
        remote()->transact(HANDLE_SET_PARAMETERS, data, &reply);
        return reply.readInt32();
    }


    virtual String8 handleGetParameters(audio_io_handle_t ioHandle, const String8& keys) const
    {
        Parcel data, reply;
        data.writeInterfaceToken(IPrivateAfGlueHandler::getInterfaceDescriptor());
        data.writeInt32((int32_t) ioHandle);
        data.writeString8(keys);
        remote()->transact(HANDLE_GET_PARAMETERS, data, &reply);
        return reply.readString8();
    }
};

IMPLEMENT_META_INTERFACE(PrivateAfGlueHandler, "android.media.IPrivateAfGlueHandler");

status_t BnPrivateAfGlueHandler::onTransact(
    uint32_t code, const Parcel& data, Parcel* reply, uint32_t flags)
{
    switch (code) {
        case HANDLE_SET_PARAMETERS: {
            CHECK_INTERFACE(IPrivateAfGlueHandler, data, reply);
            audio_io_handle_t ioHandle = (audio_io_handle_t) data.readInt32();
            String8 keyValuePairs(data.readString8());
            reply->writeInt32(handleSetParameters(ioHandle, keyValuePairs));
            return NO_ERROR;
        } break;
        case HANDLE_GET_PARAMETERS: {
            CHECK_INTERFACE(IPrivateAfGlueHandler, data, reply);
            audio_io_handle_t ioHandle = (audio_io_handle_t) data.readInt32();
            String8 keys(data.readString8());
            reply->writeString8(handleGetParameters(ioHandle, keys));
            return NO_ERROR;
        } break;
        default:
            return BBinder::onTransact(code, data, reply, flags);
    }
};

};
