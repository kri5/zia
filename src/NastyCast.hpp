#include <assert.h>

template<typename T, typename U>
U	nasty_cast(T symbol)
{
    assert(sizeof(T) == sizeof(U));
    union
    {
        T   symbol;
        U   function;
    }       cast;
    cast.symbol = symbol;
    return cast.function;
}


