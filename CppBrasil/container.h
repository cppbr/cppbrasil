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

template <class T>
class Container
{
public:
    QList<T *> *items;
    T *obj;
public:
    Container();
    ~Container();
    void add();
    void add(T *object);
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
    items->append(obj);
    T *ptr = new T;
    obj = ptr;
}

template<class T>
void Container<T>::add(T *object)
{
    items->append(object);
}

template<class T>
void Container<T>::clear()
{
    obj->clear();
    qDeleteAll(*items);
    items->clear();
    if (obj == NULL)
    {
        T *ptr = new T;
        obj = ptr;
    }
}

#endif // CONTAINER_H
