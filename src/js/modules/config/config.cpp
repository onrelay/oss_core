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

#include "OSS/JS/JSPlugin.h"
#include "OSS/UTL/CoreUtils.h"
#include "OSS/UTL/Logger.h"
#include "OSS/JS/modules/ConfigObject.h"

#include <stdio.h>

//
// Define the Interface
//
JS_CLASS_INTERFACE(ConfigObject, "Config") 
{
  JS_CLASS_METHOD_DEFINE(ConfigObject, "readFile", readFile);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "writeFile", writeFile);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "toString", toString);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "rootSetting", rootSetting);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "lookupString", lookupString);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "lookupInteger", lookupInteger);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "lookupFloat", lookupFloat);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "lookupBoolean", lookupBoolean);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "lookupSetting", lookupSetting);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLength", settingLength);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingType", settingType);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupElement", settingLookupElement);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupString", settingLookupString);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupInteger", settingLookupInteger);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupFloat", settingLookupFloat);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupBoolean", settingLookupBoolean);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingLookupSetting", settingLookupSetting);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingAddSetting", settingAddSetting);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingSetAsString", settingSetAsString);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingSetAsInteger", settingSetAsInteger);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingSetAsFloat", settingSetAsFloat);
  JS_CLASS_METHOD_DEFINE(ConfigObject, "settingSetAsBoolean", settingSetAsBoolean);
  JS_CLASS_INTERFACE_END(ConfigObject); 
}


ConfigObject::ConfigObject() :
  _id(0),
  _rootSettingId(0)
{
  config_init(&_cfg);
}

ConfigObject::~ConfigObject()
{
  config_destroy(&_cfg);
}

config_setting_t* ConfigObject::findSetting(uint32_t id)
{
  Settings::iterator iter = _settings.find(id);
  if (iter == _settings.end())
  {
    return 0;
  }
  return iter->second;
}

uint32_t ConfigObject::findSetting(config_setting_t* pSetting)
{
  for (Settings::iterator iter = _settings.begin(); iter != _settings.end(); iter++)
  {
    if (pSetting == iter->second)
    {
      return iter->first;
    }
  }
  return 0;
}

JS_CONSTRUCTOR_IMPL(ConfigObject) 
{
  ConfigObject* pConfig = new ConfigObject();
  pConfig->Wrap(js_method_self());
  js_method_set_return_self();
}

JS_METHOD_IMPL(ConfigObject::readFile)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  std::string path = js_method_arg_as_std_string(0);
  js_method_set_return_boolean(!!config_read_file(&pConfig->getConfig(), path.c_str()));
}

JS_METHOD_IMPL(ConfigObject::writeFile)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  std::string path = js_method_arg_as_std_string(0);
  js_method_set_return_boolean(!!config_write_file(&pConfig->getConfig(), path.c_str()));
}

JS_METHOD_IMPL(ConfigObject::toString)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_uint32(0);
  uint32_t bufLen = js_method_arg_as_uint32(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  
  FILE* fp = ::fmemopen(0, bufLen, "w+");
  config_write(&pConfig->getConfig(), fp);
  
  fseek(fp, 0, SEEK_END);
  int size = ftell(fp);
  rewind(fp);

  char* chars = new char[size + 1];
  chars[size] = '\0';
  for (int i = 0; i < size;) {
    int read = fread(&chars[i], 1, size - i, fp);
    i += read;
  }
  fclose(fp);
  
  JSStringHandle result = js_method_string(chars);
  delete[] chars;
  js_method_set_return_handle(result);
}

JS_METHOD_IMPL(ConfigObject::lookupString)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  
  std::string key = js_method_arg_as_std_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  const char* value;
  if (!config_lookup_string(&pConfig->getConfig(), key.c_str(), &value))
  {
    js_method_set_return_undefined();
    return;
  }
  js_method_set_return_string(value);
}

JS_METHOD_IMPL(ConfigObject::lookupFloat)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  
  std::string key = js_method_arg_as_std_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  double value = 0;
  if (!config_lookup_float(&pConfig->getConfig(), key.c_str(), &value))
  {
    js_method_set_return_undefined();
    return;
  }
  js_method_set_return_integer(value);
}

