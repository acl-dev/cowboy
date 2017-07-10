#include "acl_cpp/lib_acl.hpp"
#include "model.h"
#include "mapper.h"

#include "customer_order_dao.h"

static inline bool streq(const char *left, const char *right)
{
	if (!left && !right)
		return true;
	if (left && right)
		return strcmp(left, right) == 0;
	return false;
}

customer_order_dao::customer_order_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

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
bool customer_order_dao:: get_customer_order(customer_order_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select c.id as cid, c.address, c.postcode, c.sex, c.cname, o.id as od, o.code, o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id");

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

		const char* $cid = (*row)["cid"];
		const char* $address = (*row)["address"];
		const char* $postcode = (*row)["postcode"];
		const char* $sex = (*row)["sex"];
		const char* $cname = (*row)["cname"];

		const char* $$oid = (*row)["oid"];
		const char* $$code = (*row)["code"];
		const char* $$customer_id = (*row)["customer_id"];

		if($cid)
			obj.id = atoi($cid);
		if($address)
			obj.address = $address;
		if($postcode)
			obj.postcode = $postcode;
		if($sex)
			obj.sex = $sex;
		if($cname)
			obj.cname = $cname;

		orders_t $item;
		if($$oid)
			$item.id = atoi($$oid);
		if($$code)
			$item.code = $$code;
		if($$customer_id)
			$item.customer_id = atoi($$customer_id);
		obj.orders.push_back($item);
	}
	return !!db_handle_.length();
}

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
bool customer_order_dao:: get_customer_orders(std::list<customer_order_t> &obj) 
{
	acl::query query;
	query.create_sql("select c.id as cid, c.address, c.postcode, c.sex, c.cname, o.id as oid, o.code, o.customer_id from customer c, orders o where o.customer_id=c.id");


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		customer_order_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $cid = (*row)["cid"];
		const char* $address = (*row)["address"];
		const char* $postcode = (*row)["postcode"];
		const char* $sex = (*row)["sex"];
		const char* $cname = (*row)["cname"];


		if($cid)
			$item.id = atoi($cid);
		if($address)
			$item.address = $address;
		if($postcode)
			$item.postcode = $postcode;
		if($sex)
			$item.sex = $sex;
		if($cname)
			$item.cname = $cname;

		for (; i< db_handle_.length(); i++)
		{
			orders_t $$item;
			const char* $$oid = (*row)["oid"];
			const char* $$code = (*row)["code"];
			const char* $$customer_id = (*row)["customer_id"];

			const char* $$$cid = (*row)["cid"];
			const char* $$$address = (*row)["address"];
			const char* $$$postcode = (*row)["postcode"];
			const char* $$$sex = (*row)["sex"];
			const char* $$$cname = (*row)["cname"];

			if (!streq($$$cid, $cid) ||
				!streq($$$address, $address) ||
				!streq($$$postcode, $postcode) ||
				!streq($$$sex, $sex) ||
				!streq($$$cname, $cname))
			{
				--i;
				break;
			}
			if($$oid)
				$$item.id = atoi($$oid);
			if($$code)
				$$item.code = $$code;
			if($$customer_id)
				$$item.customer_id = atoi($$customer_id);

			$item.orders.push_back($$item);

		}

		obj.push_back($item);
	}
	return db_handle_.length() != 0;
}

