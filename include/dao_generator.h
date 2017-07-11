#pragma once
#include <map>
#include <set>
#include <vector>

namespace acl
{

    class dao_generator
    {
    private:
        friend struct update_token;
        friend struct syntax_error;

        struct token
        {
            token()
            {
                line_ = 0;
                type_ = e_null;
            }
            int line_;
            std::string str_;
            enum
            {
                e_null,             // null type
                e_less,             // <
                e_greate,           // >
                e_open_paren,       // (
                e_close_paren,      // )
                e_open_curly_brace, // {
                e_close_curly_brace,// }
                e_equal,            // =
                e_double_equal,     // ==
                e_colon,            // :
                e_double_colon,      // ::
                e_semicolon,        // ;
                e_quote,            // '
                e_double_quote,     // "
                e_asterisk,         // *
                e_pound_sign,       // #
                e_minus,            // -
                e_add,              // +
                e_and,              // &
                e_comma,            // ,
                e_dot,              // .
                e_struct,           // struct
                e_identifier,       // identifier
                e_using,            // using  
                e_namespace,        // namespace
                e_const,            // const
                e_long_long_int,    // long long int
                e_int,              // int
                e_bool,             // bool
                e_float,            // float
                e_double,           // double
                e_time,             // time_t
                e_void,             // void
                e_virtual,          // virtual
                e_std_string,       // std::string
                e_acl_string,       // acl::string
                e_std_list,         // std::list
                e_std_vector,       // std::vector
                e_comment,          // //
                e_comment_begin,    // /*
                e_comment_end,      // */
                e_div,              // /
                e_include,          // #include
                e_pragma_once,      // #pragma once
                e_eof,              // end of file 
                e_$,                // @
                e_$select,          // @Select
                e_$insert,          // @Insert
                e_$update,          // @Update
                e_$delete,          // @Delete
                e_$model,           // @Model
                e_$models,          // @Models
                e_$table,           // @Table
                e_$column,          // @Column
                e_$mapper,          // @mapper 
                e_$mappers,         // @mappers
                e_$result,          // @Result
                e_$include,         // @Include
            }type_;
        };
        struct entry;
        struct field
        {
            field():
                entry_(NULL)
            {
            }
            typedef enum
            {
                e_void,
                e_bool,
                e_long_long_int, // long long int
                e_int,           // int
                e_float,         // float
                e_double,        // double
                e_string,        // acl::string ,std::string
                e_entry,         // entry ,struct
                e_vector,        // std::list<object>, std::vector<object>
            } type_t;

            type_t type_;

            std::string type_str_;
            //
            std::string name_;

            //column name
            std::string column_;

            //to_one ,to_many 
            entry* entry_;
        };

        struct entry
        {
            bool model_;

            std::string filepath_;

            std::vector<std::string> namespaces_;

            //struct map to table. table_name_ is the table name
            std::string table_name_;

            // entry name. eq struct name
            std::string name_;

            std::vector<field> fields_;
        };

        
        struct mapper_function
        {
            typedef enum 
            {
                e_select,
                e_insert,
                e_update,
                e_delete,
            }type_t;
            
            struct result
            {
                std::string column_;
                std::string property_;
                std::string str_;
                int line_;
            };

            type_t type_;
            std::string sql_;
            bool sql_mutil_line_;
            std::string declare_;
            std::string name_;
            std::vector<result> columns_;
            std::vector<field> params_;
            std::vector<std::string> sql_params_;
            field return_;
            bool log_;
        };

        struct mapper
        {
            std::string name_;
            std::vector<mapper_function> mfuncs_;
            std::set<std::string> model_files_;
            std::string mapper_path_;
            std::vector<std::string> namespaces_;
        };

    public:
        dao_generator();
        void print_entries();
        void print_mappers();
        bool parse_path(const std::string &path);
        bool parse_file(const std::string &file_path);
        void gen_code(const std::string &path);
        void gen_code_multi_files(const std::string &path);
    private:
        //token lexer
        void reset_lexer();
        std::string get_string(char end );
        void store_file_point();
        void reload_file_point();
        token get_next_token();
        token current_token();
        void push_back_token(token t);

        std::string next_token(const std::string &delimiters);
        std::string look_ahead(const std::string &delimiters);
        void skip_space();
        void skip_space(std::string &line);
        
        int line_num();
        std::string next_line();
        std::string get_line();
    private:

        void reset_status();
        bool parse_model_file();

        void skip_comment();
        void parse_include();
        std::vector<std::string> get_namespace();
        void parse_model_struct();
        void parse_construct_func();

        bool parse_mapper_file();
        void parse_mapper_struct();

        field get_return_field();
        std::vector<field> get_mapper_func_params();
        std::vector<std::string> get_sql_param();

        void update_mapper_function_columns(mapper_function &func);
        std::vector<mapper_function::result> get_result_columns();

        std::string gen_class_implement(const mapper &m);
        std::string gen_query_set_parameters(const mapper_function &func);
        std::string get_assign_code(const field &f, const std::string &str);

        std::string get_define_columns(const std::vector<field> &f,
                                      const std::string &prefix = "",
                                      const std::string &tabs = "",
                                      bool br= true);

        std::string get_define_column(const field &f,
                                      const std::string &prefix = "",
                                      bool br= true);
        std::string gen_func_implement(const mapper_function &func);
        std::string get_class_name(const std::string &parent_name);
        std::string gen_class_declare(const mapper &m);
        std::string gen_annotation(const mapper_function &func,
                                   bool tab = true);
        std::string gen_func_impl_name(const std::string &class_name,
                                       const std::string &declare_);
        std::string gen_streq_code()const;
private:
        int to_field_type(int type);
        void print_entry(const entry &entry);
        void print_mapper(const mapper &mapper);
        void print_func(const mapper_function &func);
        std::string get_type(const mapper_function &func);

        void entry_reset();
        bool check_entry(std::string &name,
                         const std::vector<std::string> &namespaces);

        std::vector<field> get_fields(const std::string &name,
                                      const std::vector<std::string> &nss);

        entry get_entry(const std::string &name,
                        const std::vector<std::string> &nps);

        std::set<std::string> get_include_files(const std::string &file);
        std::vector<std::string> 
            parse_include(const std::vector<std::string> &files);
    private:
        long long int file_offset_;
        int line_num_;
        std::string token_buf_;
        std::list<token> tokens_;
        token current_token_;

        std::string line_buffer_;
        std::string current_line_;

        acl::ifstream file_;
        std::string file_path_;

        std::vector<std::string> namespaces_;

        entry entry_;
        std::vector<entry> entries_;

        mapper mapper_;
        std::vector<mapper> mappers_;

        std::set<std::string> model_files_;
        std::set<std::string> mapper_files_;
        bool use_streq_;
    };
}