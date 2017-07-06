//@Mappers
#pragma once

//@Mapper
struct customer_mapper
{
    //@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
    virtual void insert(const customer &obj) = 0;

    //@Delete{delete from customer where id=:id}
    virtual void delete_by_id(int id) = 0;

    //@Update{update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id}
    virtual void update(const customer &obj) = 0;

    //@Select{select * from customer where id=:id}
    virtual customer select_by_id(int id) = 0;

};

//@Mapper
struct orders_mapper 
{
    //@Insert{insert into order(code,customer_id) values (:code,:customer_id)}
    virtual void insert(const orders &obj) = 0;

    //@Delete{delete from order where id=:id}
    virtual void delete_by_id(int id) = 0;

    //@Update{update order set code=:code,customer_id=:customer_id where id=:id}
    virtual void update(const orders &obj) = 0;

    //@Select{select * from order where id=:id}
    virtual orders select_by_id(int id) = 0;
};

//@Mapper
struct customer_order_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
    //@Result{column=cid, property=customer_order.id}
    //@Result{column=oid, property=order.id}
    virtual customer_order  get_customer_order(int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
    //@Result{column=cid, property=customer_order.id}
    //@Result{column=oid, property=order.id}
    virtual std::list<customer_order> get_customer_orders() = 0;
};

//@Mapper
struct order_customer_mapper
{
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual order_customer get_order_customer(int id) = 0;

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
    //@Result{column=cid, property=customer.id}
    //@Result{column=oid, property=id}
    virtual std::list<order_customer> get_order_customers() = 0;
};