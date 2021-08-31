#pragma once

#include <QtCore>

#if defined(VV_SHARED) || !defined(VV_STATIC)
#  ifdef VV_STATIC
#    error "Both VV_SHARED and VV_STATIC defined, please make up your mind"
#  endif
#  ifndef VV_SHARED
#    define VV_SHARED
#  endif
#  if defined(VUTILITY_LIB)
#    define VUTILITY_LIBRARY_EXPORT Q_DECL_EXPORT
#  else
#    define VUTILITY_LIBRARY_EXPORT Q_DECL_IMPORT
#  endif
#else
#  define VUTILITY_LIBRARY_EXPORT
#endif

#define VV_UTILITY_BEGIN namespace vvUtility{
#define VV_UTILITY_END }
#define USING_VV_UTILITY using namespace vvUtility