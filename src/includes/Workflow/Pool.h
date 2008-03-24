#ifndef POOL_H__
# define POOL_H__

#include <queue>
#include <list>

#include "Task.h"
#include "Threads/IThread.h"
#include "Workflow/Worker.h"
#include "Mutex/Mutex.h"

class Worker;

struct  KeepAliveClient
{
    KeepAliveClient(ClientSocket* _clt, const std::vector<const Vhost*>* _vhosts)
        : clt(_clt), vhosts(_vhosts) {}
    ClientSocket*   clt;
    const std::vector<const Vhost*>*    vhosts;
};

class   Pool
{
    public:
        Pool(unsigned int nbThreads, unsigned int);
        void                    init();
        bool                    addTask(ClientSocket*, const std::vector<const Vhost*>*);
        void                    rescheduleTask(Task*);
        void                    addSleepingThread(Worker*);
        Task*                   popTask();
        Worker*                 popFreeThread();
        void                    finishTask(Task*);
        bool                    createThread();
        void                    killThread();
        void                    relaunchThread(Worker*);
        unsigned int            getNbThreads() const;
        unsigned int            getFreeThreadsNbr() const;
        unsigned int            getTaskNbr() const;
        bool                    empty() const;
        void                    addKeepAliveClient(ClientSocket*, const std::vector<const Vhost*>*);
        void                    flushKeepAlive(std::list<KeepAliveClient>&);
    private:
        ///Must be launched from a thread safe environment.
        void                    __createThread();

        std::queue<Task*>       _tasks;
        std::queue<Task*>       _freeTasks;
        std::queue<Worker*>     _threads;
        std::list<Worker*>      _workingThreads;
        std::queue<KeepAliveClient>    _keepAlive;
        unsigned int            _nbThreads;
        unsigned int            _nbTasks;
        IMutex*                 _mutex;
        IMutex*                 _keepAliveMutex;
        class   Manager : public IThread
        {
            public:
                struct  Error
                {
                    enum Code
                    {
                        Unknown,
                        Poll
                    };
                    static const char*  Msg[];
                };
                static Manager* create(Pool*);
            private:
                Manager(Pool*);
                void            code();
                void            initKeepAlivePoll();
                void            checkKeepAlive();
                Pool*           _pool;
                struct pollfd*  _fds;
                std::list<KeepAliveClient>    _keepAlive;
                
        };
        Pool::Manager*          _manager;
};

#endif //POOL_H__

