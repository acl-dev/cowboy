#include "acl_cpp/lib_acl.hpp"
#include "model.h"
#include "mapper.h"

#include "orders_dao.h"

orders_dao::orders_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

/*@Insert{insert into order(code,customer_id)
                        values (:code,:customer_id)}*/
bool orders_dao:: insert(const orders_t &obj) 
{
	acl::query query;
	query.create_sql("insert into order(code,customer_id) values (:code,:customer_id)");

	query.set_parameter("code", obj.code.c_str());
	query.set_parameter("customer_id", obj.customer_id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("exec_update() failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("commit() failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Delete{delete from order where id=:id}
bool orders_dao:: delete_by_id(int id) 
{
	acl::query query;
	query.create_sql("delete from order where id=:id");

	query.set_parameter("id", id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("exec_update() failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("commit() failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

/*@Update{update order 
                set 
                  code=:code,
                  customer_id=:customer_id 
                where id=:id}*/
bool orders_dao:: update(const orders_t &obj) 
{
	acl::query query;
	query.create_sql("update order set code=:code, customer_id=:customer_id where id=:id");

	query.set_parameter("code", obj.code.c_str());
	query.set_parameter("customer_id", obj.customer_id);
	query.set_parameter("id", obj.id);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_update(query))
	{
		logger_error("exec_update() failed :%s",db_handle_.get_error());
		return false;
	}
	if (!db_handle_.commit())
	{
		logger_error("commit() failed :%s",db_handle_.get_error());
		return false;
	}
	return true;
}

//@Select{select * from order where id=:id}
bool orders_dao:: select_by_id(orders_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select * from order where id=:id");

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

//@Select{select * from order where cname=:name}
bool orders_dao:: select_by_name(std::list<orders_t> &obj, std::string &name) 
{
	acl::query query;
	query.create_sql("select * from order where cname=:name");

	query.set_parameter("name", name.c_str());


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		orders_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $id = (*row)["id"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];


		if($id)
			$item.id = atoi($id);
		if($code)
			$item.code = $code;
		if($customer_id)
			$item.customer_id = atoi($customer_id);

		obj.push_back($item);
	}
	return db_handle_.length() != 0;
}

//@Select{select * from order where customer_id=:cid}
bool orders_dao:: get_customer_ordors(std::list<orders_t> &obj, int cid) 
{
	acl::query query;
	query.create_sql("select * from order where customer_id=:cid");

	query.set_parameter("cid", cid);


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		orders_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $id = (*row)["id"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];


		if($id)
			$item.id = atoi($id);
		if($code)
			$item.code = $code;
		if($customer_id)
			$item.customer_id = atoi($customer_id);

		obj.push_back($item);
	}
	return db_handle_.length() != 0;
}

//@Select{select * from order where customer_id=:cid and code !=:code}
bool orders_dao:: get_customer_ordors_without(std::list<orders_t> &obj, int cid, const std::string &code) 
{
	acl::query query;
	query.create_sql("select * from order where customer_id=:cid and code !=:code");

	query.set_parameter("cid", cid);
	query.set_parameter("code", code.c_str());


	logger("sql: %s", query.to_string().c_str());

	if (!db_handle_.exec_select(query))
	{
		logger_error("exec_select() failed :%s",db_handle_.get_error());
		return false;
	}

	for (size_t i = 0; i < db_handle_.length(); ++i)
	{
		orders_t $item;
		const acl::db_row* row = db_handle_[i];

		const char* $id = (*row)["id"];
		const char* $code = (*row)["code"];
		const char* $customer_id = (*row)["customer_id"];


		if($id)
			$item.id = atoi($id);
		if($code)
			$item.code = $code;
		if($customer_id)
			$item.customer_id = atoi($customer_id);

		obj.push_back($item);
	}
	return db_handle_.length() != 0;
}

