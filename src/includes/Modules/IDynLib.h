#ifndef IDYNLIB_H
#define IDYNLIB_H

#include <string>

/// Dynamic librairies interface, to ensure multiplatform compactibility
class IDynLib
{
    public:

        /**
         * The interface virtual destructor 
         */
        virtual ~IDynLib(){};

        /**
         * Loads the specified dynamic library.
         * @param filename The filename of the dynamic library
         * @return Whether the library has been loaded or not
         */
        virtual bool		load(const std::string& filename) = 0;

        /**
         * Loads the specified symbol from the previous loaded library. 
         * @param symbol the name of the symbol you want to load
         * @return A pointer to the loaded symbol.0
         */
        virtual void*		sym(const std::string& symbol) = 0;

        /**
         * Closes the previously loaded library.
         */
        virtual void		close() = 0;

        /**
         * Returns the last error that append during loading whether it's a library nor symbol.
         * @return A string explaining the error.
         */
        virtual const char* lastError() = 0;
};
#endif // IDYNLIB_H

