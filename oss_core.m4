#
# Export version variables
#
OSS_CORE_VERSION_INFO="$OSS_CORE_VERSION_CURRENT:$OSS_CORE_VERSION_REVISION:$OSS_CORE_VERSION_AGE"
AC_SUBST(OSS_CORE_VERSION_CURRENT)
AC_SUBST(OSS_CORE_VERSION_REVISION)
AC_SUBST(OSS_CORE_VERSION_AGE)
AC_SUBST(OSS_CORE_VERSION_INFO)

#
# Check if C++0x/11/14 is supported by the compiler
#
m4_include([config/ax_cxx_compile_stdcxx.m4])

#
# --disable-dep-check will simply display missing 
# dependencies as warnings.  This is useful when you
# simply need to generate the makefile for tasks that
# do not require building.  Example is "make dist"
#
AC_ARG_ENABLE(dep-check,
      AS_HELP_STRING([--disable-dep-check],
      [Do not check for dependencies other then those required for dist target]))

AC_DEFUN([ERROR_MISSING_DEP],[
    if test "x$enable_dep_check" != "xno"; then
        AC_MSG_ERROR([$2])
    fi
    AC_SUBST([$1], 0)
])

AC_DEFUN([ENABLE_FEATURE],[
    AM_CONDITIONAL([ENABLE_FEATURE_$1], true)
    AC_SUBST([ENABLE_FEATURE_$1], 1)
    AC_MSG_NOTICE([$1 feature ENABLED])
    FEATURE_$1="enabled"
])

AC_DEFUN([DISABLE_FEATURE],[
    AM_CONDITIONAL([ENABLE_FEATURE_$1], false)
    AC_SUBST([ENABLE_FEATURE_$1], 0)
    AC_MSG_NOTICE([$1 feature DISABLED])
])

#
# Flag dependecy whether they are missing or not
#
AC_DEFUN([FLAG_MISSING_DEP],[
    AC_SUBST([$1], 0)
    AM_CONDITIONAL([$1], false)
])

#
# Flag existing CXX dependencies
#
AC_DEFUN([FLAG_EXISTING_CXX_DEP],[
    AC_SUBST([$1], 1)
    AM_CONDITIONAL([$1], true)
    OSS_CORE_DEP_LIBS="$2 $OSS_CORE_DEP_LIBS"
])


