#ifndef SOURCEFILE_H_
#define SOURCEFILE_H_

#include <string.h>

// compile time calculation of basename of source file                                  -- 编译器获取源文件的basename
class SourceFile                                                                        // 用于接收__FILE__宏定义的值
{
public:
    template<int N>                                                                     // 构造函数还能是模板函数？？，参数是char数组
    SourceFile(const char (&arr)[N])                                                    // 用于接收字符串常量 -- "liuyunian"
        : data_(arr),
        size_(N-1)
    {
        const char* slash = strrchr(data_, '/'); // builtin function                    -- 内置函数
        if (slash)
        {
        data_ = slash + 1;
        size_ -= static_cast<int>(data_ - arr);
        }
    }

    explicit SourceFile(const char* filename)                                           // 用于接收字符串变量 -- const char * str = "liuyunian"
        : data_(filename)
    {
        const char* slash = strrchr(filename, '/');
        if (slash)
        {
        data_ = slash + 1;
        }
        size_ = static_cast<int>(strlen(data_));
    }

    const char* data_;
    int size_;
};

#endif // SOURCEFILE_H_