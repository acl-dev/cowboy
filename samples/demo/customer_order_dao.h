#pragma once

class customer_order_dao: public customer_order_mapper
{
public:
	customer_order_dao(acl::db_handle& handle);

	/*@Select{select 
               c.id as cid,
               c.address,
               c.postcode,
               c.sex,
               c.cname,
               o.id as od,
               o.code,
               o.customer_id 
             from customer c, order o 
             where c.id=:id and o.customer_id=c.id}*/
	//@Result{column=cid, property=id}
	//@Result{column=oid, property=orders.id}
	virtual bool  get_customer_order(customer_order_t &obj, int id) ;

	/*@Select{select 
               c.id as cid,
               c.address,
               c.postcode,
               c.sex,
               c.cname,
               o.id as oid,
               o.code,
               o.customer_id 
              from customer c, orders o 
              where  o.customer_id=c.id}*/
	//@Result{column=cid, property=id}
	//@Result{column=oid, property=orders.id}
	virtual bool  get_customer_orders(std::list<customer_order_t> &obj) ;

private:
	acl::db_handle& db_handle_;
};

