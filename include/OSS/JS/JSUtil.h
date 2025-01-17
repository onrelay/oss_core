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

#ifndef OSS_JSUTIL_H_INCLUDED
#define OSS_JSUTIL_H_INCLUDED

#include "OSS/build.h"
#if ENABLE_FEATURE_V8

#include "v8.h"
#include "OSS/UTL/CoreUtils.h"
#include <OSS/JS/JS.h>

namespace OSS {
namespace JS {

std::string string_from_js_string(v8::Isolate* isolate, v8::Handle<v8::Value> str);
std::string string_from_js_string(v8::Handle<v8::Value> str);

std::string string_from_js_value(v8::Isolate* isolate, const v8::Handle<v8::Value>& str);
std::string string_from_js_value(v8::Handle<v8::Value> str);

const char* cstring_from_js_string(const v8::String::Utf8Value& value);

std::string read_file(const std::string& name);
std::string read_file_skip_shebang(const std::string& name, bool hasCommonJs = false);
std::string read_directory(const boost::filesystem::path& directory);

//
// Logging
//
void report_js_exception(v8::Isolate* isolate, v8::TryCatch &try_catch, bool show_line);
JS_GLOBAL_FUNCTION_DECLARE(log_info_callback);
JS_GLOBAL_FUNCTION_DECLARE(log_debug_callback);
JS_GLOBAL_FUNCTION_DECLARE(log_error_callback);

//
// Unwrap C++ Object
//

template<typename T>
static T* unwrap_external_object(JSCallbackInfo args)
{
  if (args.Length() < 1)
    return 0;
  v8::Handle<v8::Value> obj = args[0];
  if (!obj->IsObject())
    return 0;
  v8::Handle<v8::External> field = v8::Handle<v8::External>::Cast(obj->ToObject(args.GetIsolate()->GetCurrentContext()).ToLocalChecked()->GetInternalField(0));
  void* ptr = field->Value();
  return static_cast<T*>(ptr);
}

/* Not used 
void wrap_external_object(v8::Persistent<v8::Context>* pContext, 
  v8::Persistent<v8::ObjectTemplate>* pRequestTemplate,
  v8::Handle<v8::Object>& objectInstance,
  OSS_HANDLE pObject);
  */


std::string get_stack_trace(v8::Isolate* isolate, v8::Handle<v8::Message> message, uint32_t bufLen);
  
}}


#endif // ENABLE_FEATURE_V8
#endif // OSS_JSUTIL_H_INCLUDED

