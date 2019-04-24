/*
  Asynchronous TCP library for Espressif MCUs

  Copyright (c) 2016 Hristo Gochkov. All rights reserved.
  This file is part of the esp8266 core for Arduino environment.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
*/

#ifndef SYNCCLIENT_H_
#define SYNCCLIENT_H_

#include "Client.h"
#include <async_config.h>
class cbuf;
class AsyncClient;

#ifndef CONST
#define CONST
#endif

class SyncClient: public Client {
  private:
    AsyncClient *_client;
    cbuf *_tx_buffer;
    size_t _tx_buffer_size;
    cbuf *_rx_buffer;

    size_t _sendBuffer();
    void _onData(void *data, size_t len);
    void _onConnect(AsyncClient *c);
    void _onDisconnect();
    void _attachCallbacks();
    void _attachCallbacks_Disconnect();
    void _attachCallbacks_AfterConnected();

    int *_ref;
    void _release();

//D    void _zap() {_client=NULL; _tx_buffer=NULL; _rx_buffer=NULL;} //Added to help debug problem with mallco corruption - SEP 30 2017 - mjh

  public:
    SyncClient(size_t txBufLen = TCP_MSS);
    SyncClient(AsyncClient *client, size_t txBufLen = TCP_MSS);
    virtual ~SyncClient();
    int ref();
    int unref();

    operator bool(){ return connected(); }
    SyncClient & operator=(const SyncClient &other);

#if ASYNC_TCP_SSL_ENABLED
    int connect(CONST IPAddress& ip, uint16_t port, bool secure);
    int connect(const char *host, uint16_t port, bool secure);
    int connect(CONST IPAddress& ip, uint16_t port){
      return connect(ip, port, false);
    }
    int connect(const char *host, uint16_t port){
      return connect(host, port, false);
    }
#else
#ifdef ARDUINO_ESP8266_RELEASE_2_5_0
    int connect(CONST IPAddress& ip, uint16_t port);
#else
    int connect(IPAddress ip, uint16_t port);
#endif
    int connect(const char *host, uint16_t port);
#endif
    void setTimeout(uint32_t seconds);

    uint8_t status();
    uint8_t connected();

//#ifdef ARDUINO_ESP8266_RELEASE_2_5_0
// #if 1 //LWIP_IPV6_NUM_ADDRESSES == 0
    bool stop(unsigned int maxWaitMs);
    bool flush(unsigned int maxWaitMs);
    void stop() { (void)stop(0);}
    void flush() { (void)flush(0);}
// #else
//     bool stop(unsigned int maxWaitMs = 0);
//     bool flush(unsigned int maxWaitMs = 0);
// #endif

    size_t write(uint8_t data);
    size_t write(const uint8_t *data, size_t len);

    int available();
    int peek();
    int read();
    int read(uint8_t *data, size_t len);


};

#endif /* SYNCCLIENT_H_ */
