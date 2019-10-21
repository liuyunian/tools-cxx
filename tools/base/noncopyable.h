#ifndef NONCOPYABLE_H_
#define NONCOPYABLE_H_

class noncopyable{
public:
    noncopyable(const noncopyable&) = delete;      // 禁用拷贝构造函数
    void operator=(const noncopyable&) = delete;   // 禁用赋值运算符函数

protected:
    noncopyable() = default;
    ~noncopyable() = default;
};

#endif // NONCOPYABLE_H_