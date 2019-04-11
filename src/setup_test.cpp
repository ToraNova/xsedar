
#include "xsedar.h"
#include <cstdlib>
#include <iostream>
#include <ENCRYPTO_utils/connection.h>

#include "justGarble.h"

#define circuit_filename "justine_aes.gc"

int main(int argc, char *argv[]){

  if(argc<2){
    std::cout << "Please specify sender:1 or receiver:0\n" ;
    return -1;
  }

  //port setup
  std::string addr = "127.0.0.1";
	uint16_t port = 7766;

	//Determines whether the program is executed in the sender or receiver role
	m_nPID = atoi(argv[1]);
	//the number of OTs that are performed. Has to be initialized to a certain minimum size due to
	uint64_t numOTs = 1000000;
	//bitlength of the values that are transferred - NOTE that when bitlength is not 1 or a multiple of 8, the endianness has to be observed
	uint32_t bitlength = 8;

	uint32_t runs = 1;
	uint32_t nsndvals = 2;

	//Use elliptic curve cryptography in the base-OTs
	m_eFType = ECC_FIELD;
	//The symmetric security parameter (80, 112, 128)
	uint32_t m_nSecParam = 128;

	//Number of threads that will be used in OT extension
	m_nNumOTThreads = 1;

	//Specifies which OT flavor should be used
	snd_ot_flavor stype = Snd_OT;
	rec_ot_flavor rtype = Rec_OT;


	m_nBaseOTs = 190;
	m_nChecks = 380;

	m_bUseMinEntCorAssumption = false;

	m_eProt = IKNP;

  crypto *crypt = new crypto(m_nSecParam, (uint8_t*) m_cConstSeed[m_nPID]);

  GarbledCircuit circuit;
  readCircuitFromFile(&circuit, circuit_filename);

	std::cout << "Trial run OK" << std::endl;
	return 0;
}
