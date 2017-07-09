#pragma once

class order_customer_dao: public order_customer_mapper
{
public:
	order_customer_dao(acl::db_handle& handle);

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
              where o.id=:id and o.customer_id = c.id}*/
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool  get_order_customer(order_customer_t &obj, int id) ;

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
              where o.customer_id = c.id}*/
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool  get_order_customers(std::list<order_customer_t> &obj) ;

	/*@Select{select 
		c.id as cid,
		c.address,
		c.postcode,
		c.sex,
		c.cname,
		o.id as oid,
		o.code,
		o.customer_id 
	   from  customer c, orders o 
 	   where o.customer_id = c.id and c.cname = :name}*/
	//@Result{column=cid, property=customer.id}
	//@Result{column=oid, property=id}
	virtual bool  get_order_customers_by_name(std::list<order_customer_t> &obj,const std::string & name) ;

private:
	acl::db_handle& db_handle_;
};

