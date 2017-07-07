
#include <iostream>
#include <exception>
#include "acl_cpp/lib_acl.hpp"
#include "helper.hpp"
#include "create_tables.hpp"
#include "model.hpp"
#include "mapper.hpp"
#include "dao.hpp"
#include "../../include/dao_generator.h"


void customer_mapper_dao_insert(acl::db_mysql &db)
{
    customer_dao dao(db);
    customer _customer;
    _customer.address = "hello_akzi";
    _customer.cname = "hello_akzi";
    _customer.postcode = "0000000";
    _customer.sex = "male";
    dao.insert(_customer);
}

void order_mapper_dao_insert(acl::db_mysql &db)
{
    orders o;
    o.code = "hello_akzi";
    o.customer_id = 2;

    orders_dao dao(db);
    dao.insert(o);
    o.code = "hello_akzi2";
    dao.insert(o);
}

void customer_mapper_dao_select(acl::db_mysql &db)
{
    customer_dao dao(db);
    customer obj;
    dao.select_by_id(obj,1);
}

void customer_order_dao_test(acl::db_mysql &db)
{
    customer_order_dao dao(db);
    customer_order o;
    dao.get_customer_order(o, 1);
}

void get_customer_orders_test(acl::db_mysql &db)
{
    customer_order_dao dao(db);

    std::list<customer_order> list;
    dao.get_customer_orders(list);
}

void order_customer_dao_test(acl::db_mysql &db)
{
    order_customer_dao dao(db);

    order_customer o;
    dao.get_order_customer(o, 1);

    std::list<order_customer> list;
    dao.get_order_customers(list);
}

int db_test(void)
{
    acl::acl_cpp_init();

    acl::log::stdout_open(true);

    acl::db_handle::set_loadpath("F:/fork/acl-dev/libmysql.dll");

    acl::string dbaddr("192.168.3.170:3306");
    acl::string dbname("orm_test"), dbuser("root"), dbpass("skyinno251");

    create_db(dbaddr,dbname,dbuser,dbpass);
    create_customer_table(dbaddr, dbname, dbuser, dbpass);
    create_orders_table(dbaddr, dbname, dbuser, dbpass);

    acl::db_mysql db(dbaddr, dbname, dbuser, dbpass, 0, false);
    if (db.open() == false)
    {
        logger_error("open db(%s@%s) error\r\n", dbaddr.c_str(), dbname.c_str());
        return 1;
    }

    order_customer_dao_test(db);

    return 0;
}


int main()
{
    acl::dao_generator gen;

    gen.parse_file("F:\\fork\\acl-dev\\orm\\samples\\demo\\model.hpp");

    gen.print_entries();

    gen.parse_file("F:\\fork\\acl-dev\\dao_generator\\samples\\demo\\mapper.hpp");

    gen.gen_code("");
    
    return 0;
}