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

#ifndef DROID_AFGLUE_H
#define DROID_AFGLUE_H

#include <sys/types.h>
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _DroidAfGlue DroidAfGlue;

typedef struct {
    int (*get_parameters)(const char *key, char **reply, void *userdata);
    int (*set_parameters)(const char *key_value_pairs, void *userdata);
} DroidAfGlueCallbacks;

DroidAfGlue *droid_afglue_connect(DroidAfGlueCallbacks *cb, void *userdata);
void droid_afglue_disconnect(DroidAfGlue *glue);

#ifdef __cplusplus
};
#endif

#endif /* DROID_AFGLUE_H */
