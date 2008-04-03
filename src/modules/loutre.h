#ifndef LOUTRE_H__
# define LOUTRE_H__

#include "API/ISendResponse.h"

class Loutre : public zAPI::ISendResponse
{
    public:
        Loutre();
        ~Loutre();
};

#endif //LOUTRE_H__

