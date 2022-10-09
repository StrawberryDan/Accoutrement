#include "Util/Net/Socket/Socket.hpp"



#if WIN32

Socket::Error Socket::ErrorFromWSACode(int error)
{
    switch (error)
    {
        default:
            std::unreachable();
    }
}

#endif // WIN32