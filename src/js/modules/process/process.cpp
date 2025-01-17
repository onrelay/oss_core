#include "OSS/JS/JSPlugin.h"
#include "OSS/UTL/CoreUtils.h"
#include "OSS/UTL/Logger.h"
#include "OSS/JS/modules/ProcessObject.h"

using OSS::JS::JSObjectWrap;

JS_CLASS_INTERFACE(ProcessObject, "Process") 
{
  JS_CLASS_METHOD_DEFINE(ProcessObject, "run", run);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "kill", kill);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "stop", stop);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "restart", restart);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "unmonitor", unmonitor);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "isAlive", isAlive);
  JS_CLASS_METHOD_DEFINE(ProcessObject, "getPid", getPid);
  JS_CLASS_INTERFACE_END(ProcessObject); 
}

ProcessObject::ProcessObject() :
  _pProcess(0)
{
}

ProcessObject::~ProcessObject()
{
  delete _pProcess;
}

JS_CONSTRUCTOR_IMPL(ProcessObject)
{
  ProcessObject* pProcess = 0;
  
  js_method_args_assert_size_gteq(2);
  js_method_arg_assert_string(0);
  js_method_arg_assert_string(1);
  
  std::string processName = js_method_arg_as_std_string(0);
  std::string startupCommand = js_method_arg_as_std_string(1);
  std::string shutdownCommand;
  std::string pidFile;
  
  if (js_method_args_length() >= 3)
  {
    js_method_arg_assert_string(2);
    startupCommand = js_method_arg_as_std_string(2);
  }
  
  if (js_method_args_length() >= 4)
  {
    js_method_arg_assert_string(3);
    pidFile = js_method_arg_as_std_string(3);
  }
  
  pProcess = new ProcessObject();
  pProcess->_pProcess = new ProcessObject::Process(processName, startupCommand, shutdownCommand, pidFile);
  pProcess->_pProcess->setDeadProcAction(ProcessObject::Process::ProcessUnmonitor);
  pProcess->Wrap(js_method_self());
  
  js_method_set_return_self();
}

JS_METHOD_IMPL(ProcessObject::run)
{
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_boolean(pProcess->_pProcess->executeAndMonitor());
}

JS_METHOD_IMPL(ProcessObject::kill)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_int32(0);
  int signal = js_method_arg_as_int32(0);
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_handle(js_method_int32(pProcess->_pProcess->kill(signal)));
}

JS_METHOD_IMPL(ProcessObject::stop)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_int32(0);
  int signal = js_method_arg_as_int32(0);
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_boolean(pProcess->_pProcess->shutDown(signal));
}

JS_METHOD_IMPL(ProcessObject::restart)
{
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_boolean(pProcess->_pProcess->restart());
}

JS_METHOD_IMPL(ProcessObject::unmonitor)
{
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  pProcess->_pProcess->unmonitor();
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ProcessObject::isAlive)
{
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_boolean(pProcess->_pProcess->isAlive());
}

JS_METHOD_IMPL(ProcessObject::getPid)
{
  ProcessObject* pProcess = js_method_unwrap_self(ProcessObject);
  js_method_set_return_handle(js_method_int32(pProcess->_pProcess->getPID()));
}

JS_EXPORTS_INIT()
{
  js_export_class(ProcessObject);
  js_export_finalize(); 
}

JS_REGISTER_MODULE(JSProcess);