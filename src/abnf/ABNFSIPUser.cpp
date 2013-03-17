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


#include "OSS/ABNF/ABNFSIPUser.h"


namespace OSS {

namespace ABNF {


/// Satisfies RFC 3261 ABNF Rule for 
/// user =  1*( unreserved / escaped / user-unreserved )
typedef ABNFAnyOfMultiple3<ABNF_SIP_unreserved, ABNF_SIP_escaped, ABNF_SIP_user_unreserved> _pvar1;///unreserved / escaped / user-unreserved
typedef ABNFLoopUntil<_pvar1, ABNFLoopExitChars<'@', ':'>, 1, 1024> Parser;///user =  1*( unreserved / escaped / user-unreserved )
static Parser _parser;

ABNFSIPUser::ABNFSIPUser()
{
}

char* ABNFSIPUser::parse(const char* _t)
{
  return _parser.parse(_t);
}

} } //OSS::ABNF
