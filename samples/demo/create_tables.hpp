#pragma once
#include "acl_cpp/lib_acl.hpp"

inline bool create_db(const char* dbaddr,
                      const char* dbname,
                      const char* dbuser,
                      const char* dbpass)
{
    acl::db_mysql db(dbaddr, "mysql", dbuser, dbpass);
    if (db.open() == false)
    {
        printf("open %s@mysql error, dbuser: %s, dbpass: %s\r\n",
               dbaddr, dbuser, dbpass);
        return false;
    }

    acl::string sql;
    sql.format("use mysql");
    if (db.sql_update(sql.c_str()) == false)
    {
        printf("'%s' error: %s\r\n", sql.c_str(), db.get_error());
        return false;
    }
    db.free_result();

    sql.format("create database %s character set utf8;\r\n", dbname);
    if (db.sql_update(sql.c_str()) == false)
    {
        printf("'%s' error: %s\r\n", sql.c_str(), db.get_error());
        return false;
    }
    db.free_result();

    sql.format("grant CREATE,DROP,INSERT,DELETE,UPDATE,SELECT on %s.* to %s",
               dbname, dbuser);
    if (db.sql_update(sql.c_str()) == false)
    {
        printf("'%s' error: %s\r\n", sql.c_str(), db.get_error());
        return false;
    }
    db.free_result();

    sql = "flush privileges";
    if (db.sql_update(sql.c_str()) == false)
    {
        printf("'%s' error: %s\r\n", sql.c_str(), db.get_error());
        return false;
    }
    db.free_result();

    printf("create db(%s) ok\r\n", dbname);
    return true;
}

inline bool create_table(const char* dbaddr,
                       const char* dbname,
                       const char* dbuser,
                       const char* dbpass,
                       const char *sql)
{
    acl::db_mysql db(dbaddr, dbname, dbuser, dbpass);
    if (db.open() == false)
    {
        printf("open %s@mysql error, dbuser: %s, dbpass: %s\r\n",
               dbaddr, dbuser, dbpass);
        return false;
    }

    if (db.tbl_exists("group_tbl"))
    {
        printf("table exist\r\n");
        return (true);
    }
    else if (db.sql_update(sql) == false)
    {
        printf("sql error\r\n");
        return (false);
    }
    else
    {
        printf("create table ok\r\n");
        return (true);
    }
}
inline bool create_customer_table(const char* dbaddr,
                                  const char* dbname,
                                  const char* dbuser,
                                  const char* dbpass)
{
    const char *sql = 
        "CREATE TABLE customer("
        "id int(10) not null auto_increment,"
        "address varchar(120),"
        "postcode varchar(6),"
        "sex varchar(2),"
        "cname varchar(24),"
        "primary key(id))";

    return create_table(dbaddr, dbname, dbuser, dbpass, sql);
}

inline bool create_orders_table(const char* dbaddr,
                                  const char* dbname,
                                  const char* dbuser,
                                  const char* dbpass)
{
    const char *sql = "CREATE TABLE orders("
        "id int(10) not null auto_increment,"
        "code varchar(24),"
        "customer_id int(3) not null,"
        "primary key(id))";

    return create_table(dbaddr, dbname, dbuser, dbpass, sql);
}

