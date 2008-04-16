#ifndef POOL_H__
# define POOL_H__

#include <queue>
#include <list>

#include "Task.h"
#include "Threads/IThread.h"
#include "Workflow/Worker.h"
#include "Mutex/Mutex.h"
#include "Time/ITime.h"
#include "API/IClientSocket.h"

class Worker;

struct  KeepAliveClient
{
    KeepAliveClient(zAPI::IClientSocket* _clt, int _timeout, const std::vector<Vhost*>* _vhosts)
        : clt(_clt), timeout(_timeout), vhosts(_vhosts) {}
    zAPI::IClientSocket*            clt;
    ITime*                          timer;
    int                             timeout;
    const std::vector<Vhost*>*      vhosts;
};

class   Pool
{
    public:
        Pool(unsigned int nbThreads, unsigned int);
        virtual                 ~Pool();
        void                    init();
        bool                    addTask(zAPI::IClientSocket*, const std::vector<Vhost*>*);
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
        void                    addKeepAliveClient(zAPI::IClientSocket*, int timeout, const std::vector<Vhost*>*);
        void                    flushKeepAlive(std::list<KeepAliveClient>&);
    private:

        std::queue<Task*>       _tasks;
        std::queue<Task*>       _freeTasks;
        std::queue<Worker*>     _threads;
        std::list<Worker*>      _workingThreads;
        std::queue<KeepAliveClient>    _keepAlive;
        unsigned int            _nbThreads;
        unsigned int            _nbTasks;
        IMutex*                 _mutex;
        //IMutex*                 _keepAliveMutex;
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
                ITime*          _timer;
                
        };
        Pool::Manager*          _manager;
};

#endif //POOL_H__

