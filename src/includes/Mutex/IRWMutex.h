#ifndef IRWMUTEX_H
#define IRWMUTEX_H

class IRWMutex
{
public:
        virtual         ~IRWMutex() { }
        /// Get a *shared* read lock.
        virtual void    rdLock() = 0;
        /// Get an *exclusive* write lock.
        virtual void    wrLock() = 0;
        /// Unlock a shared read lock.
        virtual void    rdUnlock() = 0;
		/// Unlock an exclusive write lock.
		virtual void	wrUnlock() = 0;
};

#endif // IRWMUTEX_H
