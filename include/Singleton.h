#ifndef SINGLETON_HPP
#define SINGLETON_HPP

/*!
 *  \addtogroup chip8
 *  @{
 */

//! chip8 code
namespace chip8
{

/**
 * @brief Singleton interface class even though singletons are bad
 * 
 * @tparam T type of derived class that will become a singleton upon class inheritance
 */
template<typename T>
class Singleton {
public:
    /**
     * @brief Get singleton instance
     * 
     * @return T& Singleton instance
     */
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

} // namespace chip8

/*! @} End of Doxygen Groups*/

#endif 
