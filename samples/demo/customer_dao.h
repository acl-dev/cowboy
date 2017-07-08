#pragma once

class customer_dao: public customer_mapper
{
public:
	customer_dao(acl::db_handle& handle);

	/*@Insert{insert into customer(address,postcode,sex,cname) 
							values (:address,:postcode,:sex,:cname)}*/
	virtual bool insert(const customer_t &obj) ;

	//@Delete{delete from customer where id=:id}
	virtual bool delete_by_id(int id) ;

	/*@Update{update customer 
               set 
                address=:address,
                postcode=:postcode,
                sex=:sex,
                cname=:cname 
               where id=:id}*/
	virtual bool update(const customer_t &obj) ;

	//@Update{update customer set address=:new_address where id=:id}
	virtual bool update_address(const customer_t &obj,const std::string &new_address) ;

	//@Select{select * from customer where id=:id}
	virtual bool select_by_id(customer_t &obj, int id) ;

private:
	acl::db_handle& db_handle_;
};

