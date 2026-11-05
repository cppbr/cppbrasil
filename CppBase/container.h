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
private:
    QList<T *> m_items; //itens confirmados, o container e o dono.
public:
    T *obj; //rascunho: preencher e chamar add() para confirmar.

    //obsoleto: acesso direto a lista. Use count(), value(i) ou for (auto *x : *container).
    [[deprecated("use count(), value(i) ou for (auto *x : *container)")]]
    QList<T *> *const items;
public:
    typedef typename QList<T *>::const_iterator const_iterator;

    Container();
    ~Container();
    //proibe a copia: o container e dono dos itens e do obj, a copia causaria double delete.
    Container(const Container&) = delete;
    Container& operator=(const Container&) = delete;
    void add();
    void add(std::unique_ptr<T> object); //o container assume a propriedade do objeto.
    void clear();

    int count() const;
    bool isEmpty() const;
    T *value(const int &i) const; //retorna nullptr se o indice for invalido.
    const_iterator begin() const;
    const_iterator end() const;
};

//inicializar o items (obsoleto) nao deve gerar aviso de deprecated, apenas o uso dele.
#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable: 4996)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
#endif
template<class T>
Container<T>::Container(): obj(new T), items(&m_items)
{
}
#if defined(_MSC_VER)
#pragma warning(pop)
#elif defined(__GNUC__) || defined(__clang__)
#pragma GCC diagnostic pop
#endif

template<class T>
Container<T>::~Container()
{
    qDeleteAll(m_items);
    m_items.clear();
    delete obj;
}

template<class T>
void Container<T>::add()
{
    //cria o novo obj antes do append, se o new lancar excecao o container continua consistente.
    std::unique_ptr<T> ptr = std::make_unique<T>();
    m_items.append(obj);
    obj = ptr.release();
}

template<class T>
void Container<T>::add(std::unique_ptr<T> object)
{
    m_items.append(object.get());
    object.release();
}

template<class T>
void Container<T>::clear()
{
    obj->clear();
    qDeleteAll(m_items);
    m_items.clear();
}

template<class T>
int Container<T>::count() const
{
    return m_items.count();
}

template<class T>
bool Container<T>::isEmpty() const
{
    return m_items.isEmpty();
}

template<class T>
T *Container<T>::value(const int &i) const
{
    return m_items.value(i, nullptr);
}

template<class T>
typename Container<T>::const_iterator Container<T>::begin() const
{
    return m_items.cbegin();
}

template<class T>
typename Container<T>::const_iterator Container<T>::end() const
{
    return m_items.cend();
}

#endif // CONTAINER_H
