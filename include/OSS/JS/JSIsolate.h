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


#ifndef OSS_JSISOLATE_H_INCLUDED
#define OSS_JSISOLATE_H_INCLUDED


#include "OSS/build.h"
#if ENABLE_FEATURE_V8

#include <boost/enable_shared_from_this.hpp>
#include "JS.h"
#include "OSS/JSON/Json.h"
#include "OSS/UTL/CoreUtils.h"
#include "OSS/UTL/Thread.h"
#include "OSS/JSON/Json.h"
#include "OSS/JS/JSPersistentValue.h"
#include "OSS/JS/JSInterIsolateCall.h"



namespace OSS {
namespace JS {
  
class JSEventLoop;
class JSModule;
class JSPluginManager;
  
class JSIsolate : public boost::enable_shared_from_this<JSIsolate>
{
public:
  typedef boost::shared_ptr<JSIsolate> Ptr;
  typedef JSInterIsolateCall::Request Request;
  typedef JSInterIsolateCall::Result Result;
  typedef boost::function<void(void*)> Task;
  
  ~JSIsolate();
    /// Destructor.  This is intentially made public so isolates can be used
    /// with shared_ptr
  
  
  void notify(const std::string& request, void* userData);
  void notify(const Request& request, void* userData);
  void notify(const std::string& request, void* userData, JSPersistentFunctionHandle* cb);
  void notify(const Request& request, void* userData, JSPersistentFunctionHandle* cb);
  bool execute(const std::string& request, std::string& result, uint32_t timeout, void* userData);
  bool execute(const Request& request, Result& result, uint32_t timeout, void* userData);
  void emit(const std::string& eventName, const OSS::JSON::Array& args, int queueFd);
  void doTask(const Task& task, void* userData);
  void terminate();
  void join();
  void setExitValue(int value);
  int getExitValue() const;
  JSModule* getModuleManager();
  JSPluginManager* getPluginManager();
  
  bool isThreadSelf();
    /// returns true if the current thread is the isolate thread
  
  static JSIsolate::Ptr getIsolate();
    /// Returs the isolate for the current active isolate thread.
    /// If the current thread is not an isolate thread, an empty pointer
    /// will be returned
  
  const std::string& getName() const;
    /// Returns the given name for this isolate
  
  pthread_t getThreadId() const;
    /// Returns the thread identifier for this isolate
  
  bool isRoot() const;
    /// Returns true if this is the root isolate
  
  JSEventLoop* eventLoop();
    /// Returns a reference to the event loop
  
  JSObjectHandle getGlobal();
    /// Returns the global object.  Must be called in the vent loop thread
  
  JSValueHandle parseJSON(const std::string& json);
    /// returns an object from the json string.  This
    /// function must be called from the event loop thread
  
  const boost::filesystem::path getScript() const;
  
  v8::Isolate* getV8Isolate() const;
  
  const JSIsolate::Ptr& getParentIsolate() const;
  
  void setEventEmitterFd(int fd);
  
  void dispose();
    /// Dispose the isloate.  This can never be undone.
    /// This must be called from outside of the isolate thread
  
  JSLocalObjectHandle wrapExternalPointer(void* ptr);
  
  void setForceAsync(bool force);
  bool getForceAsync() const;
protected:
  JSIsolate(pthread_t parentThreadId);
    /// Creates a new isolate.  You MUST not create isolate directly.
  
  void run(const boost::filesystem::path& script);
    /// Run the script using this isolate
  
  void runSource(const std::string& source);
  
  void internal_run();
  
  JSCopyablePersistentContextHandle _context;
  JSCopyablePersistentObjectTemplateHandle _objectTemplate;
  JSCopyablePersistentObjectTemplateHandle _globalTemplate;
  
  v8::Isolate* _pIsolate;
  JSModule* _pModuleManager;
  JSPluginManager* _pPluginManager;
  int _exitValue;
  pthread_t _threadId;
  pthread_t _parentThreadId;
  std::string _name;
  JSEventLoop* _pEventLoop;
  bool _isRoot;
  boost::thread* _pThread;
  std::string _source;
  boost::filesystem::path _script;
  JSIsolate::Ptr _pParentIsolate;
  int _eventEmitterFd;
  bool _forceAsync;
  friend class JSIsolateManager;
};
  
//
// Inlines
//

inline void JSIsolate::setForceAsync(bool force)
{
  _forceAsync = force;
}

inline bool JSIsolate::getForceAsync() const
{
  return _forceAsync;
}
  
inline void JSIsolate::setExitValue(int value)
{
  _exitValue = value;
}

inline int JSIsolate::getExitValue() const
{
  return _exitValue;
}

inline const std::string& JSIsolate::getName() const
{
  return _name;
}

inline pthread_t JSIsolate::getThreadId() const
{
  return _threadId;
}

inline bool  JSIsolate::isRoot() const
{
  return _isRoot;
}

inline const boost::filesystem::path JSIsolate::getScript() const
{
  return _script;
}

inline v8::Isolate* JSIsolate::getV8Isolate() const
{
  return _pIsolate;
}

inline const JSIsolate::Ptr& JSIsolate::getParentIsolate() const
{
  return _pParentIsolate;
}

inline void JSIsolate::setEventEmitterFd(int fd)
{
  _eventEmitterFd = fd;
}


} } 

#endif  //ENABLE_FEATURE_V8
#endif // OSS_JSISOLATE_H_INCLUDED