#
# Enable ALL features
#
AC_ARG_ENABLE([all-features],
    AC_HELP_STRING([--enable-all-features], [Enable ALL features]),
    [
        ENABLE_FEATURE(RESTKV)
        ENABLE_FEATURE(CARP)
        ENABLE_FEATURE(V8)
        ENABLE_FEATURE(B2BUA)
        ENABLE_FEATURE(WEBSOCKETS)
        ENABLE_FEATURE(XOR)
        ENABLE_FEATURE(RTP)
        ENABLE_FEATURE(STUN)
        ENABLE_FEATURE(ZMQ)
        ENABLE_FEATURE(REDIS)
        ENABLE_FEATURE(MCRYPT)
        ENABLE_FEATURE(CONFIG)
        ENABLE_FEATURE(NET_EXTRA)
        ENABLE_FEATURE(INOTIFY)
    ],
    [
        #
        # Disable B2BUA compilation
        #
        AC_ARG_ENABLE([b2bua],
            AC_HELP_STRING([--disable-b2bua], [Disable B2BUA Feature]),
            [DISABLE_FEATURE(B2BUA)],
            [ENABLE_FEATURE(B2BUA)])
        #
        # Disable RTP Proxy compilation
        #
        AC_ARG_ENABLE([rtp],
            AC_HELP_STRING([--disable-rtp], [Disable RTP Proxy Feature]),
            [DISABLE_FEATURE(RTP)],
            [ENABLE_FEATURE(RTP)])

        #
        # Enable UCARP compilation
        #
        AC_ARG_ENABLE([carp],
            AC_HELP_STRING([--disable-carp], [Disable CARP-HA Feature]),
            [DISABLE_FEATURE(CARP)],
            [ENABLE_FEATURE(CARP)])
        #
        # Enable V8 compilation
        #
        AC_ARG_ENABLE([v8],
            AC_HELP_STRING([--disable-v8], [Disable V8 JavaScript Feature]),
            [DISABLE_FEATURE(V8)],
            [ENABLE_FEATURE(V8)])
        #
        # Enable REST KV compilation
        #
        AC_ARG_ENABLE([restkv],
            AC_HELP_STRING([--enable-rest], [Enable REST Key Value Store Feature]),
            [ENABLE_FEATURE(RESTKV)],
            [DISABLE_FEATURE(RESTKV)])
        #
        # Enable ZeroMQ compilation
        #
        AC_ARG_ENABLE([zmq],
            AC_HELP_STRING([--disable-zmq], [Disable ZeroMQ Feature]),
            [DISABLE_FEATURE(ZMQ)],
            [ENABLE_FEATURE(ZMQ)])
        #
        # Enable Redis compilation
        #
        AC_ARG_ENABLE([redis],
            AC_HELP_STRING([--disable-redis], [Disable Redis Feature]),
            [DISABLE_FEATURE(REDIS)],
            [ENABLE_FEATURE(REDIS)])
        #
        # Enable Websocket compilation
        #
        AC_ARG_ENABLE([websockets],
            AC_HELP_STRING([--disable-websockets], [Disable Websockets Feature]),
            [DISABLE_FEATURE(WEBSOCKETS)],
            [ENABLE_FEATURE(WEBSOCKETS)])
        #
        # Enable XOR compilation
        #
        AC_ARG_ENABLE([xor],
            AC_HELP_STRING([--enable-xor], [Enable XOR Encryption Feature]),
            [ENABLE_FEATURE(XOR)],
            [DISABLE_FEATURE(XOR)])
        #
        # Enable File Encryption compilation
        #
        AC_ARG_ENABLE([mcrypt],
            AC_HELP_STRING([--enable-mcrypt], [Enable File Encryption Feature]),
            [ENABLE_FEATURE(MCRYPT)],
            [DISABLE_FEATURE(MCRYPT)])
        #
        # Enable Configpp Wrapper compilation
        #
        AC_ARG_ENABLE([config],
            AC_HELP_STRING([--disable-config], [Disable Configuration File Feature]),
            [DISABLE_FEATURE(CONFIG)],
            [ENABLE_FEATURE(CONFIG)])
        #
        # Enable STUN compilation
        #
        AC_ARG_ENABLE([stun],
            AC_HELP_STRING([--disable-stun], [Disable STUN Feature]),
            [DISABLE_FEATURE(STUN)],
            [ENABLE_FEATURE(STUN)])

        #
        # Enable NET compilation
        #
        AC_ARG_ENABLE([net-extra],
            AC_HELP_STRING([--disable-net-extra], [Disable NET Extra Feature]),
            [DISABLE_FEATURE(NET_EXTRA)],
            [ENABLE_FEATURE(NET_EXTRA)])

         #
        # Enable NET compilation
        #
        AC_ARG_ENABLE([inotify],
            AC_HELP_STRING([--disable-inotify], [Disable inotify]),
            [DISABLE_FEATURE(INOTIFY)],
            [ENABLE_FEATURE(INOTIFY)])
    ])

#
# Enable SIP Test compilation
#
AC_ARG_ENABLE([sip-test],
    AC_HELP_STRING([--enable-sip-test], [Enable Compilation of SIP Tests]),
    [ENABLE_FEATURE(COMPILE_SIPTEST)],
    [DISABLE_FEATURE(COMPILE_SIPTEST)])

#
# Search for mandatory packages
#

#
# OpenSSL
#
AC_CHECK_HEADER(openssl/conf.h, [], 
    [ERROR_MISSING_DEP(OSS_HAVE_SSL,"OpenSSL Library Headers are not installed")])
AC_CHECK_LIB(ssl, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_DL, -lssl)], 
    [ERROR_MISSING_DEP(OSS_HAVE_SSL,"OpenSSL Library not installed")])
