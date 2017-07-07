#pragma once
class customer_dao :customer_mapper
{
public:
    customer_dao(acl::db_handle& db)
        :db_(db)
    {

    }
    //@Insert{insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)}
    virtual bool insert(const customer &obj)
    {
        acl::query query;
        query.create_sql("insert into customer(address,postcode,sex,cname) values (:address,:postcode,:sex,:cname)");
        query.set_parameter("address",obj.address.c_str());
        query.set_parameter("postcode",obj.postcode.c_str());
        query.set_parameter("sex", obj.sex.c_str());
        query.set_parameter("cname", obj.cname.c_str());

        logger("%s", query.to_string().c_str());

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }
            

        if (db_.commit() == false)
        {
            logger_error("db_.commit() failed. %s", db_.get_error());
            return false;
        }
        return true;
    }

    //@Delete{delete from customer where id=:id}
    virtual bool delete_by_id(int id)
    {
        acl::query query;
        query.create_sql("delete from customer where id=:id");
        query.set_parameter("id", id);

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }

        if (db_.commit() == false)
        {
            logger_error("db_.commit() failed. %s", db_.get_error());
            return false;
        }
        return true;
    }

    //@Update("update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id")
    virtual bool  update(const customer &obj)
    {
        acl::query query;

        query.create_sql("update customer set address=:address,postcode=:postcode,sex=:sex,cname=:cname where id=:id");
        query.set_parameter("address", obj.address.c_str());
        query.set_parameter("postcode", obj.postcode.c_str());
        query.set_parameter("sex", obj.sex.c_str());
        query.set_parameter("cname", obj.cname.c_str());

        logger("%s", query.to_string().c_str());

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }

        if (db_.commit() == false)
        {
            logger_error("db_.commit() failed. %s", db_.get_error());
            return false;
        }
        return true;
    }

    //@Select{select * from customer where id=:id}
    virtual bool select_by_id(customer  &obj, int id)
    {
        acl::query query;
        query.create_sql("select * from customer where id=:id");
        query.set_parameter("id",id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }

        if(db_.length())
        {
            const acl::db_row* row = db_[0];
            const char* address = (*row)["address"];
            const char* postcode = (*row)["postcode"];
            const char* sex = (*row)["sex"];
            const char* cname = (*row)["cname"];
            const char* id = (*row)["id"];

            if (address)
                obj.address = address;
            if (postcode)
                obj.postcode = postcode;
            if (sex)
                obj.sex = sex;
            if (cname)
                obj.cname = cname;
            if (id)
                obj.id = atoi(id);
        }
        return true;
    }

private:
    acl::db_handle& db_;
};


class orders_dao:public orders_mapper
{
public:
    orders_dao(acl::db_handle& db)
        :db_(db)
    {

    }
    //@Insert{insert into orders(code,customer_id) values (:code,:customer_id)}
    virtual bool insert(const orders &obj)
    {
        acl::query query;
        query.create_sql("insert into orders(code,customer_id)values(:code,:customer_id)");
        query.set_parameter("code", obj.code.c_str());
        query.set_parameter("customer_id", obj.customer_id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }

        if (db_.commit() == false)
        {
            logger_error("db_.commit() failed. %s", db_.get_error());
            return false;
        }
        return true;
    }

    //@Delete{delete from order where id=:id}
    virtual bool delete_by_id(int id)
    {
        acl::query query;
        query.create_sql("delete from order where id=:id");
        query.set_parameter("id", id);

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }

        if (db_.commit() == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }
        return true;
    }

    //@Update{update order set code=:code,customer_id=:customer_id where id=:id}
    virtual bool update(const orders &obj)
    {
        acl::query query;

        query.create_sql("update order set code=:code,customer_id=:customer_id where id=:id");
        query.set_parameter("code", obj.code.c_str());
        query.set_parameter("customer_id", obj.customer_id);
        query.set_parameter("id", obj.id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_update(query) == false)
        {
            logger_error("db_.exec_update() failed. %s", db_.get_error());
            return false;
        }

        if (db_.commit() == false)
        {
            logger_error("db_.commit() failed. %s", db_.get_error());
            return false;
        }
    }

    //@Select{select * from order where id=:id}
    virtual bool select_by_id(orders &obj ,int id)
    {
        acl::query query;
        query.create_sql("select * from customer where id=:id");
        query.set_parameter("id", id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }
        if (db_.length())
        {
            const acl::db_row* row = db_[0];
            const char* code = (*row)["code"];
            const char* customer_id = (*row)["customer_id"];
            const char* id = (*row)["id"];

            if (code)
                obj.code = code;
            if (customer_id)
                obj.customer_id = atoi(customer_id);
            if (id)
                obj.id = atoi(id);
        }
        return true;
    }
private:
    acl::db_handle& db_;
};

