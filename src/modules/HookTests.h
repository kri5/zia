#ifndef HOOKTESTS_H
#define HOOKTESTS_H

#include "API/IBuildResponse.h"
#include "API/IModuleManager.h"
#include "API/INetwork.h"
#include "API/IReceiveRequest.h"
#include "API/ISendResponse.h"
#include "API/IServerEvent.h"
#include "API/IWorkflow.h"


class HookTests : public IBuildResponse, public IModuleManager,
public INetwork, public IReceiveRequest, public ISendResponse,
public IServerEvent, public IWorkflow
{
    HookTests();
    ~HookTests();

};

#endif // HOOKTESTS_H
