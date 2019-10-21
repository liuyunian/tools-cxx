#ifndef CURRENTTHREAD_H_
#define CURRENTTHREAD_H_

namespace CurrentThread{
    extern int get_tid();

    extern const char* get_tid_string();

    extern int get_tid_string_length();

} // namespace CurrentThread

#endif // CURRENTTHREAD_H_