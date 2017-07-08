#include "acl_cpp/lib_acl.hpp"
#include "model.h"
#include "mapper.h"
#include "dao.h"

static inline bool streq(const char *left, const char *right)
{
	if (!left && !right)
		return true;
	if (left && right)
		return strcmp(left, right) == 0;
	return false;
}

customer_dao::customer_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

//@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
bool customer_dao::insert(const customer_t &obj) 
{
	acl::query query;
	query.create_sql("insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)");

	query.set_parameter("address", obj.address.c_str());
	query.set_parameter("postcode", obj.postcode.c_str());
	query.set_parameter("sex", obj.sex.c_str());
	query.set_parameter("cname", obj.cname.c_str());


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Delete{delete from customer where id=:id}
bool customer_dao::delete_by_id(int id) 
{
	acl::query query;
	query.create_sql("delete from customer where id=:id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Update{update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id}
bool customer_dao::update(const customer_t &obj) 
{
	acl::query query;
	query.create_sql("update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id");

	query.set_parameter("address", obj.address.c_str());
	query.set_parameter("postcode", obj.postcode.c_str());
	query.set_parameter("sex", obj.sex.c_str());
	query.set_parameter("cname", obj.cname.c_str());
	query.set_parameter("id", obj.id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Select{select * from customer where id=:id}
bool customer_dao::select_by_id(customer_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select * from customer where id=:id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		const acl::db_row* row = db_handle_[i];

		const char* $id = (*row)["id"];
		const char* $address = (*row)["address"];
		const char* $postcode = (*row)["postcode"];
		const char* $sex = (*row)["sex"];
		const char* $cname = (*row)["cname"];

		if($id)
			obj.id = atoi($id);
		if($address)
			obj.address = $address;
		if($postcode)
			obj.postcode = $postcode;
		if($sex)
			obj.sex = $sex;
		if($cname)
			obj.cname = $cname;
	}
	return !!db_handle_.length();
}

orders_dao::orders_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

//@Insert{insert into order(code,customer_id) values (:code,:customer_id)}
bool orders_dao::insert(const orders_t &obj) 
{
	acl::query query;
	query.create_sql("insert into order(code,customer_id) values (:code,:customer_id)");

	query.set_parameter("code", obj.code.c_str());
	query.set_parameter("customer_id", obj.customer_id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Delete{delete from order where id=:id}
bool orders_dao::delete_by_id(int id) 
{
	acl::query query;
	query.create_sql("delete from order where id=:id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Update{update order set code=:code,customer_id=:customer_id where id=:id}
bool orders_dao::update(const orders_t &obj) 
{
	acl::query query;
	query.create_sql("update order set code=:code,customer_id=:customer_id where id=:id");

	query.set_parameter("code", obj.code.c_str());
	query.set_parameter("customer_id", obj.customer_id);
	query.set_parameter("id", obj.id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("db_.exec_update failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("db_.commit failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Select{select * from order where id=:id}
bool orders_dao::select_by_id(orders_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select * from order where id=:id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		const acl::db_row* row = db_handle_[i];

		const char* $id = (*row)["id"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];

		if($id)
			obj.id = atoi($id);
		if($code)
			obj.code = $code;
		if($customer_id)
			obj.customer_id = atoi($customer_id);
	}
	return !!db_handle_.length();
}

customer_order_dao::customer_order_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
//@Result{column=cid, property=id}
//@Result{column=oid, property=orders.id}
bool customer_order_dao::get_customer_order(customer_order_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
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

		orders_t $_obj;
		if($$oid)
			$_obj.id = atoi($$oid);
		if($$code)
			$_obj.code = $$code;
		if($$customer_id)
			$_obj.customer_id = atoi($$customer_id);
		obj.orders.push_back($_obj);
	}
	return !!db_handle_.length();
}

//@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
//@Result{column=cid, property=id}
//@Result{column=oid, property=orders.id}
bool customer_order_dao::get_customer_orders(std::list<customer_order_t> &obj) 
{
	acl::query query;
	query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id");


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
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
			orders_t orders_t_item;
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
				orders_t_item.id = atoi($$oid);
			if($$code)
				orders_t_item.code = $$code;
			if($$customer_id)
				orders_t_item.customer_id = atoi($$customer_id);

			$item.orders.push_back(orders_t_item);

		}

		obj.push_back($item);
	}
	return !!db_handle_.length();
}

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
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
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
			obj.customer.id = atoi($$cid);
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
		logger_error("db_.exec_select failed :%s",db_handle_.get_error());
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
			$item.customer.id = atoi($$cid);
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

