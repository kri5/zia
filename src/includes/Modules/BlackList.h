#ifndef BLACKLIST_H__
# define BLACKLIST_H__

class   BlackList
{
    public:
        struct  Item
        {
            int modifTime;
            int size;
        };
        BlackList(const std::string&);
        bool        isBlackListed(const std::string&) const;
        void        blackList(const std::string&);
        bool        hasChanged(const std::string) const;
    private:
        std::map<std::string, Item> _blackList;
        std::string                 _path;
};

#endif //BLACKLIST_H__

