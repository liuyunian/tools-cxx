#include <iostream>
#include <tools/base/Timestamp.h>

int main(){
    Timestamp now(Timestamp::now());
    std::cout << now.to_string() << std::endl;
    std::cout << now.to_formatted_string() << std::endl;
    std::cout << now.to_formatted_string(false) << std::endl;

    return 0;
}