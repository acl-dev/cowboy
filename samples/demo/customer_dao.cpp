#include "acl_cpp/lib_acl.hpp"
#include "model.h"
#include "mapper.h"

#include "customer_dao.h"

customer_dao::customer_dao(acl::db_handle& handle)
	:db_handle_(handle)
{
}

/*@Insert{insert into customer(address,postcode,sex,cname) 
							values (:address,:postcode,:sex,:cname)}*/
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

//@Delete{delete from customer where id=:id}
bool customer_dao::delete_by_id(int id) 
{
	acl::query query;
	query.create_sql("delete from customer where id=:id");

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

/*@Update{update customer 
               set 
                address=:address,
                postcode=:postcode,
                sex=:sex,
                cname=:cname 
               where id=:id}*/
bool customer_dao::update(const customer_t &obj) 
{
	acl::query query;
	query.create_sql("update customer set address=:address, postcode=:postcode, sex=:sex, cname=:cname where id=:id");

	query.set_parameter("address", obj.address.c_str());
	query.set_parameter("postcode", obj.postcode.c_str());
	query.set_parameter("sex", obj.sex.c_str());
	query.set_parameter("cname", obj.cname.c_str());
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

//@Update{update customer set address=:new_address where id=:id}
bool customer_dao::update_address(const customer_t &obj,const std::string &new_address) 
{
	acl::query query;
	query.create_sql("update customer set address=:new_address where id=:id");

	query.set_parameter("new_address", new_address.c_str());
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

//@Select{select * from customer where id=:id}
bool customer_dao::select_by_id(customer_t &obj, int id) 
{
	acl::query query;
	query.create_sql("select * from customer where id=:id");

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
		const char* $address = (*row)["address"];
		const char* $postcode = (*row)["postcode"];
		const char* $sex = (*row)["sex"];
		const char* $cname = (*row)["cname"];

		if($id)
			obj.id = atoll($id);
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

