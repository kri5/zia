#ifndef POOL_H__
# define POOL_H__

#include <queue>

#include "Task.h"
#include "IThread.h"
#include "Mutex.h"

class   Pool
{
    public:
        Pool(unsigned int nbThreads);
        void                    init();
        bool                    addTask(Task*);
        void                    addSleepingThread(IThread*);
        Task*                   popTask();
        IThread*                popFreeThread();
        bool                    createThread();
        void                    killThread();
        unsigned int            getNbThreads() const;
        unsigned int            getFreeThreadsNbr() const;
        unsigned int            getTaskNbr() const;
        bool                    empty() const;
    private:
        std::queue<Task*>       _tasks;
        std::queue<IThread*>    _threads;
        unsigned int            _nbThreads;
        IMutex*                 _mutex;
        class   Manager : public IThread
        {
            public:
                static Manager* create(Pool*);
            private:
                Manager(Pool*);
                void        code();
                Pool*       _pool;
                
        };
        Pool::Manager*          _manager;
};

#endif //POOL_H__