AC_CHECK_LIB(crypto, main, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_CRYPTO, -lcrypto)], 
    [ERROR_MISSING_DEP(OSS_HAVE_CRYPTO)])
case $host_os in
    darwin* )
        OSS_CORE_DEP_LIBS="-L/usr/local/opt/openssl/lib $OSS_CORE_DEP_LIBS"
        ;;
esac


#
# Boost
#
AC_LANG_PUSH([C++])

AC_CHECK_HEADER(boost/thread.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_THREAD, 
    "Boost Thread Library Headers are not installed")])
AC_CHECK_LIB(boost_thread-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_THREAD, -lboost_thread-mt)],
    [AC_CHECK_LIB(boost_thread, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_THREAD, -lboost_thread)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_THREAD, "Boost Thread Library is not installed")])])

AC_CHECK_HEADER(boost/date_time.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_DATE_TIME, 
    "Boost Date Library Headers are not installed")])
AC_CHECK_LIB(boost_date_time-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_DATE_TIME, -lboost_date_time-mt)],
    [AC_CHECK_LIB(boost_date_time, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_DATE_TIME, -lboost_date_time)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_DATE_TIME, "Boost Date Library is not installed")])])

AC_CHECK_HEADER(boost/filesystem.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_FILESYSTEM, 
    "Boost Filesystem Library Headers are not installed")])
AC_CHECK_LIB(boost_filesystem-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_FILESYSTEM, -lboost_filesystem-mt)],
    [AC_CHECK_LIB(boost_filesystem, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_FILESYSTEM, -lboost_filesystem)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_FILESYSTEM, "Boost Filesystem Library is not installed")])])

AC_CHECK_LIB(boost_system-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_SYSTEM, -lboost_system-mt)],
    [AC_CHECK_LIB(boost_system, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_SYSTEM, -lboost_system)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_SYSTEM, "Boost System Library is not installed")])])

AC_CHECK_HEADER(boost/program_options.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_PROGRAM_OPTIONS, 
    "Boost Program Options Library Headers are not installed")])
AC_CHECK_LIB(boost_program_options-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_PROGRAM_OPTIONS, -lboost_program_options-mt)],
    [AC_CHECK_LIB(boost_program_options, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_PROGRAM_OPTIONS, -lboost_program_options)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_PROGRAM_OPTIONS, "Boost System Library is not installed")])])

AC_CHECK_LIB(boost_iostreams-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_IOSTREAMS, -lboost_iostreams-mt)],
    [AC_CHECK_LIB(boost_iostreams, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_IOSTREAMS, -lboost_iostreams)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_IOSTREAMS, "Boost Iostreams Library is not installed")])])

AC_CHECK_HEADER(boost/regex.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_REGEX, 
    "Boost Regex Library Headers are not installed")])
AC_CHECK_LIB(boost_regex-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_REGEX, -lboost_regex-mt)],
    [AC_CHECK_LIB(boost_regex, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_REGEX, -lboost_regex)],
    	[ERROR_MISSING_DEP(OSS_HAVE_BOOST_REGEX, "Boost Regex Library is not installed")])])
AC_LANG_POP([C++])

#
# POCO
#
AC_LANG_PUSH([C++])

AC_CHECK_HEADER(Poco/Foundation.h, [], [ERROR_MISSING_DEP(OSS_HAVE_POCO_FOUNDATION, 
    "Poco C++ Foundation Library Headers not found")])
AC_CHECK_LIB(PocoFoundation, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_POCO_FOUNDATION, -lPocoFoundation)],
    [ERROR_MISSING_DEP(OSS_HAVE_POCO_FOUNDATION,"Poco C++ Foundation Library not found")])

AC_CHECK_HEADER(Poco/Util/Util.h, [], [ERROR_MISSING_DEP(OSS_HAVE_POCO_UTIL, 
    "Poco C++ Util Library Headers not found")])
AC_CHECK_LIB(PocoUtil, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_POCO_UTIL, -lPocoUtil)],
    [ERROR_MISSING_DEP(OSS_HAVE_POCO_UTIL,"Poco C++ Util Library not found")])

