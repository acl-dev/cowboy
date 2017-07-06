//@Models
#pragma once
#include <string>
#include <list>

//@Model
//@Table{customer}
struct customer 
{
    customer()
    {
        //}}}
        /*}}}}}*/
        /*
        }
        }
        }
        */
        id = 0;
        int a = 2;
        a++;

    }
    int id;
    std::string address;

    //@Column{postcode}
    std::string postcode;

    //@Column{sex}
    std::string sex;

    //@Column{cname}
    std::string cname;
};

//@Model
//@Table{orders}
struct orders
{
    int id;
    std::string code;
    int customer_id;
};

//@Model
struct customer_order :customer 
{
    //一对多
    std::list<orders> orders;
};

//@Model
struct order_customer :orders
{
    //多对一
    customer customer;
};