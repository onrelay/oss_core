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

#ifndef OSS_ASYNC_H_INCLUDED
#define OSS_ASYNC_H_INCLUDED

#include <boost/thread.hpp>
#include <boost/thread/future.hpp>

#include <OSS/JS/JSPlugin.h>
#include <OSS/UTL/BlockingQueue.h>
#include <queue>
#include <pthread.h>
#include "OSS/JSON/Json.h"
#include "OSS/JS/JSIsolate.h"


typedef OSS::BlockingQueue<std::string> AsyncStringQueue;
typedef boost::function<void(std::string message)> AsyncStringQueueCallback;
typedef boost::function<void()> WakeupTask;
typedef std::queue<WakeupTask> WakeupTaskQueue;
typedef boost::promise<std::string> StringPromise;
typedef boost::future<std::string> AsyncFuture;


struct Async
{
  static pthread_t _threadId;
  static JSCopyablePersistentObjectTemplateHandle _externalPointerTemplate;
  
  static void unmonitor_fd(const OSS::JS::JSIsolate::Ptr& pIsolate, int fd);
    // Forcibly unmonitor a particular file handle.  Use with extreme caution.
  
  static void clear_timer(int timerId);
    // Cancel a running timer
  
  static bool json_execute_promise(OSS::JS::JSIsolate* pIsolate, const OSS::JSON::Object& request, OSS::JSON::Object& reply, uint32_t timeout = 0, void* promiseData = 0);
    // This method is called from the C++ side to execute an RPC style function call towards ossjs.
    // The request object must have a (1) method property containing the name of the callback function
    // (2) and an argument property containing any structure.  The promiseData parameter may 
    // contain a pointer to a structure which can be accessed by C++ plugins.
    // This function is perfectly thread safe. 
    // CAVEAT: timeout will be ignored if promiseData is not null.  This makes sure that the js side
    // will not end up with an invalid pointer if it was not done processing the data yet

  
  //
  // Methods intended to be called within the event loop and are not safe 
  // to be called directly from another thread.  These function would be prefix with __
  //
  
  static bool __execute_one_promise();
    // Execute a promise queued by json_execute_promise 
  
  static void __wakeup_pipe(OSS::JS::JSIsolate* pIsolate = 0);
    // Wakes up the event loop and reset the poll fd set
  
  
  
};


#endif // endif