AC_CHECK_HEADER(Poco/Net/Net.h, [], [ERROR_MISSING_DEP(OSS_HAVE_POCO_NET, 
    "Poco C++ Util Library Headers not found")])
AC_CHECK_LIB(PocoNet, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_POCO_NET, -lPocoNet -lPocoNetSSL)],
    [ERROR_MISSING_DEP(OSS_HAVE_POCO_NET,"Poco C++ Net Library not found")])
AC_LANG_POP([C++])

#
# UCARP
#
if test "x$FEATURE_CARP" == "xenabled"; then
    m4_include([config/carp.m4])
fi


#
# Google V8
#
if test "x$FEATURE_V8" == "xenabled"; then
    AC_LANG_PUSH([C++])
    AC_CHECK_HEADER(v8.h, 
        [FLAG_EXISTING_CXX_DEP(OSS_HAVE_V8, -lv8)],
        [ERROR_MISSING_DEP(OSS_HAVE_V8, "V8 Library is not installed")])
    AC_LANG_POP([C++])
else
    AM_CONDITIONAL(OSS_HAVE_V8, false)
    AC_SUBST(OSS_HAVE_V8, 0)
fi

#
# ZeroMQ
#
if test "x$FEATURE_ZMQ" == "xenabled"; then
    AC_CHECK_HEADER(zmq.h, 
        [FLAG_EXISTING_CXX_DEP(OSS_HAVE_ZMQ, -lzmq)], 
        [ERROR_MISSING_DEP(OSS_HAVE_ZMQ, "ZeroMQ Library is not installed")])
else
    AM_CONDITIONAL(OSS_HAVE_ZMQ, false)
    AC_SUBST(OSS_HAVE_ZMQ, 0)
fi
#
# Redis
#
if test "x$FEATURE_REDIS" == "xenabled"; then
AC_CHECK_HEADER(hiredis/hiredis.h,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_HIREDIS, -lhiredis)], 
    [ERROR_MISSING_DEP(OSS_HAVE_HIREDIS, "Hiredis Library is not installed")])
else
    AM_CONDITIONAL(OSS_HAVE_HIREDIS, false)
    AC_SUBST(OSS_HAVE_HIREDIS, 0)
fi
#
# MCRYPT
#
if test "x$FEATURE_MCRYPT" == "xenabled"; then
AC_CHECK_HEADER(mcrypt.h,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_MCRYPT, -lmcrypt)], 
    [FLAG_MISSING_DEP(OSS_HAVE_MCRYPT, "Mcrypt Library is not installed")])
else
    AM_CONDITIONAL(OSS_HAVE_MCRYPT, false)
    AC_SUBST(OSS_HAVE_MCRYPT, 0)
fi
#
# INOTIFY
#
if test "x$FEATURE_INOTIFY" == "xenabled"; then
AC_CHECK_HEADER(sys/inotify.h,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_INOTIFY, )], 
    [FLAG_MISSING_DEP(OSS_HAVE_INOTIFY, "inotify feature is not available")])
else
    AM_CONDITIONAL(OSS_HAVE_INOTIFY, false)
    AC_SUBST(OSS_HAVE_INOTIFY, 0)
fi
#
# Config file support
#
if test "x$FEATURE_CONFIG" == "xenabled"; then
AC_LANG_PUSH([C++])
AC_CHECK_HEADER(libconfig.h++,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_CONFIGPP, -lconfig++)], 
    [FLAG_MISSING_DEP(OSS_HAVE_CONFIGPP, "libconfig Library not installed")])
AC_LANG_POP([C++])
else
    AM_CONDITIONAL(OSS_HAVE_CONFIGPP, false)
    AC_SUBST(OSS_HAVE_CONFIGPP, 0)
fi
#
# REST Key/Value Store
#
if test "x$FEATURE_RESTKV" == "xenabled"; then
AC_LANG_PUSH([C++])
AC_CHECK_HEADER(leveldb/db.h, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_LEVELDB, -lleveldb)], 
    [ERROR_MISSING_DEP(OSS_HAVE_LEVELDB,"LevelDB Library is not installed")])
