/*
	Project for oblivious transfer OTextension
	with justGarble
	ToraNova 2019
*/
#include "xsedar.h"
#include <cstdlib>
#include <iostream>

BOOL Init(crypto* crypt)
{
	return TRUE;
}

// Base methods for XSEDAR
xsedar::xsedar(uint32_t&& nPID){
	//xsedar base constructor, initialize crypto objects
	mcrypt = new crypto(nSecParam, (uint8_t*) m_cConstSeed[nPID]);
  glock = new CLock(); // pass this to sender and receiver constructors
}

void xsedar::start_threads(){
	//begin the snd and rcv threads for Oblivious transfers.
	sndthread = new SndThread(m_Socket.get(), glock);
	rcvthread = new RcvThread(m_Socket.get(), glock);
	rcvthread->Start();
	sndthread->Start();
}

void xsedar::reset_sock(){
	//perform a reset count on the socket
	m_Socket->ResetSndCnt();
	m_Socket->ResetRcvCnt();
}

crypto *xsedar::getCrypt(){
	return mcrypt;
}

//Overidden methods
//
//#####################################################################
//			SENDER
//#####################################################################
void xsedar_sender::initialize(std::string &address, const int port){
	m_Socket = Listen(address,port); // begin listening (server)

	if (!m_Socket) {
		std::cerr << "Connect failed on " << address << ":" << port << "\n";
		std::exit(1); //error occured here
	}
	start_threads(); //begin sending and receiving threads
	switch(eProt) {
		//setup the sender
		case ALSZ: otext = new ALSZOTExtSnd(mcrypt, rcvthread, sndthread, nBaseOTs, nChecks); break;
		case IKNP: otext = new IKNPOTExtSnd(mcrypt, rcvthread, sndthread); break;
		case NNOB: otext = new NNOBOTExtSnd(mcrypt, rcvthread, sndthread); break;
		case KK: otext = new KKOTExtSnd(mcrypt, rcvthread, sndthread); break;
		default: otext = new ALSZOTExtSnd(mcrypt, rcvthread, sndthread, nBaseOTs, nChecks); break;
	}
	if(bUseMinEntCorAssumption)
		otext->EnableMinEntCorrRobustness();
	otext->ComputeBaseOTs(eFType); // TODO: find out why program stucked here
}

bool xsedar_sender::obliv_transfer(
	CBitVector** X,
	int numOTs,
	int bitlength,
	uint32_t nsndvals,
	MaskingFunction *fMaskFct)
{
	bool success = false;
	reset_sock();
	timespec ot_begin, ot_end;
	double rndgentime;
	std::cout<<"."<<std::endl;
	clock_gettime(CLOCK_MONOTONIC, &ot_begin);
	// Execute OT sender routine
	success = otext->send(numOTs, bitlength, nsndvals, X, stype, rtype, nThreads, fMaskFct);
	clock_gettime(CLOCK_MONOTONIC, &ot_end);
	std::cout << "Send success :" << success << std::endl;
	std::cout << getMillies(ot_begin, ot_end) + rndgentime << "\t"
	<< m_Socket->getSndCnt() << "\t" << m_Socket->getRcvCnt() << std::endl;
	return success;
}


//#####################################################################
//			RECEIVERS
//#####################################################################
void xsedar_receiver::initialize(std::string &address, const int port){
	m_Socket = Connect(address,port); // begin connecting

	if (!m_Socket) {
		std::cerr << "Listen failed on " << address << ":" << port << "\n";
		std::exit(1); //error occured here
	}
	start_threads(); //begin sending and receiving threads
	switch(eProt) {
		//setup the receiver
		case ALSZ: otext = new ALSZOTExtRec(mcrypt, rcvthread, sndthread, nBaseOTs, nChecks); break;
		case IKNP: otext = new IKNPOTExtRec(mcrypt, rcvthread, sndthread); break;
		case NNOB: otext = new NNOBOTExtRec(mcrypt, rcvthread, sndthread); break;
		case KK: otext = new KKOTExtRec(mcrypt, rcvthread, sndthread); break;
		default: otext = new ALSZOTExtRec(mcrypt, rcvthread, sndthread, nBaseOTs, nChecks); break;
	}
	if(bUseMinEntCorAssumption)
		otext->EnableMinEntCorrRobustness();
	otext->ComputeBaseOTs(eFType);
}

bool xsedar_receiver::obliv_transfer(
	CBitVector* choices,
	CBitVector* ret,
	int numOTs,
	int bitlength,
	uint32_t nsndvals,
	MaskingFunction *fMaskFct)
{
	bool success = false;
	reset_sock();
	timespec ot_begin, ot_end;
	double rndgentime;
	std::cout<<"."<<std::endl;
	clock_gettime(CLOCK_MONOTONIC, &ot_begin);
	// Execute OT sender routine
	success = otext->receive(numOTs, bitlength, nsndvals, choices, ret,
		stype, rtype, nThreads, fMaskFct);
	clock_gettime(CLOCK_MONOTONIC, &ot_end);
	std::cout << "Recv success :" << success << std::endl;
	std::cout << getMillies(ot_begin, ot_end) + rndgentime << "\t"
	<< m_Socket->getSndCnt() << "\t" << m_Socket->getRcvCnt() << std::endl;
	return success;
}
