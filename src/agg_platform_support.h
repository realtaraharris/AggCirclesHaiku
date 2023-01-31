#ifndef AGG_PLATFORM_SUPPORT_INCLUDED
#define AGG_PLATFORM_SUPPORT_INCLUDED

namespace agg {
    // A predeclaration of the platform dependent class. Since we do not
    // know anything here the only we can have is just a pointer to this
    // class as a data member. It should be created and destroyed explicitly
    // in the constructor/destructor of the platform_support class. 
    // Although the pointer to platform_specific is public the application 
    // cannot have access to its members or methods since it does not know
    // anything about them and it's a perfect incapsulation :-)
    class platform_specific;
}

#endif
