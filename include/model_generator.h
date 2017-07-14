#pragma once
#include <vector>
#include <string>
namespace acl
{
    class model_generator
    {
    private:
        struct token
        {
            typedef enum type
            {
                e_eof,
                e_create,           // CREATE
                e_table,            // TABLE
                e_unique,           // UNIQUE
                e_primary,          // PRIMARY
                e_auto_increment,   // AUTO_INCREASE
                e_if,               // IF
                e_not,              // NOT
                e_exist,            // EXIST
                e_engine,           // ENGINE
                e_default,          // default
                e_charset,          // charset
                e_temporary,        // temporary
                e_as,               // AS
                e_null,             // null
                e_constraint,       // CONSTRAINT
                e_foreign,          // FOREIGN KEY
                e_reference,        // reference
                e_sql_comment,      // comment ''
                e_comment,          // //
                e_comment_begin,    // /*
                e_comment_end,      // */
                e_key,              // key
                e_index,            //
                e_pack_keys,        // PACK_KEYS
                e_drop,
                e_now,              // NOW()

                e_bit,
                e_bool,
                e_unsigned,         // UNSIGNED
                e_tinyint,          // TINYINT
                e_smallint,         // SMALLINT
                e_mediumint,        // MEDIUMIN
                e_integer,          // INTEGER
                e_int,              // INT
                e_bigint,           // BIGINT
                e_float,            // FLOAT
                e_double,           // DOUBLE
                e_decimal,          // DECIMAL

                e_char,
                e_varchar,
                e_tinytext,
                e_text,
                e_blob,
                e_mediumtext,
                e_mediumblob,
                e_longtext,
                e_longblob,

                e_enum,
                e_set,

                e_date,
                e_datetime,
                e_timestamp,
                e_time,
                e_year,

                e_comma,            // ,
                e_semicolon,        // ;
                e_backtick,         // `
                e_double_quote,     // "
                e_quote,            // '
                e_open_paren,       // (
                e_close_paren,      // )
                e_eq,               // =
                e_sub,              // -
                e_double_sub,       // --
                e_asterisk,         // *
                e_identifier,
            }type;
            type type_;
            std::string str_;
            int line_;
        };
        struct field
        {
            field()
            {
                type_ = e_void;
                max_len_ = 0;
                auto_increase_ = false;
                unsigned_ = false;
                not_null_ = false;
                unique_ = false;
                primary_key_ = false;
            }
            typedef enum type
            {
                e_void,
                e_bool,
                e_char,
                e_short,
                e_int,
                e_long_long_int,
                e_float,
                e_double,
                e_std_string
            }type;

            int max_len_;
            std::string type_str_;
            std::string type_len_;
            type type_;
            std::string name_;
            std::string comment_;
            bool unsigned_;
            bool auto_increase_;
            bool primary_key_;
            bool not_null_;
            bool unique_;
            std::string default_;
        };
        struct table
        {
            std::string name_;
            std::string primary_key_;
            std::string engine_;
            std::string charset_;
            std::string auto_increment_;
            std::vector<field> fields_;
            std::string sql_;
            std::string file_path_;
        };
    public:
        model_generator();
        void set_namespace(const std::vector<std::string> &namespaces);
        bool parse(const std::string &path);
        void gen_model(const std::string &path);
        void gen_models(const std::string &path);
        void gen_create_tables(const std::string &path);
        void gen_mapper(const std::string &path);
        void gen_mappers(const std::string &path);
    private:
        std::string next_token(const std::string &delimiters);
        std::string get_string(char end);
        std::string next_line();
        void push_back_token(const token&t);
        token get_next_token();
        token current_token();

        char look_ahead();
    private:
        field::type to_field_type(const token &t);
        std::string get_name();
        std::string get_string();
        field parse_field();
        void parse_table();
        void parse();
    private:
        std::string get_type(field &f);
        std::string gen_field(field &f);
        std::string gen_construct(table &t);
        std::string gen_model(table &t);
        
        field get_primary_key(const table &t);
        std::string gen_mapper(const table &t);
    private:
        int lines_;
        std::vector<std::string> namespaces_;
        std::string line_buffer_;
        std::string field_suffix_;
        std::string table_suffix_;
        std::string current_line_;
        token current_token_;
        std::vector <token> tokens_;
        std::string namespace_;
        acl::ifstream file_;
        std::string file_path_;
        table table_;
        std::vector<table> tables_;
    };
}