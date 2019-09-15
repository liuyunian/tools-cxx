#include <iostream>
#include <tools_cxx/Timestamp.h>

int main(){
    Timestamp now(Timestamp::now());
    std::cout << now.to_string() << std::endl;
    std::cout << now.to_formattedString() << std::endl;
    std::cout << now.to_formattedString(false) << std::endl;

    return 0;
}