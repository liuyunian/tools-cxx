#ifndef SOURCEFILE_H_
#define SOURCEFILE_H_

#include <string.h>

// compile time calculation of basename of source file                                  -- 编译器获取源文件的basename
class SourceFile                                                                        // 用于接收__FILE__宏定义的值
{
public:
    template<int N>                                                                     // 构造函数还能是模板函数？？，参数是char数组
    SourceFile(const char (&arr)[N]) :                                                  // 用于接收字符串常量 -- "liuyunian"
        m_data(arr),
        m_size(N-1)
    {
        const char* slash = strrchr(m_data, '/');                                       // builtin function -- 内置函数
        if (slash){
            m_data = slash + 1;
            m_size -= static_cast<int>(m_data - arr);
        }
    }

    explicit SourceFile(const char* filename) :                                        // 用于接收字符串变量 -- const char * str = "liuyunian"
        m_data(filename)
    {
        const char* slash = strrchr(filename, '/');
        if (slash){
            m_data = slash + 1;
        }
        m_size = static_cast<int>(strlen(m_data));
    }

public:
    const char* m_data;
    int m_size;
};

#endif // SOURCEFILE_H_