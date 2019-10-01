#include <iostream>

#include <unistd.h>

#include <tools_cxx/ThreadPool.h>

void func1(){
    std::cout << "test thread func1" << std::endl;
}

void func2(int num){
    std::cout << "test thread func2, num = " << num << std::endl;
}

int main(){
    ThreadPool tp;
    tp.create(5);

    for(int i = 15; i > 0; -- i){
        if(i % 2 == 0){
            tp.call(func1);
        }
        else{
            tp.call(std::bind(func2, i));
        }
    }

    sleep(2);
    
    return 0;
}