JS_METHOD_IMPL(ConfigObject::lookupInteger)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  
  std::string key = js_method_arg_as_std_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  int value = 0;
  if (!config_lookup_int(&pConfig->getConfig(), key.c_str(), &value))
  {
    js_method_set_return_undefined();
    return;  
  }
  js_method_set_return_integer(value);
}

JS_METHOD_IMPL(ConfigObject::lookupBoolean)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  
  std::string key = js_method_arg_as_std_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  int value = 0;
  if (!config_lookup_bool(&pConfig->getConfig(), key.c_str(), &value))
  {
    js_method_set_return_undefined();
    return;  
  }
  js_method_set_return_boolean(!!value);
}

JS_METHOD_IMPL(ConfigObject::lookupSetting)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_string(0);
  
  std::string key = js_method_arg_as_std_string(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = config_lookup(&pConfig->getConfig(), key.c_str());
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }

  uint32_t existingId = pConfig->findSetting(pSetting);
  if (existingId)
  {
    js_method_set_return_handle(js_method_uint32(existingId));
    return;
  }
  
  pConfig->_settings[++pConfig->_id] = pSetting;
  js_method_set_return_handle(js_method_uint32(pConfig->_id));
}

JS_METHOD_IMPL(ConfigObject::rootSetting)
{
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  if (pConfig->_rootSettingId)
  {
    js_method_set_return_handle(js_method_uint32(pConfig->_rootSettingId));
    return;
  }
  
  config_setting_t* pSetting = config_root_setting(&pConfig->getConfig());
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }   
  pConfig->_rootSettingId = ++pConfig->_id;
  pConfig->_settings[pConfig->_rootSettingId] = pSetting;
  js_method_set_return_handle(js_method_uint32(pConfig->_rootSettingId));
}

JS_METHOD_IMPL(ConfigObject::settingLength)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_uint32(0);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  if (!pSetting)
  {
    js_method_set_return_handle(js_method_int32(0));
  }
  uint32_t count = config_setting_length(pSetting);
  
  js_method_set_return_handle(js_method_int32(count));
}

JS_METHOD_IMPL(ConfigObject::settingType)
{
  js_method_args_assert_size_eq(1);
  js_method_arg_assert_uint32(0);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }
  int type = config_setting_type(pSetting);
  
  js_method_set_return_handle(js_method_int32(type));
}

JS_METHOD_IMPL(ConfigObject::settingLookupString)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }
  const char* value;
  if (!key.empty())
  {
    if (!config_setting_lookup_string(pSetting, key.c_str(), &value))
    {
    js_method_set_return_undefined();
    return;  
    }
  }
  else if (config_setting_type(pSetting) == CONFIG_TYPE_STRING)
  {
    value = config_setting_get_string(pSetting);
  }
      
  js_method_set_return_string(value);
}

JS_METHOD_IMPL(ConfigObject::settingLookupInteger)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }
  int value = 0;
  if (!key.empty())
  {
    if (!config_setting_lookup_int(pSetting, key.c_str(), &value))
    {
      js_method_set_return_undefined();
      return;  
    }
  }
  else if (config_setting_type(pSetting) == CONFIG_TYPE_INT)
  {
    value = config_setting_get_int(pSetting);
  }
  js_method_set_return_handle(js_method_int32(value));
}

JS_METHOD_IMPL(ConfigObject::settingLookupFloat)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }
  double value = 0;
  if (!key.empty())
  {
    if (!config_setting_lookup_float(pSetting, key.c_str(), &value))
    {
      js_method_set_return_undefined();
      return;  
    }
  }
  else if (config_setting_type(pSetting) == CONFIG_TYPE_FLOAT)
  {
    value = config_setting_get_float(pSetting);
  }
  js_method_set_return_integer(value);
}

