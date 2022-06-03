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


#ifndef PAG_H
#define PAG_H

#include <memory>
#include "container.h"
#include "detpag.h"

#ifdef CPPBRASIL_STATIC
    class Pag
#else
    #include "cppbrasil_global.h"
    class CPPBRASIL_EXPORT Pag
#endif
{
public:
    Pag();
    ~Pag();
    void clear();
    double get_vTroco() const;
    void set_vTroco(const double &vTroco);

    std::shared_ptr<Container<DetPag>> detPag;
private:
    //em baixo de card
    double m_vTroco;

};

#endif // PAG_H
