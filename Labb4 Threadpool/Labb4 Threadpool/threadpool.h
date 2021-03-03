
#ifndef THREAD_POOL_H
#define THREAD_POOL_H

class ThreadPool {
public:
    //used by main (you are free to change here and in main)
    ThreadPool(size_t threads);
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args);

    /* todo: your implementation and more methods and variables*/
};

#endif


