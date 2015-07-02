
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

#include "OSS/SIP/B2BUA/SIPB2BRegisterAgent.h"
#include "OSS/UTL/Logger.h"
#include "OSS/SIP/SIPURI.h"


namespace OSS {
namespace SIP {
namespace B2BUA {

  
#define REG_RETRY_INTERVAL 1000 * 60 /// One minute  
  

SIPB2BRegisterAgent::SIPB2BRegisterAgent() :
  _pRegisterUa(0)
{
}
  
SIPB2BRegisterAgent::~SIPB2BRegisterAgent()
{
  delete _pRegisterUa;
}
  
bool SIPB2BRegisterAgent::initialize(const std::string& agentName)
{
  if (_pRegisterUa)
  {
    OSS_LOG_ERROR("SIPB2BRegisterAgent::initialize called while _pRegisterUa is no longer NULL.");
    return false;
  }
  
  _pRegisterUa = new OSS::SIP::UA::SIPUserAgent(agentName);
  return true;
}

OSS::SIP::UA::SIPRegistration* SIPB2BRegisterAgent::prepareRegister(
    const std::string& domain, 
    const std::string user, 
    const std::string& authUser, 
    const std::string& authPass,
    const std::string& contactParams,
    const std::string& extraHeaders,
    const std::string& nextHop,
    OSS::UInt32 expires
  )
{
  if (!_pRegisterUa)
  {
    OSS_LOG_ERROR("SIPB2BRegisterAgent::sendClientRegister - Registration User Agent not initialized.");
    return false;
  }
  
  OSS::SIP::UA::SIPRegistration* pReg  = new OSS::SIP::UA::SIPRegistration(*_pRegisterUa);

  pReg->setDomain(domain);
  pReg->setContactUser(user);
  pReg->setCredentials(authUser, authPass);
  pReg->setContactParams(contactParams);
  
  if (expires)
    pReg->setExpires(expires);
  
  if (!nextHop.empty())
    pReg->setRouteHeader(nextHop);
  
  pReg->setExtraHeaders(extraHeaders);
  
  return pReg;
}

bool SIPB2BRegisterAgent::sendRegister(
  const std::string& domain, 
  const std::string user, 
  const std::string& authUser, 
  const std::string& authPass,
  const std::string& contactParams,
  const std::string& extraHeaders,
  const std::string& nextHop,
  OSS::UInt32 expires)
{
  OSS::SIP::UA::SIPRegistration* pReg  = prepareRegister(
    domain,
    user,
    authUser,
    authPass,
    contactParams,
    extraHeaders,
    nextHop,
    expires);
  
  pReg->addResponseHandler(boost::bind(&SIPB2BRegisterAgent::onRegisterResponse,
    this, _1, _2, _3));
  
  if (!pReg)
    return false;

  return pReg->run();
}



bool SIPB2BRegisterAgent::sendRegister(
  const std::string& domain, 
  const std::string user, 
  const std::string& authUser, 
  const std::string& authPass,
  const std::string& contactParams,
  const std::string& extraHeaders,
  const std::string& nextHop,
  OSS::UInt32 expires,
  const OSS::SIP::UA::SIPRegistration::ResponseHandler& responseHandler)
{
  OSS::SIP::UA::SIPRegistration* pReg  = prepareRegister(
    domain,
    user,
    authUser,
    authPass,
    contactParams,
    extraHeaders,
    nextHop,
    expires);
  
  if (!pReg)
    return false;

  pReg->addResponseHandler(responseHandler);
  
  
  //
  // Make sure we add the local response handler as the last because it can invalidate the reg pointer
  //
  pReg->addResponseHandler(boost::bind(&SIPB2BRegisterAgent::onRegisterResponse,
    this, _1, _2, _3));
  
  return pReg->run();
}

void SIPB2BRegisterAgent::onRegisterResponse(
  OSS::SIP::UA::SIPRegistration* pReg, 
  const SIPMessage::Ptr& pMsg, 
  const std::string& error)
{
  OSS::mutex_critic_sec_lock lock(_sessionsMutex);
  
  if (pMsg)
  {
    std::string callId = pMsg->hdrGet(OSS::SIP::HDR_CALL_ID);
    OSS_VERIFY_NULL(pReg);
    pReg->setCallId(callId);
    
    if (!callId.empty() && _sessions.find(callId) != _sessions.end())
    {
      OSS::SIP::UA::SIPRegistration::Ptr pNewReg(pReg);
      _sessions[callId] = pNewReg;
    }
    
    if (pMsg->is2xx())
    {
      if (pMsg->hdrPresent(OSS::SIP::HDR_CONTACT))
      {
        
        if (!pReg->isRegistered())
        {
          pReg->markRegistered(true);
        }
        OSS_LOG_INFO("SIPB2BRegisterAgent::onClientRegisterResponse - " 
          << pReg->getContactUser() << "@" << pReg->getDomain() << " - " << callId << " REGISTERED");
      }
      else
      {
        OSS_LOG_INFO("SIPB2BRegisterAgent::onClientRegisterResponse - " 
          << pReg->getContactUser() << "@" << pReg->getDomain() << " UNREGISTERED.");
      }
    }
    else
    {
      OSS_LOG_ERROR("SIPB2BRegisterAgent::onClientRegisterResponse - " 
        << pReg->getContactUser() << "@" << pReg->getDomain() << " registration failure.  " << pMsg->startLine());   
    }
  }
  else if (pReg)
  {
    OSS_LOG_ERROR("SIPB2BRegisterAgent::onClientRegisterResponse - " 
        << pReg->getContactUser() << "@" << pReg->getDomain() << " registration failure.  " << error);
  }
  else
  {
    OSS_LOG_ERROR("SIPB2BRegisterAgent::onClientRegisterResponse - NULL data???"); 
  }
}

bool SIPB2BRegisterAgent::run(const OSS::SIP::UA::SIPUserAgent::ExitHandler& exitHandler)
{
  _pRegisterUa->setExitHandler(exitHandler);
   if  (!_pRegisterUa->run())
  {
    delete _pRegisterUa;
    _pRegisterUa = 0;
    return false;
  }

  return true;
}

void SIPB2BRegisterAgent::stop()
{
  if (_pRegisterUa)
  {
    OSS::mutex_critic_sec_lock lock(_sessionsMutex);
    //
    // Deregister all clients
    //
    for (Sessions::iterator iter = _sessions.begin(); iter != _sessions.end(); iter++)
    {
      iter->second->stop();
    }
    _sessions.clear();
    _pRegisterUa->stop(false);
  }
}

bool SIPB2BRegisterAgent::isLocalRegistration(const std::string& uri) const
{
  OSS::mutex_critic_sec_lock lock(_sessionsMutex);
  OSS::SIP::SIPURI binding(uri);
  for (Sessions::const_iterator iter = _sessions.begin(); iter != _sessions.end(); iter++)
  {
    if (iter->second->isRegisteredBinding(binding))
    {
      return true;
    }
  }
  return false;
}



} } } //OSS::SIP::B2BUA



