#include "OSS/JS/JSPlugin.h"
#include "OSS/UTL/CoreUtils.h"
#include "OSS/UTL/Logger.h"
#include "OSS/JS/ObjectWrap.h"
#include "OSS/JS/BufferObject.h"

using OSS::JS::ObjectWrap;

using OSS::JS::BufferObject;
typedef BufferObject::ByteArray ByteArray; 

static bool int_array_to_byte_array(v8::Handle<v8::Array>& input, ByteArray& output)
{
  v8::HandleScope scope;
  output.clear();
  output.reserve(input->Length());
  for(uint32_t i = 0; i < input->Length(); i++)
  {
    uint32_t val = input->Get(i)->ToInt32()->Value();
    if (val >= 256)
    {
      return false;
    }
    output.push_back(val);
  }
  return !output.empty();
}

static bool string_to_byte_array(std::string& input, ByteArray& output)
{
  output.clear();
  output.reserve(input.size());
  for(uint32_t i = 0; i < input.size(); i++)
  {
    output.push_back((uint32_t)input.at(i));
  }
  return !output.empty();
}

static bool byte_array_to_int_array(ByteArray& input, v8::Handle<v8::Array>& output)
{
  v8::HandleScope scope;
  uint32_t i = 0;
  for (ByteArray::iterator iter = input.begin(); iter != input.end(); iter++)
  {
    output->Set(i++, v8::Int32::New(*iter));
  }
  return output->Length() > 0;
}

v8::Persistent<v8::Function> BufferObject::_constructor;

BufferObject::BufferObject()
{
}

BufferObject::BufferObject(const BufferObject& obj) :
  _buffer(obj._buffer)
{
}

BufferObject::BufferObject(std::size_t size) :
  _buffer(size)
{
}

BufferObject::~BufferObject()
{
}

v8::Handle<v8::Value> BufferObject::New(const v8::Arguments& args) 
{
  v8::HandleScope scope;
  BufferObject* pBuffer = 0;
  
  if (args.Length() == 1 && args[0]->IsNumber())
  {
    pBuffer = new BufferObject(args[0]->IntegerValue());
  }
  else if (args.Length() == 1 && args[0]->IsString())
  {
    std::string str = *v8::String::Utf8Value(args[0]);
    pBuffer = new BufferObject();
    if (!string_to_byte_array(str, pBuffer->_buffer))
    {
      delete pBuffer;
      return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid String Elements")));
    }
  }
  else if (args.Length() == 1 && args[0]->IsArray())
  {
    v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(args[0]);
    pBuffer = new BufferObject();
    if (!int_array_to_byte_array(array, pBuffer->_buffer))
    {
      delete pBuffer;
      return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Array Elements")));
    }
  }
  else if (args.Length() == 1 && args[0]->IsObject() &&
    args[0]->ToObject()->Has(v8::String::NewSymbol("ObjectType")) &&
    args[0]->ToObject()->Get(v8::String::NewSymbol("ObjectType"))->ToString()->Equals(v8::String::NewSymbol("Buffer")))
  {
    BufferObject* obj = ObjectWrap::Unwrap<BufferObject>(args[0]->ToObject());
    if (!obj)
    {
      return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Buffer Object")));
    }
    pBuffer = new BufferObject(*obj);
  }
  else if(args.Length() != 0)
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Argument")));
  }
  else
  {
    pBuffer = new BufferObject();
  }
  
  pBuffer->Wrap(args.This());
  return args.This();
}

v8::Handle<v8::Value> BufferObject::Create(const v8::Arguments& args)
{
  v8::HandleScope scope;
  v8::Handle<v8::Value> buffer = BufferObject::_constructor->NewInstance();
  return buffer;
}

v8::Handle<v8::Value> BufferObject::size(const v8::Arguments& args)
{
  v8::HandleScope scope;
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(args.This());
  if (!pBuffer)
  {
    return v8::Undefined();
  }
  v8::Handle<v8::Integer> size = v8::Integer::New(pBuffer->_buffer.size());
  return size;
}


v8::Handle<v8::Value> BufferObject::fromArray(const v8::Arguments& args)
{
  v8::HandleScope scope;
  if (args.Length() != 1 || !args[0]->IsArray())
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Argument")));
  }
  v8::Handle<v8::Array> array = v8::Handle<v8::Array>::Cast(args[0]);
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(args.This());
  if (!int_array_to_byte_array(array, pBuffer->_buffer))
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Array Elements")));
  }
  return v8::Undefined();
}

v8::Handle<v8::Value> BufferObject::fromString(const v8::Arguments& args)
{
  v8::HandleScope scope;
  if (args.Length() != 1 || !args[0]->IsString())
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Argument")));
  }
  std::string str = *v8::String::Utf8Value(args[0]);
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(args.This());
  if (!string_to_byte_array(str, pBuffer->_buffer))
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Array Elements")));
  }
  return v8::Undefined();
}

