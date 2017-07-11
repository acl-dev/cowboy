//@Models
#pragma once
#include <string>
#include <list>

//@Model
//@Table{customer}
struct customer_t
{
    customer_t()
    {
	    id = 0;
    }
    int id;
    std::string address;

    //@Column{postcode}
    std::string postcode ;

    //@Column{sex}
    std::string sex;

    //@Column{cname}
    std::string cname;
};

//@Model
//@Table{orders}
struct orders_t
{
    orders_t()
    {
        id = 0;
        customer_id = 0;

    }
    int id;
    std::string code;
    int customer_id;
};


//@Model
struct customer_order_t :customer_t
{
    std::list<orders_t> orders;
};

//@Model
struct order_customer_t :orders_t
{
    customer_t customer;
};