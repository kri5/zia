#ifndef IRWMUTEX_H
#define IRWMUTEX_H

class IRWMutex
{
public:
        virtual         ~IRWMutex() { }
        /// Get a *shared* read lock.
        virtual void    rdlock() = 0;
        /// Get an *exclusive* write lock.
        virtual void    wrlock() = 0;
        /// Unlock an exclusive write or shared read lock.
        virtual void    unlock() = 0;
};

#endif // IRWMUTEX_H
