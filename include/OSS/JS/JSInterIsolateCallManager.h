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

#ifndef OSS_JSINTERISOLATECALLMANAGER_H_INCLUDED
#define OSS_JSINTERISOLATECALLMANAGER_H_INCLUDED

#include "OSS/build.h"
#if ENABLE_FEATURE_V8
#include <queue>
#include "OSS/UTL/Thread.h"
#include "OSS/JS/JS.h"
#include "OSS/JS/JSInterIsolateCall.h"
#include "OSS/JS/JSEventLoopComponent.h"
#include "OSS/JS/JSPersistentValue.h"


namespace OSS {
namespace JS {


class JSInterIsolateCallManager : public JSEventLoopComponent
{
public:
  typedef std::queue<JSInterIsolateCall::Ptr> CallQueue;
  typedef JSInterIsolateCall::Request Request;
  typedef JSInterIsolateCall::Result Result;
  
  JSInterIsolateCallManager(JSEventLoop* pEventLoop);
  ~JSInterIsolateCallManager();
  void notify(const std::string& request, void* userData);
  void notify(const Request& request, void* userData);
  void notify(const std::string& request, void* userData, JSPersistentFunctionHandle* cb);
  void notify(const Request& request, void* userData, JSPersistentFunctionHandle* cb);
  bool execute(const std::string& request, std::string& result, uint32_t timeout, void* userData);
  bool execute(const Request& request, Result& result, uint32_t timeout, void* userData);
  void setHandler(const JSCopyablePersistentFunctionHandle& handler);
  bool doOneWork();
  bool isEnabled();
protected:
  void enqueue(const JSInterIsolateCall::Ptr& pCall);
  JSInterIsolateCall::Ptr dequeue();
  OSS::mutex_critic_sec _queueMutex;
  CallQueue _queue;
  JSCopyablePersistentFunctionHandle _handler;
  friend class JSEventLoop;
};

//
// Inlines
//

inline void JSInterIsolateCallManager::setHandler(const JSCopyablePersistentFunctionHandle& handler)
{
  _handler = handler;
}

inline bool JSInterIsolateCallManager::isEnabled()
{
  return !_handler.IsEmpty();
}

} }

#endif // ENABLE_FEATURE_V8
#endif // OSS_JSINTERISOLATECALLMANAGER_H_INCLUDED

