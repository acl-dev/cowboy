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

private:
	acl::db_handle& db_handle_;
};

