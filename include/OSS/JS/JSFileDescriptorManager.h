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

#ifndef OSS_JSFILEDESCRIPTORMANAGER_H_INCLUDED
#define OSS_JSFILEDESCRIPTORMANAGER_H_INCLUDED

#include "OSS/build.h"
#if ENABLE_FEATURE_V8

#include "OSS/UTL/Thread.h"
#include "OSS/JS/JSFileDescriptor.h"
#include "OSS/JS/JSEventLoopComponent.h"


namespace OSS {
namespace JS {

class JSEventLoop;

class JSFileDescriptorManager : public JSEventLoopComponent
{
public:
  typedef std::map<int, JSFileDescriptor::Ptr> DescriptorMap;
  typedef std::vector<pollfd> Descriptors;
  
  JSFileDescriptorManager(JSEventLoop* pEventLoop);
  ~JSFileDescriptorManager();
  
  void addFileDescriptor(v8::Isolate* isolate, int fd, v8::Handle<v8::Value> ioHandler, int events);
  bool removeFileDescriptor(int fd);
  JSFileDescriptor::Ptr findDescriptor(int fd);
  void appendDescriptors(Descriptors& descriptors);
  bool signalIO(v8::Isolate* isolate, pollfd pfd);
  
  //
  // Methods exposed to javascript
  //
  JS_METHOD_DECLARE(monitor_descriptor);
  JS_METHOD_DECLARE(unmonitor_descriptor);
  
private:
  OSS::mutex_critic_sec _descriptorsMutex;
  DescriptorMap _descriptors;
};

} } // OSS::JS

#endif // ENABLE_FEATURE_V8
#endif // OSS_JSFILEDESCRIPTORMANAGER_H_INCLUDED

