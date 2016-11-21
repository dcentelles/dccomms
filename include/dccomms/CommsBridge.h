/*
 * DLinkInterface.h
 *
 *  Created on: 22 oct. 2016
 *      Author: centelld
 */

#ifndef COMMSTCP_H_
#define COMMSTCP_H_

#include <dccomms/CommsDeviceService.h>
#include <dccomms/ICommsDevice.h>
#include <pthread.h>
#include <iostream>
#include <dccomms/Utils.h>
#include <string>
#include <mutex>
#include <Loggable.h>

namespace dccomms {

#define MAX_ODATA_BUF 4096

using namespace dccomms;

class CommsBridge: public Loggable {
public:
	CommsBridge(ICommsDevice *, int _baudrate = 2000, DataLinkFrame::fcsType _chksum = DataLinkFrame::fcsType::crc32);
	virtual ~CommsBridge();
	virtual void Start();
	virtual void Stop();

	//Two instances of CommsBridge for the same purpose in the same machine (for debug reasons) must have different namespaces
	//This method must be called before Start
	void SetNamespace(std::string nspace);

protected:
	virtual void TxWork();
	virtual void RxWork();
	virtual void TransmitFrame();
	virtual bool ReceiveFrame();
	virtual bool TryToConnect();
	virtual bool TryToReconnect();
	Timer timer;
	unsigned int _frameTransmissionTime; //milis
	double _byteTransmissionTime; //milis

	std::string serv_namespace;
	CommsDeviceService phyService;
	DataLinkFramePtr txdlf;
	DataLinkFramePtr rxdlf;

	uint8_t obuf[MAX_ODATA_BUF];

	mutex devicemutex;
	bool connected;
	bool transcurridoTiempoEnvio;
	int baudrate;
	ICommsDevice * device;

	ServiceThread<CommsBridge> txserv, rxserv;

};
} /* namespace dcent */

#endif /* COMMSTCP_H_ */