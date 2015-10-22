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

#ifndef DROID_PRIVATE_AF_GLUE_H
#define DROID_PRIVATE_AF_GLUE_H

#include <android/log.h>
#include <utils/String8.h>
#include <binder/IInterface.h>
#include <binder/IBinder.h>
#include <binder/BinderService.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>
#include <media/AudioParameter.h>

#include "common.h"
#include "IPrivateAfGlue.h"
#include "IPrivateAfGlueHandler.h"

namespace android {

class PrivateAfGlue : public BinderService<PrivateAfGlue>,
                      public BnPrivateAfGlue,
                      public IBinder::DeathRecipient
{
public:
    PrivateAfGlue();
    ~PrivateAfGlue();

    static char const *getServiceName() {
        return DROID_AFGLUE_SERVICE_NAME;
    }

    void binderDied(const wp<IBinder>&) {
        // Nothing
    }

    virtual status_t setParameters(audio_io_handle_t ioHandle,
                                   const String8 &keyValuePairs);

    virtual String8 getParameters(audio_io_handle_t ioHandle,
                                  const String8 &keys) const;

    virtual void registerHandler(const sp<IPrivateAfGlueHandler> &handler);

private:
    sp<IPrivateAfGlueHandler> mHandler;
};

};

#endif
