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

#define LOG_TAG "AfGlue"
#include <android/log.h>

#include "audioflingerglue.h"
#include <utils/String8.h>
#include <binder/IBinder.h>
#include <binder/BinderService.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <media/AudioParameter.h>
#include "IPrivateAfGlue.h"
#include "IPrivateAfGlueHandler.h"
#include "common.h"

using namespace android;


extern "C" {

struct _DroidAfGlue
{
    _DroidAfGlue() :
        m_cb_data(0)
    {
        memset(&m_cb, 0x0, sizeof(m_cb));
    }

    sp<IPrivateAfGlue> m_af;
    DroidAfGlueCallbacks m_cb;
    void *m_cb_data;
};

class GlueHandler : public BinderService<GlueHandler>,
                    public BnPrivateAfGlueHandler
{
public:
    GlueHandler(DroidAfGlue *glue) :
        BnPrivateAfGlueHandler(),
        m_glue(glue)
    {
    }

    ~GlueHandler()
    {
    }

    virtual status_t handleSetParameters(audio_io_handle_t ioHandle,
                                                  const String8 &keyValuePairs)
    {
        if (!m_glue)
            return NO_ERROR;

        return m_glue->m_cb.set_parameters(keyValuePairs.string(), m_glue->m_cb_data);
    }

    virtual String8 handleGetParameters(audio_io_handle_t ioHandle,
                                                 const String8 &keys) const
    {
        if (!m_glue)
            return String8("");

        char *r = 0;
        if (m_glue->m_cb.get_parameters(keys.string(), &r, m_glue->m_cb_data) == NO_ERROR) {
            String8 reply(r);
            free(r);
            return reply;
        }

        return String8("");
    }

private:
    DroidAfGlue *m_glue;
};

DroidAfGlue *droid_afglue_connect(DroidAfGlueCallbacks *cb, void *userdata)
{
    sp<IPrivateAfGlue> af;
    sp<IServiceManager> sm = defaultServiceManager();
    sp<IBinder> binder;

    binder = sm->getService(String16(DROID_AFGLUE_SERVICE_NAME));
    if (binder == 0) {
        ALOGE("Couldn't find %s service", DROID_AFGLUE_SERVICE_NAME);
        return NULL;
    }
    af = interface_cast<IPrivateAfGlue>(binder);

    DroidAfGlue *glue = new DroidAfGlue;

    if (!glue) {
        ALOGE("Failed to allocate DroidAfGlue");
        return NULL;
    }

    memcpy(&glue->m_cb, cb, sizeof(glue->m_cb));
    glue->m_cb_data = userdata;

    glue->m_af = af;
    glue->m_af->registerHandler(new GlueHandler(glue));

    ProcessState::self()->startThreadPool();

    return glue;
}

void droid_afglue_disconnect(DroidAfGlue *glue)
{
    glue->m_af->registerHandler(0);
    delete glue;
}

};
