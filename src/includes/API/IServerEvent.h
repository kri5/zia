#ifndef __ISERVEREVENT_H__
# define __ISERVEREVENT_H__

#include "IModule.h"

namespace zAPI
{
    /**
     *  This hook interface forces you to implement two event methods.
     *  <ul>
     *      <li>The first on (onServerStart) will be called when the server starts</li>
     *      <li>The second one (onServerStop) will be called before the server exits</li>
     *  </ul>
     */
    /// Base interface for modules who wants to hook on server events.
    class IServerEvent
    {
        public:

            /**
             *  The interface virtual destructor
             */
            virtual ~IServerEvent() {}

            /**
             *  The serverStart event.<br>
             *  This method will be called just after the server has been launched.
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus     onServerStart() = 0;

            /**
             *  The serverStop event.<br>
             *  This method will be called before the server stops.
             *  @return a ChainStatus
             */
            virtual IModule::ChainStatus     onServerStop() = 0;
    };
}

#endif /* !__ISERVEREVENT_H__ */
