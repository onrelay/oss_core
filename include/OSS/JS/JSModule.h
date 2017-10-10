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


#ifndef OSS_JSMODULE_H_INCLUDED
#define OSS_JSMODULE_H_INCLUDED


#include "OSS/build.h"
#if ENABLE_FEATURE_V8

#include "v8.h"
#include "OSS/UTL/CoreUtils.h"


namespace OSS {
namespace JS {

class JSBase;

class JSModule
{
public:
  struct Module
  {
    std::string name;
    std::string script;
  };
  
  typedef std::map<std::string, Module> InternalModules;
  typedef std::vector<Module> ModuleHelpers;
  
  JSModule(JSBase* pBase);
  ~JSModule();
  
  bool setGlobals(v8::Handle<v8::ObjectTemplate>& global);
  bool initialize(v8::TryCatch& try_catch, v8::Handle<v8::ObjectTemplate>& global);
  
  InternalModules& getInternalModules();
  ModuleHelpers& getModuleHelpers();
  const std::string& getModulesDir() const;
  void setModulesDir(const std::string& modulesDir);
protected:
  void registerInternalModule(const Module& module);
  void registerModuleHelper(const Module& module);
  bool compileModuleHelpers(v8::TryCatch& try_catch, v8::Handle<v8::ObjectTemplate>& global);
  
  JSBase* _pBase;
  
  InternalModules _modules;
  ModuleHelpers _moduleHelpers;
  std::string _modulesDir;
};


//
// Inlines
//

inline JSModule::InternalModules& JSModule::getInternalModules()
{
  return _modules;
}
  
inline JSModule::ModuleHelpers& JSModule::getModuleHelpers()
{
  return _moduleHelpers;
}

inline const std::string& JSModule::getModulesDir() const
{
  return _modulesDir;
}

inline void JSModule::setModulesDir(const std::string& modulesDir)
{
  _modulesDir = modulesDir;
}


} }

#endif // ENABLE_FEATURE_V8
#endif // OSS_JSMODULE_H_INCLUDED