AC_LANG_POP([C++])
else
    AM_CONDITIONAL(OSS_HAVE_LEVELDB, false)
    AC_SUBST(OSS_HAVE_LEVELDB, 0)
fi

#
# Search for optional packages
#

AC_LANG_PUSH([C++])
AC_CHECK_LIB(gtest, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_GTEST, -lgtest)],
    [FLAG_MISSING_DEP(OSS_HAVE_GTEST)])
AC_LANG_POP([C++])

AC_CHECK_HEADER(pcap/pcap.h,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_PCAP, -lpcap)], 
    [FLAG_MISSING_DEP(OSS_HAVE_PCAP)])

AC_CHECK_HEADER(pthread.h, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_THREAD, -lpthread)], 
    [FLAG_MISSING_DEP(OSS_HAVE_THREAD)])

AC_CHECK_HEADER(resolv.h, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_RESOLV, -lresolv)], 
    [FLAG_MISSING_DEP(OSS_HAVE_RESOLV)])

AC_CHECK_HEADER(zlib.h, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_ZLIB, -lz)], 
    [FLAG_MISSING_DEP(OSS_HAVE_ZLIB)])

AC_CHECK_LIB(ltdl, main, 
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_LTDL, -lltdl)],
    [FLAG_MISSING_DEP(OSS_HAVE_LTDL)])

AC_CHECK_LIB(dl, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_DL, -ldl)], 
    [FLAG_MISSING_DEP(OSS_HAVE_DL)])

AC_CHECK_LIB(rt, main,     
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_RT, -lrt)], 
    [FLAG_MISSING_DEP(OSS_HAVE_RT)])

AC_LANG_PUSH([C++])
AC_CHECK_HEADER(boost/random.hpp, [], [ERROR_MISSING_DEP(OSS_HAVE_BOOST_RANDOM, 
    "Boost Random Library Headers are not installed")])
AC_CHECK_LIB(boost_random-mt, main,
    [FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_RANDOM, -lboost_random-mt)],
    [AC_CHECK_LIB(boost_random, main,
    	[FLAG_EXISTING_CXX_DEP(OSS_HAVE_BOOST_RANDOM, -lboost_random)],
    	[FLAG_MISSING_DEP(OSS_HAVE_BOOST_RANDOM, "Boost Random Library is not installed")])])
AC_LANG_POP([C++])

CURRENT_DIR=`pwd`
cd ${srcdir}
OSS_CORE_SRCDIR=`pwd`
AC_SUBST(OSS_CORE_SRCDIR)
cd ${CURRENT_DIR}

#
# Common CXX and C Flags
#
OSS_CXX_C_FLAGS="-fmessage-length=0"
#
# Compiler warnings
#
OSS_CXX_WARNINGS="-Wall -Wformat -Wwrite-strings -Wpointer-arith -Wno-unused-result -Wno-strict-aliasing"
OSS_C_WARNINGS="-Wall -Wnested-externs -Werror=return-type -Werror=uninitialized -Wno-pointer-sign -Wno-unused-function -Wno-shift-negative-value -Wno-format-extra-args"
#
# Additional CXX and C Flags
#
CXXFLAGS="$CXXFLAGS $OSS_CXX_C_FLAGS $OSS_CXX_WARNINGS"
CFLAGS="$CFLAGS $OSS_CXX_C_FLAGS $OSS_C_WARNINGS -fno-omit-frame-pointer -fno-common -fsigned-char -fPIC"

#
# Set C++ 11 flag if supported by compiler
#
AX_CXX_COMPILE_STDCXX([11], [], [optional])

#
# Used by applications building oss_core inline.  In our case, we set it to nothing
#
AC_SUBST([OSS_CORE_ADDITIONAL_INCLUDES], [''])



AC_SUBST(CXXFLAGS)
AC_SUBST(CFLAGS)
AC_SUBST(OSS_CORE_DEP_LIBS)
