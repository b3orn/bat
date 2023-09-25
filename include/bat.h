#ifndef BAT_H
#define BAT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <clap/clap.h>

#include "bat/version.h"
#include "bat/types.h"
#include "bat/consts.h"
#include "bat/utils.h"
#include "bat/inst.h"
#include "bat/note.h"
#include "bat/osc.h"
#include "bat/filter.h"
#include "bat/env.h"
#include "bat/lfo.h"
#include "bat/mod.h"
#include "bat/wav.h"
#include "bat/mid.h"
#include "bat/plugin/entry.h"
#include "bat/plugin/factory.h"
#include "bat/plugin/plugin.h"
#include "bat/plugin/params.h"
#include "bat/plugin/state.h"
#include "bat/plugin/gui.h"

#ifdef __cplusplus
}
#endif

#endif
