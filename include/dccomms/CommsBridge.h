/*
 * DLinkInterface.h
 *
 *  Created on: 22 oct. 2016
 *      Author: centelld
 */

#ifndef DCCOMMS_COMMSBRIDGE_H_
#define DCCOMMS_COMMSBRIDGE_H_

#include <cpplogging/Loggable.h>
#include <dccomms/CommsDeviceService.h>
#include <dccomms/ICommsDevice.h>
#include <dccomms/TransportPDU.h>
#include <dccomms/Utils.h>
#include <iostream>
#include <mutex>
#include <pthread.h>
#include <string>

namespace dccomms {

#define MAX_ODATA_BUF 4096

using namespace dccomms;

class CommsBridge : public virtual Loggable {
public:
  CommsBridge(ICommsDevice *, PacketBuilderPtr packetBuilder,
              int _baudrate = 2000);
  virtual ~CommsBridge();
  virtual void Start();
  virtual void Stop();

  // Two instances of CommsBridge for the same purpose in the same machine (for
  // debug reasons) must have different namespaces
  // This method must be called before Start
  void SetCommsDeviceId(std::string nspace);
  virtual void SetLogName(std::string name);
  virtual void SetLogLevel(cpplogging::LogLevel);
  virtual void FlushLog();
  virtual void FlushLogOn(LogLevel);
  virtual void LogToConsole(bool);
  virtual void LogToFile(const std::string &filename);

protected:
  virtual void TxWork();
  virtual void RxWork();
  virtual void TransmitFrame();
  virtual bool ReceiveFrame();
  virtual bool TryToConnect();
  virtual bool TryToReconnect();
  Timer timer;
  unsigned int _frameTransmissionTime; // milis
  double _byteTransmissionTime;        // milis

  std::string serv_namespace;
  CommsDeviceService phyService;
  PacketPtr txdlf;
  PacketPtr rxdlf;

  uint8_t obuf[MAX_ODATA_BUF];

  std::mutex devicemutex;
  bool connected;
  bool transcurridoTiempoEnvio;
  int baudrate;
  ICommsDevice *device;

  ServiceThread<CommsBridge> txserv, rxserv;
  PacketBuilderPtr _packetBuilder;
};
} /* namespace dcent */

#endif /* DCCOMMS_COMMSBRIDGE_H_ */