class customer_order_dao:public customer_order_mapper
{

public:
    customer_order_dao(acl::db_handle& db)
        :db_(db)
    {

    }
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as od,o.code,o.customer_id from customer c, order o where c.id=:id and o.customer_id=c.id}
    //@Result column="cid" property="id"
    //@Result column="oid" property="order.id"
    virtual bool get_customer_order(customer_order  &obj, int id)
    {
        acl::query query;
        query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where c.id=:id and o.customer_id=c.id");
        query.set_parameter("id", id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }
        for (size_t i = 0; i < db_.length(); ++i)
        {
            const acl::db_row* row = db_[i];

            const char* address  = (*row)["address"];
            const char* postcode = (*row)["postcode"];
            const char* sex      = (*row)["sex"];
            const char* cname    = (*row)["cname"];
            const char* cid      = (*row)["cid"];

            const char* code = (*row)["code"];
            const char* customer_id = (*row)["customer_id"];
            const char* oid = (*row)["oid"];

            if (address)
                obj.address = address;
            if (postcode)
                obj.postcode = postcode;
            if (sex)
                obj.sex = sex;
            if (cname)
                obj.cname = cname;
            if (cid)
                obj.id = atoi(cid);

            orders o;

            if (code)
                o.code = code;
            if (oid)
                o.id = atoi(oid);
            if (customer_id)
                o.customer_id = atoi(customer_id);
            obj.orders.push_back(o);
        }
        return !!db_.length();
    }

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id}
    //@Result column="cid" property="id"
    //@Result column="oid" property="order.id"
    virtual bool get_customer_orders(std::list<customer_order> &obj)
    {
        acl::query query;
        query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where  o.customer_id=c.id");

        logger("%s", query.to_string().c_str());

        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }

        for (size_t i = 0; i < db_.length(); ++i)
        {

            customer_order customer_order_obj;

            const acl::db_row* row = db_[i];

            const char* cid      = (*row)["cid"];
            const char* address  = (*row)["address"];
            const char* postcode = (*row)["postcode"];
            const char* sex      = (*row)["sex"];
            const char* cname    = (*row)["cname"];

            

            if (address)
                customer_order_obj.address = address;
            if (postcode)
                customer_order_obj.postcode = postcode;
            if (sex)
                customer_order_obj.sex = sex;
            if (cname)
                customer_order_obj.cname = cname;
            if (cid)
                customer_order_obj.id = atoi(cid);

            for (; i< db_.length(); i++)
            {
                orders orders_obj;
                const acl::db_row* row1 = db_[i];

                const char* code        = (*row1)["code"];
                const char* customer_id = (*row1)["customer_id"];
                const char* oid         = (*row1)["oid"];

                const char* cid         = (*row1)["cid"];
                const char* $address1    = (*row1)["address"];
                const char* postcode1   = (*row1)["postcode"];
                const char* sex1        = (*row1)["sex"];
                const char* cname1      = (*row1)["cname"];

                if (!streq(cid, cid) ||
                    !streq($address1, address) ||
                    !streq(postcode1, postcode) ||
                    !streq(sex1, sex) ||
                    !streq(cname1, cname))
                {
                    i--;
                    break;
                }

                if (code)
                    orders_obj.code = code;
                if (oid)
                    orders_obj.id = atoi(oid);
                if (customer_id)
                    orders_obj.customer_id = atoi(customer_id);

                customer_order_obj.orders.push_back(orders_obj);

               
            }
            obj.push_back(customer_order_obj);
        }
        return !!db_.length();
    }
private:

    acl::db_handle& db_;
};

class order_customer_dao:public order_customer_mapper
{
public:
    order_customer_dao(acl::db_handle& db)
        :db_(db)
    {

    }
    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id}
    //@Result column="cid" property="customer.id"
    //@Result column="oid" property="id"
    virtual bool get_order_customer(order_customer &obj, int id)
    {
        acl::query query;
        query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.id=:id and o.customer_id = c.id");
        query.set_parameter("id", id);

        logger("%s", query.to_string().c_str());

        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }

        if(db_.length())
        {
            const acl::db_row* row = db_[0];

            const char* address = (*row)["address"];
            const char* postcode = (*row)["postcode"];
            const char* sex = (*row)["sex"];
            const char* cname = (*row)["cname"];
            const char* cid = (*row)["cid"];

            const char* code = (*row)["code"];
            const char* customer_id = (*row)["customer_id"];
            const char* oid = (*row)["oid"];

            if (address)
                obj.customer.address = address;
            if (postcode)
                obj.customer.postcode = postcode;
            if (sex)
                obj.customer.sex = sex;
            if (cname)
                obj.customer.cname = cname;
            if (cid)
                obj.customer.id = atoi(cid);

            if (code)
                obj.code = code;
            if (oid)
                obj.id = atoi(oid);
            if (customer_id)
                obj.customer_id = atoi(customer_id);
        }
        return !!db_.length();
    }

    //@Select{select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id}
    //@Result column="cid" property="customer_order.id"
    //@Result column="oid" property="order.id"
    virtual bool get_order_customers(std::list<order_customer> &result)
    {
        acl::query query;

        query.create_sql("select c.id as cid,c.address,c.postcode,c.sex,c.cname,o.id as oid,o.code,o.customer_id from customer c, orders o where o.customer_id = c.id");
        logger("%s", query.to_string().c_str());
        if (db_.exec_select(query) == false)
        {
            logger_error("db_.exec_select() failed. %s", db_.get_error());
            return false;
        }

        for(size_t i = 0; i < db_.length(); i++)
        {
            order_customer obj;
            const acl::db_row* row = db_[i];

            const char* address     = (*row)["address"];
            const char* postcode    = (*row)["postcode"];
            const char* sex         = (*row)["sex"];
            const char* cname       = (*row)["cname"];
            const char* cid         = (*row)["cid"];

            const char* code        = (*row)["code"];
            const char* customer_id = (*row)["customer_id"];
            const char* oid         = (*row)["oid"];

            if (address)
                obj.customer.address = address;
            if (postcode)
                obj.customer.postcode = postcode;
            if (sex)
                obj.customer.sex = sex;
            if (cname)
                obj.customer.cname = cname;
            if (cid)            
                obj.customer.id = atoi(cid);

            if (code)
                obj.code = code;
            if (oid)
                obj.id = atoi(oid);
            if (customer_id)
                obj.customer_id = atoi(customer_id);

            result.push_back(obj);
        }
        return !!db_.length();
    }
private:
    acl::db_handle& db_;
};