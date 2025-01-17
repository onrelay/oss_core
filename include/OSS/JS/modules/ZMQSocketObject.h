// Library: OSS_CORE - Foundation API for SIP B2BUA
// Copyright (c) OSS Software Solutions
// Contributor: Joegen Baclor - mailto:joegen@ossapp.com
//
// Permission is hereby granted, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, execute, and to prepare
// derivative works of the Software, all subject to the
// "GNU Lesser General Public License (LGPL)".
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef OSS_ZMQSOCKETOBJECT_H_INCLUDED
#define OSS_ZMQSOCKETOBJECT_H_INCLUDED

#include <v8.h>
#include <OSS/JS/JSPlugin.h>
#include <OSS/ZMQ/ZMQSocket.h>
#include <OSS/UTL/Thread.h>

class ZMQSocketObject : public OSS::JS::JSObjectWrap
{
public:
  typedef OSS::ZMQ::ZMQSocket Socket;
  ZMQSocketObject(Socket::SocketType type);
  virtual ~ZMQSocketObject();

  JS_CONSTRUCTOR_DECLARE();
  
  JS_METHOD_DECLARE(connect);
  JS_METHOD_DECLARE(bind);
  JS_METHOD_DECLARE(subscribe);
  JS_METHOD_DECLARE(publish);
  JS_METHOD_DECLARE(send);
  JS_METHOD_DECLARE(receive);
  JS_METHOD_DECLARE(close);
  JS_METHOD_DECLARE(getFd);

  //
  // Helpers
  //
  void notifyReadable();
  void clearReadable();
  static void pollForReadEvents();
  Socket* getSocket();
  static boost::thread* _pPollThread;
  
protected:
  Socket* _pSocket;
  int _pipe[2];
};


//
// Inlines
//
inline ZMQSocketObject::Socket* ZMQSocketObject::getSocket()
{
  return _pSocket;
}
#endif // OSS_ZMQSOCKETOBJECT_H_INCLUDED

