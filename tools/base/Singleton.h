#ifndef SINGLETON_H_
#define SINGLETON_H_

#include <assert.h>     // assert
#include <pthread.h>    // pthread_once
#include <stdlib.h>     // atexit

#include "noncopyable.h"

template<typename T>                                                    // SFINAE检测
struct has_no_destory{
    template <typename C> static char test(decltype(&C::no_destroy));
    template <typename C> static int32_t test(...);
    const static bool value = sizeof(test<T>(0)) == 1;
};

template<typename T>
class Singleton : public noncopyable {
public:
    Singleton() = delete;                       // 禁用默认构造函数

    ~Singleton() = delete;                      // 禁用析构函数

    static T& instance(){
        pthread_once(&m_ponce, &Singleton::init);

        assert(m_instance != nullptr);
        return *m_instance;
    }

private:
    static void init(){
        m_instance = new T();

        if(!has_no_destory<T>::value){
            atexit(destory);
        }
    }

    static void destory(){
        typedef char T_must_be_complete_type[sizeof(T) == 0 ? -1 : 1];
        T_must_be_complete_type dummy; (void) dummy;

        delete m_instance;
        m_instance = NULL;
    }

private:
    static pthread_once_t m_ponce;
    static T * m_instance;
};

template<typename T>
pthread_once_t Singleton<T>::m_ponce = PTHREAD_ONCE_INIT;

template<typename T>
T* Singleton<T>::m_instance = nullptr;

#endif // SINGLETON_H_