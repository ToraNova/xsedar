#ifndef XSEDAR_H
#define XSEDAR_H

#include <ENCRYPTO_utils/typedefs.h>
#include <ENCRYPTO_utils/crypto/crypto.h>
#include <ENCRYPTO_utils/socket.h>
#include <ENCRYPTO_utils/cbitvector.h>
#include <ENCRYPTO_utils/rcvthread.h>
#include <ENCRYPTO_utils/sndthread.h>
#include <ENCRYPTO_utils/channel.h>
#include <ENCRYPTO_utils/timer.h>
#include <ENCRYPTO_utils/parse_options.h>

#include "ot/iknp-ot-ext-snd.h"
#include "ot/iknp-ot-ext-rec.h"
#include "ot/alsz-ot-ext-snd.h"
#include "ot/alsz-ot-ext-rec.h"
#include "ot/nnob-ot-ext-snd.h"
#include "ot/nnob-ot-ext-rec.h"
#include "ot/kk-ot-ext-snd.h"
#include "ot/kk-ot-ext-rec.h"
#include "ot/xormasking.h"

#include <vector>
#include <sys/time.h>
#include <limits.h>
#include <iomanip>
#include <iostream>
#include <memory>
#include <string>

//TODO only for debugging purpose!!
static const char* m_cConstSeed[2] = {"437398417012387813714564100", "15657566154164561"};

uint16_t m_nPort = 7766;
const std::string* m_nAddr;

BOOL Init(crypto* crypt);
BOOL Cleanup();

#endif
