#include <iostream>
#include <string>
#include <thread>

#include <tools_cxx/Singleton.h>
#include <tools_cxx/CurrentThread.h>

class Test{
public:
    Test(){
        std::cout << "执行了Test构造函数, CurrentThread: " << CurrentThread::get_tid() << std::endl;
    }

    ~Test(){
        std::cout << "执行了Test析构函数, CurrentThread: " << CurrentThread::get_tid() << std::endl;
    }

    const std::string & get_name() const {
        return m_name;
    }

    void set_name(const std::string & name){
        m_name = name;
    }

private:
    std::string m_name;
};

void threadFunc(){
    std::cout << "CurrentThread: " <<  CurrentThread::get_tid() 
            << ", instance: "  << &Singleton<Test>::instance()
            << ", name: " << Singleton<Test>::instance().get_name()
            << std::endl;

    Singleton<Test>::instance().set_name("only one, changed");
}

int main(){
    Singleton<Test>::instance().set_name("only one");

    std::thread t(threadFunc);
    t.join();

    std::cout << "CurrentThread: " <<  CurrentThread::get_tid() 
        << ", instance: "  << &Singleton<Test>::instance()
        << ", name: " << Singleton<Test>::instance().get_name()
        << std::endl;

    return 0;
}