JS_METHOD_IMPL(ConfigObject::settingLookupBoolean)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;  
  }
  int value = 0;
  if (!key.empty())
  {
    if (!config_setting_lookup_bool(pSetting, key.c_str(), &value))
    {
      js_method_set_return_undefined();
      return;  
    }
  }
  else if (config_setting_type(pSetting) == CONFIG_TYPE_BOOL)
  {
    value = config_setting_get_bool(pSetting);
  }
  js_method_set_return_boolean(!!value);
}

JS_METHOD_IMPL(ConfigObject::settingLookupSetting)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  config_setting_t* pChildSetting = config_setting_get_member(pSetting, key.c_str());
  if (!pSetting || !pChildSetting)
  {
    js_method_set_return_undefined();
    return;   
  }
  
  uint32_t existingId = pConfig->findSetting(pChildSetting);
  if (existingId)
  {
    js_method_set_return_handle(js_method_uint32(existingId));
    return;
  }
  
  pConfig->_settings[++pConfig->_id] = pChildSetting;
  js_method_set_return_handle(js_method_uint32(pConfig->_id));
}

JS_METHOD_IMPL(ConfigObject::settingLookupElement)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_uint32(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  uint32_t index = js_method_arg_as_uint32(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_set_return_undefined();
    return;    
  }
  
  config_setting_t* pElement = config_setting_get_elem(pSetting, index);
    
  uint32_t existingId = pConfig->findSetting(pElement);
  if (existingId)
  {
    js_method_set_return_handle(js_method_uint32(existingId));
    return;
  }
  
  pConfig->_settings[++pConfig->_id] = pElement;
  
  js_method_set_return_handle(js_method_uint32(pConfig->_id));
}

JS_METHOD_IMPL(ConfigObject::settingAddSetting)
{
  js_method_args_assert_size_eq(3);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  js_method_arg_assert_int32(2);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string key = js_method_arg_as_std_string(1);
  int type = js_method_arg_as_int32(2);
  
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  config_setting_t* pChildSetting = config_setting_add(pSetting, key.empty() ? 0 : key.c_str(), type);  
  pConfig->_settings[++pConfig->_id] = pChildSetting;
  js_method_set_return_handle(js_method_uint32(pConfig->_id));
}

JS_METHOD_IMPL(ConfigObject::settingSetAsString)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_string(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  std::string value = js_method_arg_as_std_string(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_throw("Config Setting Not Found");
  }
  
  js_method_set_return_boolean(!!config_setting_set_string(pSetting, value.c_str()));
}

JS_METHOD_IMPL(ConfigObject::settingSetAsInteger)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_int32(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  int value = js_method_arg_as_int32(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_throw("Config Setting Not Found");
  }
  
  js_method_set_return_boolean(!!config_setting_set_int(pSetting, value));
}

JS_METHOD_IMPL(ConfigObject::settingSetAsBoolean)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_bool(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  bool value = js_method_arg_as_bool(1);
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_throw("Config Setting Not Found");
  }
  
  js_method_set_return_boolean(!!config_setting_set_bool(pSetting, value));
}

JS_METHOD_IMPL(ConfigObject::settingSetAsFloat)
{
  js_method_args_assert_size_eq(2);
  js_method_arg_assert_uint32(0);
  js_method_arg_assert_number(1);
  
  uint32_t settingId = js_method_arg_as_uint32(0);
  double value = js_method_arg_as_number(1);
  
  ConfigObject* pConfig = js_method_unwrap_self(ConfigObject);
  config_setting_t* pSetting = pConfig->findSetting(settingId);
  
  if (!pSetting)
  {
    js_method_throw("Config Setting Not Found");
  }
  
  js_method_set_return_boolean(!!config_setting_set_float(pSetting, value));
}

JS_EXPORTS_INIT()
{
  js_export_class(ConfigObject);
  js_export_const(CONFIG_TYPE_GROUP);
  js_export_const(CONFIG_TYPE_INT);
  js_export_const(CONFIG_TYPE_FLOAT);
  js_export_const(CONFIG_TYPE_STRING);
  js_export_const(CONFIG_TYPE_BOOL);
  js_export_const(CONFIG_TYPE_ARRAY);
  js_export_const(CONFIG_TYPE_LIST);
  
  js_export_finalize();
}

JS_REGISTER_MODULE(JSConfig);
