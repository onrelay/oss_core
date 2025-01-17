#include "OSS/JS/JSPlugin.h"
#include "OSS/JS/JSIsolate.h"

#include <rutil/Logger.hxx>

#include "OSS/JS/modules/ResipMasterProfile.h"


using OSS::JS::JSObjectWrap;
using namespace resip;

//
// MasterProfile
//

JS_CLASS_INTERFACE(ResipMasterProfile, "MasterProfile") 
{  
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "addSupportedMethod", addSupportedMethod);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "addAllowedEvent", addAllowedEvent);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "validateAcceptEnabled", validateAcceptEnabled);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "validateContentEnabled", validateContentEnabled);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "setUserAgent", setUserAgent);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "setOutboundProxy", setOutboundProxy);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "setDefaultFrom", setDefaultFrom);
  JS_CLASS_METHOD_DEFINE(ResipMasterProfile, "setDigestCredential", setDigestCredential);
  JS_CLASS_INTERFACE_END(ResipMasterProfile); 
}

ResipMasterProfile::ResipMasterProfile()
{
}

ResipMasterProfile::~ResipMasterProfile()
{
}

JS_CONSTRUCTOR_IMPL(ResipMasterProfile)
{
  ResipMasterProfile* object = new ResipMasterProfile();
  object->_profile = resip::SharedPtr<resip::MasterProfile>(new resip::MasterProfile());
  object->Wrap(js_method_self());
  js_method_set_return_self();
}

JS_METHOD_IMPL(ResipMasterProfile::addSupportedMethod)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_int32(value, 0);
  MethodTypes method_type = (MethodTypes)value;
  self->profile()->addSupportedMethod(method_type);
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::addAllowedEvent)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_string(value, 0);
  Token token(value.c_str());
  self->profile()->addAllowedEvent(token);
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::validateAcceptEnabled)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_bool(value, 0);
  self->profile()->validateAcceptEnabled() = value;
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::validateContentEnabled)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_bool(value, 0);
  self->profile()->validateContentEnabled() = value;
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::setUserAgent)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_string(value, 0);
  self->profile()->setUserAgent(value.c_str());
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::setOutboundProxy)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_string(value, 0);
  Uri uri(Data(value.c_str()));
  self->profile()->setOutboundProxy(uri);
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::setDefaultFrom)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_string(value, 0);
  NameAddr aor(Data(value.c_str()));
  self->profile()->setDefaultFrom(aor);
  js_method_set_return_undefined();
}

JS_METHOD_IMPL(ResipMasterProfile::setDigestCredential)
{
  js_method_declare_self(ResipMasterProfile, self);
  js_method_declare_string(realm, 0);
  js_method_declare_string(user, 1);
  js_method_declare_string(password, 2);
  self->profile()->setDigestCredential(realm.c_str(), user.c_str(), password.c_str());
  js_method_set_return_undefined();
}