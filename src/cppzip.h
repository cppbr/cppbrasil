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


#ifndef CPPZIP_H
#define CPPZIP_H

#include <QObject>
#include <QDebug>
#include <zlib.h>

#ifdef CPPBRASIL_STATIC
    class CppZip
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT CppZip
#endif
{
public:
    CppZip();
    static bool compress(const QByteArray &input, QByteArray &output);
    static bool decompress(const QByteArray &input, QByteArray &output);
};

#endif // CPPZIP_H
