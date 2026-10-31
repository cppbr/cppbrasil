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


#ifndef CONTAINER_H
#define CONTAINER_H
#include <QList>
#include <memory>

template <class T>
class Container
{
public:
    QList<T *> *items;
    T *obj;
public:
    Container();
    ~Container();
    //proibe a copia: items e obj sao ponteiros donos, a copia causaria double delete.
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    void add();
    void add(std::unique_ptr<T> object); //o container assume a propriedade do objeto.
    void clear();
};

template<class T>
Container<T>::Container(): items(new QList<T *>), obj(new T)
{
}

template<class T>
Container<T>::~Container()
{
    qDeleteAll(*items);
    items->clear();
    delete items;
    delete obj;
}

template<class T>
void Container<T>::add()
{
    //cria o novo obj antes do append, se o new lancar excecao o container continua consistente.
    std::unique_ptr<T> ptr = std::make_unique<T>();
    items->append(obj);
    obj = ptr.release();
}

template<class T>
void Container<T>::add(std::unique_ptr<T> object)
{
    items->append(object.get());
    object.release();
}

template<class T>
void Container<T>::clear()
{
    obj->clear();
    qDeleteAll(*items);
    items->clear();
}

#endif // CONTAINER_H
