#ifndef PROPERTY_H_
#define PROPERTY_H_

#include <functional>

template <typename T>
/**
 * Property class for PipEngine.
 * Example:
 * 
 * Property<int> prop = Property<int>(
 *     [&prop]()->int { return prop.value * 2; },
 *     [&prop](int value) { prop.value = value * -2; }
 * );
 * 
 * prop = -2;
 * std::cout << prop << std::endl; // prints 8
 *
 * prop = 2;
 * std::cout << prop << std::endl; // prints -8
 *
 * prop.get = [&prop]()->int { return prop.value / 2; };
 * std::cout << prop << std::endl; // prints -2
 */
struct Property 
{
    /**
    * Stores current property value.
    */
    T value;

    /**
    * Function that returns property value.
    */
    std::function<T()> get;

    /**
    * Function that sets property to specified value.
    */
    std::function<void(T value)> set;
 
    /**
    * Default constructor for property.
    */
    Property()
    {
	    this->get = [this]()->T { return this->value; };
	    this->set = [this](T value) { this->value = value; };
    }

    /**
    * Constructor that defines set and get methods.
    */
    Property(
        std::function<T()> get,
        std::function<void(T)> set  
    ) { 
	    this->get = get;
	    this->set = set;
    }

    /**
     * Constructor that sets property to specified value using default set method.
     */

    Property(T value) : Property() 
    { 
        this->set(value);
    }

    /**
     * Constructor that sets property to speciified value using default set method and defines new get method.
     */
    Property(T value, std::function<T()> get) : Property(get)
    { 
        this->set(value);
    }

    /**
     * Constructor that sets property to given value using specified set method and defines set and get methods.
     */
    Property(T value, std::function<T()> get, std::function<void(T)> set) : Property(get, set)
    { 
        this->set(value);
    }
 
    /**
     * Sets property value.
     */
    Property& operator = (const T &value) 
    {
	    this->set(value);
        return *this;
    }

    /**
     * Sets property value.
     */
    Property& operator = (const Property<T> &value) 
    {
	    this->set(value.get());
        return *this;
    }
 
    /**
     * Gets property value.
     */
    operator T() const { return this->get(); }
};
#endif