v8::Handle<v8::Value> BufferObject::fromBuffer(const v8::Arguments& args)
{
  v8::HandleScope scope;
  if (args.Length() != 1 || !args[0]->IsObject()  ||
    !args[0]->ToObject()->Has(v8::String::NewSymbol("ObjectType")) ||
    !args[0]->ToObject()->Get(v8::String::NewSymbol("ObjectType"))->ToString()->Equals(v8::String::NewSymbol("Buffer")))
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Argument")));
  }
  
  BufferObject* theirs = ObjectWrap::Unwrap<BufferObject>(args[0]->ToObject());
  BufferObject* ours = ObjectWrap::Unwrap<BufferObject>(args.This());
  ours->_buffer = theirs->_buffer;
  return v8::Undefined();
}

v8::Handle<v8::Value> BufferObject::toArray(const v8::Arguments& args)
{
  v8::HandleScope scope;
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(args.This());
  v8::Handle<v8::Array> output = v8::Array::New(pBuffer->_buffer.size());
  byte_array_to_int_array(pBuffer->_buffer, output);
  return output;
}

v8::Handle<v8::Value> BufferObject::toString(const v8::Arguments& args)
{
  v8::HandleScope scope;
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(args.This());
  v8::Handle<v8::String> output = v8::String::New((const char*)pBuffer->_buffer.data(), pBuffer->_buffer.size());
  return output;
}

v8::Handle<v8::Value> BufferObject::equals(const v8::Arguments& args)
{
  v8::HandleScope scope;
  if (args.Length() != 1 || !args[0]->IsObject()  ||
    !args[0]->ToObject()->Has(v8::String::NewSymbol("ObjectType")) ||
    !args[0]->ToObject()->Get(v8::String::NewSymbol("ObjectType"))->ToString()->Equals(v8::String::NewSymbol("Buffer")))
  {
    return v8::Boolean::New(false);
  }
  
  BufferObject* theirs = ObjectWrap::Unwrap<BufferObject>(args[0]->ToObject());
  BufferObject* ours = ObjectWrap::Unwrap<BufferObject>(args.This());

  return v8::Boolean::New(ours->_buffer == theirs->_buffer);
}

//
// Properties
//
v8::Handle<v8::Value> BufferObject::getAt(uint32_t index, const v8::AccessorInfo& info)
{
  v8::HandleScope scope;
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(info.This());
  
  if (index >= pBuffer->_buffer.size())
  {
    return v8::ThrowException(v8::Exception::RangeError(v8::String::New("Index Out Of Range")));
  }
  
  return v8::Uint32::New(pBuffer->_buffer[index]);
}

v8::Handle<v8::Value> BufferObject::setAt(uint32_t index,v8::Local<v8::Value> value, const v8::AccessorInfo& info)
{
  v8::HandleScope scope;
  BufferObject* pBuffer = ObjectWrap::Unwrap<BufferObject>(info.This());
  
  if (index >= pBuffer->_buffer.size())
  {
    return v8::ThrowException(v8::Exception::RangeError(v8::String::New("Index Out Of Range")));
  }
  
  uint32_t val = value->ToUint32()->Value();
  if (val > 256)
  {
    return v8::ThrowException(v8::Exception::TypeError(v8::String::New("Invalid Argument")));
  }
  
  pBuffer->_buffer[index] = val;
  return v8::Undefined();
}

void BufferObject::Init(v8::Handle<v8::Object> exports)
{
  v8::Local<v8::FunctionTemplate> tpl = v8::FunctionTemplate::New(BufferObject::New);
  
  tpl->SetClassName(v8::String::NewSymbol("Buffer"));
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("ObjectType"), v8::String::NewSymbol("Buffer"));
  
  tpl->InstanceTemplate()->SetInternalFieldCount(1);
  
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("Create"), v8::FunctionTemplate::New(Create)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("size"), v8::FunctionTemplate::New(size)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("toArray"), v8::FunctionTemplate::New(toArray)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("toString"), v8::FunctionTemplate::New(toString)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("fromArray"), v8::FunctionTemplate::New(fromArray)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("fromString"), v8::FunctionTemplate::New(fromString)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("fromBuffer"), v8::FunctionTemplate::New(fromBuffer)->GetFunction());
  tpl->PrototypeTemplate()->Set(v8::String::NewSymbol("equals"), v8::FunctionTemplate::New(equals)->GetFunction());
  
  tpl->PrototypeTemplate()->SetIndexedPropertyHandler(getAt, setAt);
  
  BufferObject::_constructor = v8::Persistent<v8::Function>::New(tpl->GetFunction());
  exports->Set(v8::String::NewSymbol("Buffer"), BufferObject::_constructor);
}

static v8::Handle<v8::Value> init_exports(const v8::Arguments& args)
{
  v8::HandleScope scope; 
  v8::Persistent<v8::Object> exports = v8::Persistent<v8::Object>::New(v8::Object::New());
  BufferObject::Init(exports);
  return exports;
}

JS_REGISTER_MODULE(Buffer);