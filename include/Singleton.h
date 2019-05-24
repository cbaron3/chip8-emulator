#ifndef SINGLETON_HPP
#define SINGLETON_HPP

namespace chip8
{

// Singletons are bad :D
template<typename T>
class Singleton {
public:
    // Singleton instance
    static T& instance()
    {
        static T instance;
        return instance;
    }

    // Delete copying for singleton
    Singleton(const Singleton&) = delete;
    Singleton& operator= (const Singleton) = delete;

protected:
    Singleton() {}
    ~Singleton() {}
};

}

#endif 
