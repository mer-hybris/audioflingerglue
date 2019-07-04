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
 * Authored by: Mohammed Hassan <mohammed.hassan@jolla.com>
 *              Juho Hämäläinen <juho.hamalainen@jolla.com>
 */

#include <binder/IPCThreadState.h>
#include <binder/ProcessState.h>
#include <binder/IServiceManager.h>
#include <binder/BinderService.h>
#include <media/IAudioFlinger.h>
#include <binder/IPermissionController.h>
#include <binder/MemoryHeapBase.h>
#include "PrivateAfGlue.h"

#ifdef USE_SERVICES_VENDOR_EXTENSION

#if ANDROID_MAJOR == 5 && ANDROID_MINOR == 1
#include "services/audioflinger_5_1_0_custom.h"
#elif ANDROID_MAJOR == 7 && ANDROID_MINOR == 0
#include "services/audioflinger_7_0_0_custom.h"
#elif ANDROID_MAJOR == 7 && ANDROID_MINOR == 1
#include "services/audioflinger_7_1_0_custom.h"
#elif ANDROID_MAJOR == 6 && ANDROID_MINOR == 0
#include "services/audioflinger_6_0_0_custom.h"
#else
#error Unsupported Android version.
#endif

#else

#if ANDROID_MAJOR == 4 && ANDROID_MINOR == 4
#include "services/audioflinger_4_4_0.h"
#elif ANDROID_MAJOR == 5 && ANDROID_MINOR == 1
#include "services/audioflinger_5_1_0.h"
#elif ANDROID_MAJOR == 6 && ANDROID_MINOR == 0
#include "services/audioflinger_6_0_0.h"
#elif ANDROID_MAJOR == 7 && ANDROID_MINOR == 0
#include "services/audioflinger_7_0_0.h"
#elif ANDROID_MAJOR == 7 && ANDROID_MINOR == 1
#include "services/audioflinger_7_1_0.h"
#elif ANDROID_MAJOR == 8
#include "services/audioflinger_8_0_0.h"
#elif ANDROID_MAJOR == 9
#include "services/audioflinger_9_0_0.h"
#else
#error Unsupported Android version ANDROID_MAJOR ANDROID_MINOR .
#endif

#endif

using namespace android;


int main(int, char**)
{
    sp<ProcessState> proc(ProcessState::self());
    sp<IServiceManager> sm = defaultServiceManager();

    PrivateAfGlue::instantiate();
    MiniAudioFlinger::instantiate();

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

    return 0;
}
