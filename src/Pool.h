#ifndef POOL_H__
# define POOL_H__

#include <queue>
#include <list>

#include "Task.h"
#include "IThread.h"
#include "Worker.h"
#include "Mutex.h"

class Worker;

class   Pool
{
    public:
        Pool(unsigned int nbThreads);
        void                    init();
        bool                    addTask(Task*);
        void                    addSleepingThread(Worker*);
        Task*                   popTask();
        Worker*                 popFreeThread();
        bool                    createThread();
        void                    killThread();
        void                    relaunchThread(Worker*);
        unsigned int            getNbThreads() const;
        unsigned int            getFreeThreadsNbr() const;
        unsigned int            getTaskNbr() const;
        bool                    empty() const;
    private:
        ///Must be launched from a thread safe environment.
        void                    __createThread();

        std::queue<Task*>       _tasks;
        std::queue<Worker*>     _threads;
        std::list<Worker*>      _workingThreads;
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

