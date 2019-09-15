#ifndef CURRENTTHREAD_H_
#define CURRENTTHREAD_H_

namespace CurrentThread{
    extern int get_tid();

    extern const char * get_tidString();

    extern int get_tidStringLength();

} // namespace CurrentThread

#endif // CURRENTTHREAD_H_