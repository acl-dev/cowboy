#pragma once

class orders_dao: public orders_mapper
{
public:
	orders_dao(acl::db_handle& handle);

	/*@Insert{insert into order(code,customer_id)
                        values (:code,:customer_id)}*/
	virtual bool insert(const orders_t &obj) ;

	//@Delete{delete from order where id=:id}
	virtual bool delete_by_id(int id) ;

	/*@Update{update order 
                set 
                  code=:code,
                  customer_id=:customer_id 
                where id=:id}*/
	virtual bool update(const orders_t &obj) ;

	//@Select{select * from order where id=:id}
	virtual bool select_by_id(orders_t &obj, int id) ;

	//@Select{select * from order where cname=:name}
	virtual bool select_by_name(std::list<orders_t> &obj, std::string &name) ;

	//@Select{select * from order where customer_id=:cid}
	virtual bool get_customer_ordors(std::list<orders_t> &obj, int cid) ;

	//@Select{select * from order where customer_id=:cid and code !=:code}
	virtual bool get_customer_ordors_without(std::list<orders_t> &obj, int cid, const std::string &code) ;

private:
	acl::db_handle& db_handle_;
};

