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


#include <OSS/UTL/Thread.h>

#include "v8.h"
#include "OSS/UTL/CoreUtils.h"
#include "OSS/JSON/Json.h"
#include "OSS/UTL/Logger.h"
#include "OSS/JS/JSUtil.h"
#include "OSS/JS/JSIsolateManager.h"
#include "OSS/JS/JSEventLoop.h"


namespace OSS {
namespace JS {
  
//
// Static globals
//
static void V8ErrorMessageCallback(v8::Handle<v8::Message> message, v8::Handle<v8::Value> data)
{
  v8::Isolate* isolate = v8::Isolate::GetCurrent();
  v8::Local<v8::Context> context = isolate->GetCurrentContext();

  v8::HandleScope scope(isolate);
  
  if (message->GetSourceLine(context).ToLocalChecked()->IsString())
  {
    std::string error =
            + "Javascript error on line : "
            + string_from_js_string(isolate, message->GetSourceLine(context).ToLocalChecked());
    OSS::log_error(error);
    OSS::log_error(get_stack_trace(isolate, message, 1024));
  }
}

JSIsolateManager& JSIsolateManager::instance()
{
  static JSIsolateManager manager;
  return manager;
}

JSIsolateManager::JSIsolateManager()
{
  if (v8::V8::Initialize())
  {
    //
    // Create the root isolate
    //
    _rootIsolate = JSIsolate::Ptr(new JSIsolate(0));

    //
    // Set the external heap to 20mb before attempting to garbage collect
    //
    v8::Isolate::GetCurrent()->AdjustAmountOfExternalAllocatedMemory(1024 * 1024 * 20);
    v8::Isolate::GetCurrent()->AddMessageListener(V8ErrorMessageCallback);
  }
  
}

JSIsolateManager::~JSIsolateManager()
{
}

void JSIsolateManager::resetRootIsolate()
{
  assert(_rootIsolate->eventLoop()->isTerminated());
  _rootIsolate.reset(new JSIsolate(0));
}

JSIsolate::Ptr JSIsolateManager::createIsolate(pthread_t parentThreadId)
{
  assert(parentThreadId);
  JSIsolate::Ptr pIsolate = JSIsolate::Ptr(new JSIsolate(parentThreadId));
  return pIsolate;
}

JSIsolate::Ptr JSIsolateManager::findIsolate(pthread_t threadId)
{
  OSS::mutex_critic_sec_lock lock(_mapMutex);
  if (_rootIsolate->getThreadId() == threadId)
  {
    return _rootIsolate;
  }
  
  MapByThreadId::iterator iter = _byThreadId.find(threadId);
  if (iter != _byThreadId.end())
  {
    return iter->second;
  }
  return JSIsolate::Ptr();
}

bool JSIsolateManager::hasIsolate(pthread_t threadId)
{
  OSS::mutex_critic_sec_lock lock(_mapMutex);
  MapByThreadId::iterator iter = _byThreadId.find(threadId);
  return (iter != _byThreadId.end());
}

void JSIsolateManager::registerIsolate(JSIsolate::Ptr pIsolate)
{
  OSS::mutex_critic_sec_lock lock(_mapMutex);
  assert(pIsolate->getThreadId());
  _byThreadId[pIsolate->getThreadId()] = pIsolate;
}

JSIsolate::Ptr JSIsolateManager::rootIsolate()
{
  return _rootIsolate;
}

void JSIsolateManager::run(const JSIsolate::Ptr& pIsolate, const boost::filesystem::path& script)
{
  pIsolate->run(script);
}

void JSIsolateManager::runSource(const JSIsolate::Ptr& pIsolate, const std::string source)
{
  pIsolate->runSource(source);
}

void JSIsolateManager::setExternalData(const std::string& name, intptr_t data)
{
  OSS::mutex_critic_sec_lock lock(_mapMutex);
  _externalData[name] = data;
}

intptr_t JSIsolateManager::getExternalData(const std::string& name) const
{
  OSS::mutex_critic_sec_lock lock(_mapMutex);
  ExternalData::const_iterator iter = _externalData.find(name);
  if (iter == _externalData.end())
  {
    return 0;
  }
  return iter->second;
}

void JSIsolateManager::initGlobalExports(v8::Isolate* isolate, JSObjectTemplateHandle& global)
{
  for(GlobalExports::iterator iter = _exports.begin(); iter != _exports.end(); iter++)
  {
    global->Set(JSString(isolate,iter->first), v8::FunctionTemplate::New(isolate,iter->second));
  }
  
  for (GlobalExportVector::iterator iter = _exportHandlers.begin(); iter != _exportHandlers.end(); iter++)
  {
    (*iter)(global);
  }
}

  
} } // OSS::JS


