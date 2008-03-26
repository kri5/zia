#ifndef __IWORKFLOW_H__
# define __IWORKFLOW_H__

#include "IModule.h"

class IWorkflow : public IModule
{
    public:
        virtual ~IWorkflow() {};
        virtual ChainStatus     onBegin() = 0;
        virtual ChainStatus     onEnd() = 0;

};

#endif /* !__IWORKFLOW_H__ */
