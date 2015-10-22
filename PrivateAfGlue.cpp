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

#define LOG_TAG "PrivateAfGlue"
#include <android/log.h>

#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <media/IAudioFlinger.h>
#include <media/AudioParameter.h>
#include "PrivateAfGlue.h"
#include "IPrivateAfGlueHandler.h"

namespace android {

PrivateAfGlue::PrivateAfGlue() : BnPrivateAfGlue()
{
}

PrivateAfGlue::~PrivateAfGlue()
{
}

status_t PrivateAfGlue::setParameters(audio_io_handle_t ioHandle,
                               const String8 &keyValuePairs)
{
    if (mHandler == NULL) {
        ALOGW("No handler for setParameters.");
        return NO_ERROR;
    }

    return mHandler->handleSetParameters(ioHandle, keyValuePairs);
}

String8 PrivateAfGlue::getParameters(audio_io_handle_t ioHandle,
                              const String8 &keys) const
{
    if (mHandler == NULL) {
        ALOGW("No handler for getParameters.");
        return String8("");
    }

    return mHandler->handleGetParameters(ioHandle, keys);
}

void PrivateAfGlue::registerHandler(const sp<IPrivateAfGlueHandler> &handler)
{
    ALOGD("%s handler", handler == NULL ? "Unregister" : "Register");
    mHandler = handler;
}

};
