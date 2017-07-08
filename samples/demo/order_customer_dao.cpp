#include "acl_cpp/lib_acl.hpp"
#include "model.h"
#include "mapper.h"

#include "order_customer_dao.h"

order_customer_dao::order_customer_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
//@Result{column=cid, property=customer.id}
//@Result{column=oid, property=id}
bool order_customer_dao::get_order_customer(order_customer_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		const acl::db_row* row = db_handle_[i];

		const char* $oid = (*row)["oid"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];

		const char* $$cid = (*row)["cid"];
		const char* $$address = (*row)["address"];
		const char* $$postcode = (*row)["postcode"];
		const char* $$sex = (*row)["sex"];
		const char* $$cname = (*row)["cname"];

		if($oid)
			obj.id = atoi($oid);
		if($code)
			obj.code = $code;
		if($customer_id)
			obj.customer_id = atoi($customer_id);

		if($$cid)
			obj.customer.id = atoll($$cid);
		if($$address)
			obj.customer.address = $$address;
		if($$postcode)
			obj.customer.postcode = $$postcode;
		if($$sex)
			obj.customer.sex = $$sex;
		if($$cname)
			obj.customer.cname = $$cname;
	}
	return !!db_handle_.length();
}

//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
//@Result{column=cid, property=customer.id}
//@Result{column=oid, property=id}
bool order_customer_dao::get_order_customers(std::list<order_customer_t> &obj) 
{
	acl::query query;
	query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id");


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		order_customer_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $oid = (*row)["oid"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];


		const char* $$cid = (*row)["cid"];
		const char* $$address = (*row)["address"];
		const char* $$postcode = (*row)["postcode"];
		const char* $$sex = (*row)["sex"];
		const char* $$cname = (*row)["cname"];

		if($oid)
			$item.id = atoi($oid);
		if($code)
			$item.code = $code;
		if($customer_id)
			$item.customer_id = atoi($customer_id);

		if($$cid)
			$item.customer.id = atoll($$cid);
		if($$address)
			$item.customer.address = $$address;
		if($$postcode)
			$item.customer.postcode = $$postcode;
		if($$sex)
			$item.customer.sex = $$sex;
		if($$cname)
			$item.customer.cname = $$cname;

		obj.push_back($item);
	}
	return !!db_handle_.length();
}

//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id and c.cname = :name}
//@Result{column=cid, property=customer.id}
//@Result{column=oid, property=id}
bool order_customer_dao::get_order_customers_by_name(std::list<order_customer_t> &obj,const std::string & name) 
{
	acl::query query;
	query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id and c.cname = :name");

	query.set_parameter("name", name.c_str());


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		order_customer_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $oid = (*row)["oid"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];


		const char* $$cid = (*row)["cid"];
		const char* $$address = (*row)["address"];
		const char* $$postcode = (*row)["postcode"];
		const char* $$sex = (*row)["sex"];
		const char* $$cname = (*row)["cname"];

		if($oid)
			$item.id = atoi($oid);
		if($code)
			$item.code = $code;
		if($customer_id)
			$item.customer_id = atoi($customer_id);

		if($$cid)
			$item.customer.id = atoll($$cid);
		if($$address)
			$item.customer.address = $$address;
		if($$postcode)
			$item.customer.postcode = $$postcode;
		if($$sex)
			$item.customer.sex = $$sex;
		if($$cname)
			$item.customer.cname = $$cname;

		obj.push_back($item);
	}
	return !!db_handle_.length();
}

