## what is cowboy ?
cowboy is c++ orm working boy :)
it parse models and mappers and then generate dao files.
it like myBatis. but without verbose xml config files

### how to build?
build acl first
```
git clone https://github.com/acl-dev/acl.git
cd acl
make build_one

```
build cowboy.it depend on acl. run "cmake . -DACL_ROOT=acl_path" to generate CMakeFile. and acl_path is acl's path.eg:
/home/akzi/code/acl/
```
git clone https://github.com/acl-dev/cowboy.git
cd cowboy
mkdir cmake_build
cd cmake_build 
cmake .. -DACL_ROOT=acl_path
```
## how it work?
cowboy parse model files and mappers,and generate c++ code.

## how to use
* first define models

```cpp
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
        int a = 2;
        a++;

    }
    long long int id;
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
```

* second define mapper

```cpp
//@Mappers
#pragma once

//@Mapper
struct customer_mapper
{
    //@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
    virtual bool insert(const customer_t &obj) = 0;

    //@Delete{delete from customer where id=:id}
    virtual bool delete_by_id(int id) = 0;

    //@Update{update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id}
    virtual bool update(const customer_t &obj) = 0;

    //@Update{update customer set address=:new_address where id=:id}
    virtual bool update_address(const customer_t &obj,const std::string &new_address) = 0;

    //@Select{select * from customer where id=:id}
    virtual bool select_by_id(customer_t &obj, int id) = 0;
};

//@Mapper
struct orders_mapper 
{
    //@Insert{insert into order(code,customer_id) values (:code,:customer_id)}
    virtual bool insert(const orders_t &obj) = 0;

    //@Delete{delete from order where id=:id}
    virtual bool delete_by_id(int id) = 0;

    //@Update{update order set code=:code,customer_id=:customer_id where id=:id}
    virtual bool update(const orders_t &obj) = 0;

    //@Select{select * from order where id=:id}
    virtual bool select_by_id(orders_t &obj, int id) = 0;

    //@Select{select * from order where cname=:name}
    virtual bool select_by_name(std::list<orders_t> &obj, std::string &name) = 0;

    //@Select{select * from order where customer_id=:cid}
    virtual bool get_customer_ordors(std::list<orders_t> &obj, int cid) = 0;

    //@Select{select * from order where customer_id=:cid and code !=:code}
    virtual bool get_customer_ordors_without(std::list<orders_t> &obj, int cid, const std::string &code) = 0;
};

//@Mapper
struct customer_order_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
    //@Result{column=cid, property=id}
    //@Result{column=oid, property=orders.id}
    virtual bool get_customer_order(customer_order_t &obj, int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
    //@Result{column=cid, property=id}
    //@Result{column=oid, property=orders.id}
    virtual bool get_customer_orders(std::list<customer_order_t> &obj) = 0;
};

//@Mapper
struct order_customer_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual bool get_order_customer(order_customer_t &obj, int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual bool get_order_customers(std::list<order_customer_t> &obj) = 0;


    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id and c.cname = :name}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual bool get_order_customers_by_name(std::list<order_customer_t> &obj,const std::string & name) = 0;
};
```

* generate c++ code

```
    akzi@akzi:~/code/dao_generator/cmake_build/dao$ ls
    CMakeCache.txt  CMakeFiles  cmake_install.cmake  dao  gen_dao  Makefile  mapper.h  model.h
    ./gen_dao -s . -d ./dao
    akzi@akzi:~/code/dao_generator/cmake_build/dao$ ls
    dao.cpp  dao.h
```
it will generate dao files.

show dao.h file

```cpp
#pragma once

class customer_dao: public customer_mapper
{
public:
    customer_dao(acl::db_handle& handle);

	//@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
	virtual bool insert(const customer_t &obj) ;

	//@Delete{delete from customer where id=:id}
	virtual bool delete_by_id(int id) ;

	//@Update{update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id}
	virtual bool update(const customer_t &obj) ;

	//@Update{update customer set address=:new_address where id=:id}
	virtual bool update_address(const customer_t &obj,const std::string &new_address) ;

	//@Select{select * from customer where id=:id}
	virtual bool select_by_id(customer_t &obj, int id) ;

private:
	acl::db_handle& db_handle_;
};

class orders_dao: public orders_mapper
{
public:
	orders_dao(acl::db_handle& handle);

	//@Insert{insert into order(code,customer_id) values (:code,:customer_id)}
	virtual bool insert(const orders_t &obj) ;

	//@Delete{delete from order where id=:id}
	virtual bool delete_by_id(int id) ;

	//@Update{update order set code=:code,customer_id=:customer_id where id=:id}
	virtual bool update(const orders_t &obj) ;

	//@Select{select * from order where id=:id}
	virtual bool select_by_id(orders_t &obj, int id) ;

	//@Select{select * from order where cname=:name}
	virtual bool select_by_name(std::list<orders_t> &obj, std::string &name) ;

	//@Select{select * from order where customer_id=:cid}
	virtual bool get_customer_ordors(std::list<orders_t> &obj, int cid) ;

	//@Select{select * from order where customer_id=:cid and code !=:code}
	virtual bool get_customer_ordors_without(std::list<orders_t> &obj, int cid, const std::string &code) ;

private:
	acl::db_handle& db_handle_;
};

class customer_order_dao: public customer_order_mapper
{
public:
	customer_order_dao(acl::db_handle& handle);

	//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
	//@Result{column=cid, property=id}
	//@Result{column=oid, property=orders.id}
	virtual bool get_customer_order(customer_order_t &obj, int id) ;

	//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
	//@Result{column=cid, property=id}
	//@Result{column=oid, property=orders.id}
	virtual bool get_customer_orders(std::list<customer_order_t> &obj) ;

private:
	acl::db_handle& db_handle_;
};

class order_customer_dao: public order_customer_mapper
{
public:
	order_customer_dao(acl::db_handle& handle);

	//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool get_order_customer(order_customer_t &obj, int id) ;

	//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool get_order_customers(std::list<order_customer_t> &obj) ;

	//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id and c.cname = :name}
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool get_order_customers_by_name(std::list<order_customer_t> &obj,const std::string & name) ;

private:
	acl::db_handle& db_handle_;
};
```


