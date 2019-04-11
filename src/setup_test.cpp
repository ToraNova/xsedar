/*

  SETUP TEST - test functionality

  Toranova 2019

*/
#include "xsedar.h"
#include <cstdlib>
#include <iostream>

//pivotal
#include "justGarble.h"


#define circuit_filename "justine_aes.gc"

using namespace std;

int main(int argc, char *argv[]){

  cout << "Setup Test...\n" ;

  if(argc<2){
    cout << "Please specify sender:0 or receiver:others\n" ;
    return -1;
  }

  //networking params
  string addr = "127.0.0.1";
  uint16_t port = 7766;

  //the number of OTs that are performed. Has to be initialized to a certain minimum size due to
  uint64_t numOTs = 1000000;
  uint32_t runs = 1;
  uint32_t nsndvals = 2;
  uint32_t bitlength = 8;
  uint32_t num_threads = 1;

  uint32_t nSecParam = 128;
  uint32_t nBaseOTs = 190;
	uint32_t nChecks = 380;

  //Determines whether the program is executed in the sender or receiver role
  uint32_t nPID = atoi(argv[1]);

  //loads up a garbled AES circuit
  GarbledCircuit circuit;
  readCircuitFromFile(&circuit, circuit_filename);
  block inputLabels[2 * circuit.n];
  block outputMap[2 * circuit.m]; //JMS: Change from m to 2 * m
  block finalOutput[circuit.m];
  block extractedLabels[circuit.n];
  int outputVals[circuit.m];

  ot_ext_prot eProt = ALSZ; //IKNP seems to fail
  field_type eFType = ECC_FIELD;
  MaskingFunction *fMaskFct;

  if(nPID == SERVER_ID){

    //the program functions as the sender
    cout << "Performing Garbling" <<endl;
    garbleCircuit(&circuit, inputLabels, outputMap);

    cout << "Setting up as the oblivious transfer sender (Alice)..." << endl;
    xsedar_sender xs = xsedar_sender();
    xs.initialize(addr,port);
    cout << "Sender setup successful..." << endl;

    CBitVector delta;
    CBitVector** X = (CBitVector**) malloc(sizeof(CBitVector*) * nsndvals); //allocate memory for the target
    cout << "Initiating Oblivious Send" << endl;

    //The masking function with which the values that are sent in the last communication step are processed
    fMaskFct = new XORMasking(bitlength, delta);
    //creates delta as an array with "numOTs" entries of "bitlength" bit-values and fills delta with random values
    delta.Create(numOTs, bitlength, xs.getCrypt());

    //Create the X values as two arrays with "numOTs" entries of "bitlength" bit-values and resets them to 0
    for(uint32_t i = 0; i < nsndvals; i++) {
			X[i] = new CBitVector();
			X[i]->Create(numOTs, bitlength);
		}

    cout << "Sending bits obliviously" << endl;
    //sends out
    for(uint32_t i = 0; i < runs; i++) {
      xs.obliv_transfer(
        X, numOTs, bitlength, nsndvals, fMaskFct
      );
		}


  }else{

    //the program functions as the receiver
    cout << "Setting up as the oblivous transfer receiver (Bob)" << endl;
    xsedar_receiver xr = xsedar_receiver();
    xr.initialize(addr,port);
    cout << "Receiver setup successful..." << endl;

    CBitVector choices, response;
    cout << "Initiating Oblivious Receive" << endl;
    //The masking function with which the values that are sent in the last communication step are processed
    fMaskFct = new XORMasking(bitlength);
    //Create the bitvector choices as a bitvector with numOTs entries
		choices.Create(numOTs * ceil_log2(nsndvals), xr.getCrypt());
    //Pre-generate the respose vector for the results
		response.Create(numOTs, bitlength);
		response.Reset();

    cout << "Receiving bits obliviously" << endl;
    //receives
    for(uint32_t i = 0; i < runs; i++) {
      xr.obliv_transfer(
        &choices, &response, numOTs, bitlength, nsndvals, fMaskFct
      );
		}
  }

  // CLEANUP routine

	std::cout << "Trial run OK" << std::endl;
	return 0;
}
