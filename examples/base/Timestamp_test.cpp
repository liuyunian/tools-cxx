#include <iostream>
#include <tools/base/Timestamp.h>

int main(){
    Timestamp now = Timestamp::now();                                                // test static now()

    std::cout << "microseconds = " << now.get_microseconds_since_epoch() << '\n';   // test get_microseconds_since_epoch()
    std::cout << "seconds = " << now.get_seconds_since_epoch() << '\n';             // test get_seconds_since_epoch()
    std::cout << "period = " << now.get_period() << '\n';                           // test get_period

    std::cout << now.to_string() << '\n';                                           // test to_string
    std::cout << now.to_formatted_string() << '\n';                                 // test to_formatted_string
    std::cout << now.to_formatted_string(false) << '\n';

    std::cout << now.add_time(2).to_formatted_string() << std::endl;

    Timestamp invalidTimestamp = Timestamp::invalid();                              // test static invalid()

    std::cout << std::boolalpha << (now == invalidTimestamp) << '\n';               // test = !=
    std::cout << std::boolalpha << (now != invalidTimestamp) << '\n';

    std::cout << std::boolalpha << (now < invalidTimestamp) << '\n';                // test < > <= >=
    std::cout << std::boolalpha << (now > invalidTimestamp) << '\n';
    std::cout << std::boolalpha << (now <= invalidTimestamp) << '\n';
    std::cout << std::boolalpha << (now >= invalidTimestamp) << '\n';

    return 0;
}