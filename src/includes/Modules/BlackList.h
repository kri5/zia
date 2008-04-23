#ifndef BLACKLIST_H__
# define BLACKLIST_H__

#include <map>
#include <string>

#include "Time/Time.h"

class   BlackList
{
    public:
        struct  Item
        {
            Item(){};
            Item(time_t _modt, size_t _size) : modifTime(_modt), size(_size){}
            time_t      modifTime;
            size_t      size;
        };
        BlackList(const std::string&);
        bool        isBlackListed(const std::string&) const;
        void        blackList(const std::string&);
        bool        hasChanged(const std::string&) const;
    private:
        std::map<std::string, Item> _blackList;
        char*       _path;
};

#endif //BLACKLIST_H__

