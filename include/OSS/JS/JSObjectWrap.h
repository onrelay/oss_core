// Copyright Joyent, Inc. and other Node contributors.
//
// Permission is hereby granted, free of charge, to any person obtaining a
// copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit
// persons to whom the Software is furnished to do so, subject to the
// following conditions:
//
// The above copyright notice and this permission notice shall be included
// in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
// OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN
// NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR
// OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE
// USE OR OTHER DEALINGS IN THE SOFTWARE.

#ifndef SRC_NODE_OBJECT_WRAP_H_
#define SRC_NODE_OBJECT_WRAP_H_

#include <v8.h>
#include <assert.h>


namespace OSS {
namespace JS {

class JSIsolate;

class JSObjectWrap {
 public:
  JSObjectWrap ();
  virtual ~JSObjectWrap ();


  template <class T>
  static inline T* Unwrap (v8::Handle<v8::Object> handle) 
  {
    if(handle.IsEmpty() || handle->InternalFieldCount() <= 0)
    {
      return 0;
    }
    return static_cast<T*>(handle->GetAlignedPointerFromInternalField(0));
  }
  
  template <class T>
  static v8::Local<v8::FunctionTemplate> ExportConstructorTemplate(
    v8::Isolate* isolate, 
    const char * className, 
    v8::Handle<v8::Object>& exports)
  {
    v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(isolate,T::New);
    tpl->SetClassName(v8::String::NewFromUtf8(isolate,className).ToLocalChecked());
    tpl->PrototypeTemplate()->Set(
      v8::String::NewFromUtf8(isolate,"ObjectType").ToLocalChecked(), 
      v8::String::NewFromUtf8(isolate,className).ToLocalChecked());
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    return tpl;
  }
  
  template <class T>
  static void FinalizeConstructorTemplate(
    v8::Isolate* isolate, 
    const char * className, 
    v8::Local<v8::FunctionTemplate>& tpl, 
    v8::Handle<v8::Object>& exports)
  {
    v8::Local<v8::Function> constructorHandle = tpl->GetFunction(isolate->GetCurrentContext()).ToLocalChecked();

    T::_constructor = v8::Persistent<v8::Function, v8::CopyablePersistentTraits<v8::Function>>(isolate, constructorHandle);

    exports->Set(isolate->GetCurrentContext(), v8::String::NewFromUtf8(isolate,className).ToLocalChecked(), constructorHandle);

  }
  
  template <class T>
  static void ExportMethod(
    v8::Isolate* isolate, 
    v8::Local<v8::FunctionTemplate>& tpl, 
    const char* method,  
    v8::FunctionCallback callback)
  {
    tpl->PrototypeTemplate()->Set(
      v8::String::NewFromUtf8(isolate,method).ToLocalChecked(), 
      v8::FunctionTemplate::New(isolate,callback)->GetFunction(isolate->GetCurrentContext()).ToLocalChecked());
  }
  
  template <class T>
  static void ExportIndexHandler(v8::Isolate* isolate, v8::Local<v8::FunctionTemplate>& tpl, v8::IndexedPropertyGetterCallback getter,  v8::IndexedPropertySetterCallback setter = 0)
  {
    tpl->PrototypeTemplate()->SetIndexedPropertyHandler(getter, setter);
  }
 
  JSIsolate* getIsolate();

  v8::Persistent<v8::Object, v8::CopyablePersistentTraits<v8::Object>> handle_; // ro

 protected:
  void Wrap (v8::Handle<v8::Object> handle);
  void MakeWeak (void);

  /* Ref() marks the object as being attached to an event loop.
   * Refed objects will not be garbage collected, even if
   * all references are lost.
   */
  virtual void Ref();

  /* Unref() marks an object as detached from the event loop.  This is its
   * default state.  When an object with a "weak" reference changes from
   * attached to detached state it will be freed. Be careful not to access
   * the object after making this call as it might be gone!
   * (A "weak reference" means an object that only has a
   * persistant handle.)
   *
   * DO NOT CALL THIS FROM DESTRUCTOR
   */
  virtual void Unref();


  int refs_; // ro
  JSIsolate* _pIsolate;
  void* _pIsolatePtr;

 private:
  static void WeakCallback(const v8::WeakCallbackInfo<JSObjectWrap>& data);
};

//
// Inlines
//
inline JSIsolate* JSObjectWrap::getIsolate()
{
  return _pIsolate;
}


} } // OSS::JS

#endif  // SRC_NODE_OBJECT_WRAP_H_
