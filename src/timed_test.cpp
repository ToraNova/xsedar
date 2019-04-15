/*

  FULL TEST - test functionality
  Performs AES garbling and using oblivious
  transfers to extract the labels from alice
  timed for performance eval

  Toranova 2019

*/
#include "xsedar.h"
#include <cstdlib>
#include <iostream>

//pivotal
#include "justGarble.h"
#include "jgutil.h"
#include "aes.h"
#include "torabuild.h"

#define circuit_filename0 "justine_aes.gc"
#define circuit_filename1 "simpleAND.gc"

#define loopsize 3000

using namespace std;

int main(int argc, char *argv[]){

  cout << "Setup Test...\n" ;

  if(argc<2){
    cout << "Please specify sender:0 or receiver:others\n" ;
    return -1;
  }

  //networking params
  string addr = "127.0.0.1";
  uint16_t port = 7766; //using port for obliv, port+1 for normal socket send

  clock_t tstart,tstop;
  double elapsed;

  //the number of OTs that are performed. Has to be initialized to a certain minimum size due to
  uint64_t numOTs = 128; //there are 128 labels
  uint32_t runs = 1;
  uint32_t nsndvals = 2; //1 out of 2
  uint32_t bitlength = 128; //each label is 128 bits
  uint32_t num_threads = 1;
  short gTimeout = 10;

  uint32_t nSecParam = 128;
  uint32_t nBaseOTs = 190;
	uint32_t nChecks = 380;
  uint32_t sbatchsize = 100;

  //Determines whether the program is executed in the sender or receiver role
  uint32_t nPID = atoi(argv[1]);

  //loads up a garbled AES circuit
  GarbledCircuit circuit;
  build_JustineAES(&circuit);
  block inputLabels[2 * circuit.n];
  block outputMap[2 * circuit.m]; //JMS: Change from m to 2 * m
  block finalOutput[circuit.m];
  block extractedLabels[circuit.n];
  block *obtainedLabels; //used on the receiver
  int outputVals[circuit.m];
  int inputs[circuit.n]; // NOT USED IN ACTUALITY

  //AES functionality
  AES_KEY aeskey;
  int aesKeyInput[circuit.n - 128]; //11 rounds of ARK
  block aesKeyLabel[circuit.n - 128]; //128 label, one for each bit
  unsigned char outputAES[16];
  int outputAESbit[128];

  BYTE ai[16] = {
    0x33, 0x82, 0xBD, 0x56, 0x01, 0xF4, 0xC5, 0x1A,
    0x9F, 0xF3, 0x8B, 0x7A, 0x8F, 0xA9, 0xE5, 0x32
  };

  BYTE bi[16] = {
    0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37,
    0x38, 0x39, 0x41, 0x42, 0x43, 0x44, 0x45, 0x01
  };

  ot_ext_prot eProt = ALSZ; //IKNP seems to fail
  field_type eFType = ECC_FIELD;
  MaskingFunction *fMaskFct;

  if(nPID == SERVER_ID){
    tstart = clock();

    cout << "Setting up as the oblivious transfer sender (Alice)..." << endl;
    xsedar_sender xs = xsedar_sender();
    xs.init_msock(addr,port); //ot main socket init
    cout << "Garbled Circuit Information, Gatecount :" << circuit.q << endl;
    cout << "Sender setup successful...beginning loop" << endl;

    for(uint32_t loop =0;loop<loopsize;loop++){
      /*
        Key setup procedure
      */
      memset(&aeskey,0,sizeof(AES_KEY));
      AES_set_encrypt_key(ai,128,&aeskey);
      unsigned char* blob = (unsigned char *) aeskey.rd_key;
      make_uint_array_from_blob(aesKeyInput, blob,circuit.n/8-16);
      /*
        The key will extract the input Labels later.
      */

      //the program functions as the sender
      /*
        Performs garbling
      */
      garbleCircuit(&circuit, inputLabels, outputMap);

      for(uint32_t i=0;i<circuit.n - 128;i++){
        //go through the key
        if(aesKeyInput[i]){
          aesKeyLabel[i] = inputLabels[256 + 2*i+1];//skip the first 128bit labels, which is 256 labels
        }else{
          aesKeyLabel[i] = inputLabels[256 + 2*i];
        }
      }
      //aesKeyLabel can be sent over to bob
      //bob must perform OT to obtain first 128bits withou alice knowing
      //his choice. then he must form a circuit.n size extractedLabel
      //and perform evaluation
      /*
        Extract the inputLabels using the garbled circuit
      */

      //sends the garbled table over
      //for everygate there is a garbled table


      xs.vanil_transfer( addr,port+1, (char *)(circuit.garbledTable), 4*sizeof(block)*circuit.q,gTimeout);
      //send the global key
      xs.vanil_transfer(addr,port+1, (char *)&circuit.globalKey, sizeof(block),gTimeout);
      //send the OutputMap for bob to extract
      xs.vanil_transfer(addr,port+1, (char *)outputMap, 2*sizeof(block)*circuit.m, gTimeout);


      CBitVector delta;
      CBitVector** X = (CBitVector**) malloc(sizeof(CBitVector*) * nsndvals); //allocate memory for the target
      //The masking function with which the values that are sent in the last communication step are processed
      fMaskFct = new XORMasking(bitlength, delta);
      //creates delta as an array with "numOTs" entries of "bitlength" bit-values and fills delta with random values
      delta.Create(numOTs, bitlength, xs.getCrypt());
      //Create the X values as two arrays with "numOTs" entries of "bitlength" bit-values and resets them to 0
      for(uint32_t i = 0; i < nsndvals; i++) {
  			X[i] = new CBitVector();
  			X[i]->Create(numOTs, bitlength);
        for(uint32_t j = 0; j < numOTs; j++){
          X[i]->Set(inputLabels[2*j+i],j); //or 1-i
        }
  		}

      //sends out
      for(uint32_t i = 0; i < runs; i++) {
        xs.obliv_transfer(
          X, numOTs, bitlength, nsndvals, fMaskFct
        );
  		}

      //send alice's input label (AES KEY)
      xs.vanil_transfer(addr,port+1, (char *)aesKeyLabel, sizeof(block)*(circuit.n - 128), gTimeout);

      //***********************************************************
      // The rest down here is done for checking purposes
      //***********************************************************
      setup_AESInput(inputs, bi, ai, &aeskey, circuit.n);
      extractLabels(extractedLabels, inputLabels, inputs, circuit.n);
      evaluate(&circuit, extractedLabels, finalOutput);
      memset(outputVals, 0, sizeof(int) * circuit.m);
      mapOutputs(outputMap, finalOutput, outputVals, circuit.m);
      //visual check
      /*
      cout << "Garbled Evaluation :" << endl;
      for(uint32_t x = 0; x < 128; x++) cout << outputVals[x];
      cout << endl;
      */
      //comparison with vanilla AES
      setup_AESInput(inputs,bi,ai,&aeskey,circuit.n);
      AES_encrypt(bi, outputAES, &aeskey);
      make_uint_array_from_blob(outputAESbit, outputAES, 16);
      /*
      cout << "Vanilla AES evaluation :" << endl;
      for(uint32_t x = 0; x < 128; x++) cout << outputAESbit[x];
      cout << endl;
      */
    };

    tstop = clock();
    elapsed = (double)(tstop-tstart)/CLOCKS_PER_SEC * 1000.0;

  }else{
    tstart = clock();

    obtainedLabels = (block *) malloc(sizeof(block)*circuit.n);
    cout << "Setting up as the oblivous transfer receiver (Bob)" << endl;
    xsedar_receiver xr = xsedar_receiver();
    xr.init_msock(addr,port); //ot main socket init
    while(! xr.init_vsock(port+1)){} //run until vsock available
    cout << "Garbled Circuit Information, Gatecount :" << circuit.q << endl;
    cout << "Receiver setup successful...beginning loop" << endl;

    for(uint32_t loop=0;loop<loopsize;loop++){

      int32_t tr=0,tt;
      //the program functions as the receiver

      //creates the memory to receive the garbled table
      //obtains the garbled table

      xr.vanil_receive( (char *)(circuit.garbledTable), &tt,4*sizeof(block)*circuit.q,gTimeout,true);

      //obtains globaly key
      xr.vanil_receive( (char *) &circuit.globalKey, &tt, sizeof(block), gTimeout,true);

      //obtains the outputMap
      xr.vanil_receive( (char *) outputMap,&tt, 2*sizeof(block)*circuit.m,gTimeout,true);


      //visual inspection************************
      //printblock_arr((block *)circuit.garbledTable,circuit.q);
      //****************************************

      CBitVector choices, response;
      //The masking function with which the values that are sent in the last communication step are processed
      fMaskFct = new XORMasking(bitlength);
      //Create the bitvector choices as a bitvector with numOTs entries
  		//choices.Create(numOTs * ceil_log2(nsndvals), xr.getCrypt()); //create random rules
      choices.CreateBytes(16);
      choices.SetBytes(bi,0,16);
      //Pre-generate the respose vector for the results
  		response.Create(numOTs, bitlength);
  		response.Reset();

      //receives
      for(uint32_t i = 0; i < runs; i++) {
        xr.obliv_receive(
          &choices, &response, numOTs, bitlength, nsndvals, fMaskFct
        );
  		}

      //response.PrintHex();
      obtainedLabels = (block *) response.GetArr(); //TODO: check out what problem occured here
      //obtains the input labels from alice
      xr.vanil_receive( (char *) (obtainedLabels+128),&tt, sizeof(block)*(circuit.n-128),gTimeout,true);

      //evaluation
      evaluate(&circuit, obtainedLabels, finalOutput);
      memset(outputVals, 0, sizeof(int) * circuit.m);
      mapOutputs(outputMap, finalOutput, outputVals, circuit.m);

      //visual check
      /*
      cout << "Garbled Evaluation :" << endl;
      for(uint32_t x = 0; x < 128; x++) cout << outputVals[x];
      cout << endl;
      */
    };
    xr.close_vsock();
    tstop = clock();
    elapsed = (double)(tstop-tstart)/CLOCKS_PER_SEC * 1000.0;
  }

  // CLEANUP routine
  cout << "Total elapsed :" << elapsed << endl;
	return 0;
}

/*
    Test status
    obliv-transfer labels - OK; obtainedLabels and extractedLabels are identical
    global key are identical
    outputMap are identical

*/
