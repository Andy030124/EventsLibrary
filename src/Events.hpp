#pragma once

#ifdef FORWARD
    #include "./Listener/Event.hpp"
    #include "./Listener/Handler.hpp"
#else
    #include "./Listener/Impl.tpp"
#endif
