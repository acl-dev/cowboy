//@Mappers
#pragma once

//@Mapper
struct customer_mapper
{
    //@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
    virtual bool insert(const customer &obj) = 0;

    //@Delete{delete from customer where id=:id}
    virtual bool delete_by_id(int id) = 0;

    //@Update{update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id}
    virtual bool update(const customer &obj) = 0;

    //@Select{select * from customer where id=:id}
    virtual bool select_by_id(customer  &obj, int id) = 0;

};

//@Mapper
struct orders_mapper 
{
    //@Insert{insert into order(code,customer_id) values (:code,:customer_id)}
    virtual bool insert(const orders &obj) = 0;

    //@Delete{delete from order where id=:id}
    virtual bool delete_by_id(int id) = 0;

    //@Update{update order set code=:code,customer_id=:customer_id where id=:id}
    virtual bool update(const orders &obj) = 0;

    //@Select{select * from order where id=:id}
    virtual bool select_by_id(orders &obj, int id) = 0;
};

//@Mapper
struct customer_order_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
    //@Result{column=cid, property=id}
    //@Result{column=oid, property=orders.id}
    virtual bool get_customer_order(customer_order &obj, int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
    //@Result{column=cid, property=id}
    //@Result{column=oid, property=orders.id}
    virtual bool get_customer_orders(std::list<customer_order> &obj) = 0;
};

//@Mapper
struct order_customer_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual bool get_order_customer(order_customer &obj, int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual bool get_order_customers(std::list<order_customer> &obj) = 0;
};