/*
Copyright 2022 João Elson
jetmorju@gmail.com

Web-site: https://cppbrasil.com.br

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/


#ifndef CPPDANFEQTRPT_P_H
#define CPPDANFEQTRPT_P_H

#include <QtCore/qglobal.h>

# ifdef CPPDANFEQTRPT_STATICLIB
#  undef CPPDANFEQTRPT_SHARED
#  define CPPDANFEQTRPT_EXPORT
# else
#  ifdef CPPDANFEQTRPT_BUILD_CPPDANFEQTRPT_LIB
#   define CPPDANFEQTRPT_EXPORT Q_DECL_EXPORT
#  else
#   define CPPDANFEQTRPT_EXPORT Q_DECL_IMPORT
#  endif
# endif

#endif // CPPDANFEQTRPT_P_H
