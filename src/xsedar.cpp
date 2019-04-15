/*
	Project for oblivious transfer OTextension
	with justGarble
	ToraNova 2019
*/
#include "xsedar.h"
#include <cstdlib>
#include <iostream>
#include "simplesock.h"

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

void xsedar::reset_msock(){
	//perform a reset count on the socket
	m_Socket->ResetSndCnt();
	m_Socket->ResetRcvCnt();
}

crypto *xsedar::getCrypt(){
	return mcrypt;
}

bool xsedar::create_vsock(){
	//create the vsock object (initialization)
	v_Socket = sockgen(0);
	if(v_Socket < 0){
		std::cerr << "Failed to create vsocket.\n";
		return false; //error has occurred
	}
	return true;
}

bool xsedar::create_rsock(){
	rsock = sockgen(0);
	if(rsock<0){
		std::cerr << "Failed to create rsocket.\n";
		return false;
	}
	return true;
}

bool xsedar::init_rsock(std::string &address, const int port){
	//initialize rsock as a connector (addr and port)
	//rsock it meant to be temporary
	if(! create_rsock())return false; //create the socket
	if( sockconn(rsock, address.c_str() ,port) < 0){
		std::cerr << "Failed to connect to " << address <<":"<<port<<"\n";
		return false; //error has occured
	}
	return true;
}

bool xsedar::init_vsock(const int port){
	//initialize vsock as a listener (no addr needed)
	if(! create_vsock())return false; //create the socket
	if( sockbind(v_Socket,port) < 0){
		std::cerr << "Failed to bind socket on port "<<port<<"\n";
		return false; //error has occurred
	}
	listen(v_Socket,3); //listen to at most 3 incoming connection
	vsock_init_flag = true;
	return true;
}

bool xsedar::close_vsock(){
	//closes the vanilla listening socket
	return close(v_Socket);
}

bool xsedar::vanil_transfer(
	std::string &address,
	const int port,
	char *sbuf,
	uint32_t len,
	short timeout
)
{
	/* performs vanilla transfer (default socket send)
	on the socket rsock. uses an ephemeral socket
	The sender will only need to call this function!
	*/
	if(! init_rsock(address,port))return false;
	if(sendbuf(rsock,sbuf,len,timeout) < 0){
		std::cerr << "sendbuf failed\n";
	}
	if(close(rsock)==0) return true;
	else return false;
}

bool xsedar::vanil_receive(
	char *rbuf,
	int32_t *rcount,
	uint32_t maxbufsize,
	short timeout,
	bool fixed
)
{
	//receiver must first call init_vsock first before
	//calling this fucntion
	if(! vsock_init_flag){
		std::cerr << "v_Socket uninitialized for listening.\n";
		return false;
	}
	//setup address structure
	struct sockaddr_in caddr;
	int caddr_len = sizeof(struct sockaddr_in);
	//blocking, accept incoming connection
	rsock = accept( v_Socket, (struct sockaddr *)&caddr, (socklen_t*)&caddr_len);
	if(rsock < 0){
		std::cerr << "Failed to accept incoming connection.\n";
		return false;
	}
	//reset the receive buffer
	memset(rbuf,'\0',maxbufsize);
	if(fixed){
		*rcount = fixed_recvbuf(rsock, rbuf, maxbufsize,timeout);
	}else{
		*rcount = recvbuf(rsock, rbuf, maxbufsize,timeout);
	}

	if( *rcount < 0){
		std::cerr << "Failed to receive from socket.\n";
		return false;
	}
	//close the socket
	if(close(rsock)==0) return true;
	else return false;
}

//Overidden methods
//
//#####################################################################
//			SENDER
//#####################################################################
bool xsedar_sender::init_msock(std::string &address, const int port){
	m_Socket = Listen(address,port); // begin listening (server)

	if (!m_Socket) {
		std::cerr << "Connect failed on " << address << ":" << port << "\n";
		return false;//error occured here
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
	return true;
}

bool xsedar_sender::obliv_transfer(
	CBitVector** X,
	int numOTs,
	int bitlength,
	uint32_t nsndvals,
	MaskingFunction *fMaskFct)
{
	/*
	Performs oblivious transfer (sending) via the m_Socket
	connection on 2 threads
	*/
	bool success = false;
	reset_msock();
	timespec ot_begin, ot_end;
	double rndgentime;
	//std::cout<<".";
	clock_gettime(CLOCK_MONOTONIC, &ot_begin);
	// Execute OT sender routine
	success = otext->send(numOTs, bitlength, nsndvals, X, stype, rtype, nThreads, fMaskFct);
	clock_gettime(CLOCK_MONOTONIC, &ot_end);

	/*
	std::cout << "Send success :" << success << std::endl;
	std::cout << getMillies(ot_begin, ot_end) + rndgentime << "\t"
	<< m_Socket->getSndCnt() << "\t" << m_Socket->getRcvCnt() << std::endl;
	*/
	return success;
}


//#####################################################################
//			RECEIVERS
//#####################################################################
bool xsedar_receiver::init_msock(std::string &address, const int port){
	m_Socket = Connect(address,port); // begin connecting

	if (!m_Socket) {
		std::cerr << "Listen failed on " << address << ":" << port << "\n";
		return false; //error has occurred
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
	return true;
}

bool xsedar_receiver::obliv_receive(
	CBitVector* choices,
	CBitVector* ret,
	int numOTs,
	int bitlength,
	uint32_t nsndvals,
	MaskingFunction *fMaskFct)
{
	bool success = false;
	reset_msock();
	timespec ot_begin, ot_end;
	double rndgentime;
	//std::cout<<".";
	clock_gettime(CLOCK_MONOTONIC, &ot_begin);
	// Execute OT sender routine
	success = otext->receive(numOTs, bitlength, nsndvals, choices, ret,
		stype, rtype, nThreads, fMaskFct);
	clock_gettime(CLOCK_MONOTONIC, &ot_end);
	/*
	std::cout << "Recv success :" << success << std::endl;
	std::cout << getMillies(ot_begin, ot_end) + rndgentime << "\t"
	<< m_Socket->getSndCnt() << "\t" << m_Socket->getRcvCnt() << std::endl;
	*/
	return success;
}
