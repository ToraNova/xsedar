#ifndef XSEDAR_H
#define XSEDAR_H

//encrypto utils
#include <ENCRYPTO_utils/connection.h>
#include <ENCRYPTO_utils/socket.h>
#include <ENCRYPTO_utils/timer.h>
#include <ENCRYPTO_utils/typedefs.h>
#include <ENCRYPTO_utils/crypto/crypto.h>
#include <ENCRYPTO_utils/cbitvector.h>
#include <ENCRYPTO_utils/rcvthread.h>
#include <ENCRYPTO_utils/sndthread.h>
#include <ENCRYPTO_utils/channel.h>
#include <ENCRYPTO_utils/parse_options.h>

//pivotal
#include <memory>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>

//custom <toralib>
#include "simplesock.h"


//otextension
#include "ot/iknp-ot-ext-snd.h"
#include "ot/iknp-ot-ext-rec.h"
#include "ot/alsz-ot-ext-snd.h"
#include "ot/alsz-ot-ext-rec.h"
#include "ot/nnob-ot-ext-snd.h"
#include "ot/nnob-ot-ext-rec.h"
#include "ot/kk-ot-ext-snd.h"
#include "ot/kk-ot-ext-rec.h"
#include "ot/xormasking.h"

//TODO only for debugging purpose!!
static const char* m_cConstSeed[2] = {"437398417012387813714564100", "15657566154164561"};

//utils
BOOL Init(crypto* crypt);

#define XSEDAR_SENDER 0
#define XSEDAR_RECEIVER 1

class xsedar{
protected:
  //networking
  std::unique_ptr<CSocket> m_Socket;
  int v_Socket; //vanilla networking (to send garbled table and chosen labels
  int rsock;
  bool vsock_init_flag = false;
  SndThread *sndthread; // the sending thread
  RcvThread *rcvthread; // the receiving thread

  //OT TODO: allow choosing ?
  snd_ot_flavor stype = Snd_OT;
  rec_ot_flavor rtype = Rec_OT;
  ot_ext_prot eProt = IKNP; //IKNP seems to fail
  //possible choice - ALSZ, IKNP, NNOB, KK

  //additional params (secu, OT)
  uint32_t nSecParam = 128;
  uint32_t nBaseOTs = 190;
	uint32_t nChecks = 380;
  uint32_t nThreads = 1;

  //crypto
  field_type eFType = ECC_FIELD;
  bool bUseMinEntCorAssumption = false;

  crypto *mcrypt;
  CLock *glock;

  void start_threads();
  void reset_msock();
  bool create_vsock();
  bool create_rsock();
  bool init_rsock(std::string &address, const int port);
  xsedar();
  xsedar(uint32_t && nPID);

public:

  crypto *getCrypt();
  bool init_vsock(const int port);
  bool vanil_transfer(
    std::string &address,
    const int port,
    char *sbuf,
    uint32_t len,
    short timeout
  );
  bool vanil_receive(
    char *rbuf,
    int32_t *rcount,
    uint32_t maxbufsize,
    short timeout,
    bool fixed = false
  );
};

class xsedar_sender: public xsedar{
private:
  OTExtSnd *otext;       // sender otextension

public:
  xsedar_sender() : xsedar(XSEDAR_SENDER){}; //default constructor
  bool init_msock(std::string &address, const int port);
  bool obliv_transfer(
    CBitVector** X,
    int numOTs,
    int bitlength,
    uint32_t nsndvals,
    MaskingFunction *fMaskFct);

};

class xsedar_receiver: public xsedar{
private:
  OTExtRec *otext;     // sender otextension

public:
  xsedar_receiver(): xsedar(XSEDAR_RECEIVER){}; //default constructor
  bool init_msock(std::string &address, const int port);
  bool obliv_receive(
  	CBitVector* choices,
  	CBitVector* ret,
  	int numOTs,
  	int bitlength,
  	uint32_t nsndvals,
  	MaskingFunction *fMaskFct);
};

#endif
