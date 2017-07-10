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
                e_create,           // CREATE
                e_table,            // TABLE
                e_unique,           // UNIQUE
                e_primary,          // PRIMARY
                e_auto_increase,    // AUTO_INCREASE
                e_if,               // IF
                e_not,              // NOT
                e_exist,            // EXIST
                e_engine,           // ENGINE
                e_default,          // default
                e_charset,          // charset
                e_temporary,        // temporary
                e_as,               // AS
                e_not,              // not
                e_null,             // null
                e_foreign,          // FOREIGN KEY
                e_reference,        // reference
                e_sql_comment,      // comment ''
                e_comment,          // //
                e_comment_begin,    // /*
                e_comment_end,      // */
                e_key,              // key
                e_pack_keys,        // PACK_KEYS
                e_drop,

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
            }
            typedef enum type
            {
                e_void,
                e_char,
                e_short,
                e_int,
                e_long_long_int,
                e_float,
                e_double,
                e_std_string
            }type;

            int max_len_;
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
            std::vector<field> fields_;
        };
    public:
        model_generator();
        bool parse_sql(const std::string &file_path);
        void gen_model(const std::string &path);
    private:
        std::string next_token(const std::string &delimiters);

        std::string next_line();
        token get_next_token();
        token current_token();

        char look_ahead();
        void skip(std::string &str, const std::string &chars, bool skip_all);
    private:
        field::type to_field_type(const token &t);
        std::string get_string();
        std::string get_default();
        field parse_field();
        void parse_table();
        void parse();
    private:
        int lines_;
        std::string line_buffer_;
        std::string current_line_;
        token current_token_;
        std::vector <token> tokens_;
        std::string namespace_;
        acl::ifstream file_;
        std::string file_path_;
        table table1_;
        std::vector<table> tables_;
    };
}