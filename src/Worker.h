#ifndef __WORKER_H__
#define __WORKER_H__

#include <fstream>
#include <exception>

#include "IThread.h"
#include "Pool.h"

class Worker : public IThread
{
public:
    static Worker*      create(Pool*);

private:
    Worker(Pool* pool) : _pool(pool) {}
    virtual ~Worker(); 
    void                code();

    Pool*               _pool;
protected:
};

#endif // WORKER_H
