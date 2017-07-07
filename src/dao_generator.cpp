#include <iostream>
#include "acl_cpp/lib_acl.hpp"
#include "dao_generator.h"

namespace acl
{
    struct syntax_error : std::runtime_error
    {
    public:
        syntax_error(const std::string &error = "syntax_error")
            :std::runtime_error(error)
        {

        }
    };

    dao_generator::dao_generator()
    {
        line_num_ = 0;
    }
    void dao_generator::push_back_token(dao_generator::token t)
    {
        tokens_.push_back(t);
    }
    void dao_generator::reset_lexer()
    {
        tokens_.clear();
        line_num_ = 0;
        line_buffer_.clear();
        file_.close();
        token_buf_.clear();
    }

    dao_generator::token dao_generator::get_next_token()
    {
        token t;

        if (!tokens_.empty())
        {
            t = tokens_.front();
            tokens_.pop_front();
            return t;
        }

        std::string str = next_token();

        t.str_ = str;
        t.line_ = line_num();


        if (str == ",")
        {
            t.type_ = token::e_comma;
            return t;
        }
        if (str == ".")
        {
            t.type_ = token::e_dot;
            return t;
        }
        else if (str == ";")
        {
            t.type_ = token::e_semicolon;
            return t;
        }
        else if (str == ":")
        {
            t.type_ = token::e_colon;
            return t;
        }
        else if (str == "(")
        {
            t.type_ = token::e_open_paren;
            return t;
        }
        else if (str == ")")
        {
            t.type_ = token::e_close_paren;
            return t;
        }
        else if (str == "{")
        {
            t.type_ = token::e_open_curly_brace;
            return t;
        }
        else if (str == "}")
        {
            t.type_ = token::e_close_curly_brace;
            return t;
        }
        else if (str == "<")
        {
            t.type_ = token::e_less;
            return t;
        }
        else if (str == ">")
        {
            t.type_ = token::e_greate;
            return t;
        }
        else if (str == "=")
        {
            t.type_ = token::e_equal;
            return t;
        }
        else if (str == "&")
        {
            t.type_ = token::e_and;
            return t;
        }
        else if (str == "/")
        {
            if (look_ahead() == "/")
            {
                str = next_token();
                t.type_ = token::e_comment;
                t.str_ = "//";
                t.line_ = line_num();
                return t;
            }
            else if (look_ahead() == "*")
            {
                str = next_token();
                t.type_ = token::e_comment_begin;
                t.str_ = "/*";
                t.line_ = line_num();
                return t;
            }
            t.type_ = token::e_div;
            return t;
        }
        else if (str == "*")
        {
            if (look_ahead() == "/")
            {
                str = next_token();
                t.type_ = token::e_comment_end;
                t.str_ = "*/";
                t.line_ = line_num();
                return t;
            }
            t.type_ = token::e_asterisk;
            return t;
        }
        if (str == "#include")
        {
            t.type_ = token::e_include;
            return t;
        }
        else if (str == "#pragma")
        {
            str = next_token();
            if (str == "once")
            {
                t.type_ = token::e_pragma_once;
                t.line_ = line_num();
                t.str_ = "#pragma once";
                return t;
            }
        }
        else if (str == "int")
        {
            t.type_ = token::e_int;
            return t;
        }
        else if (str == "float")
        {
            t.type_ = token::e_float;
            return t;
        }
        else if (str == "const")
        {
            t.type_ = token::e_const;
            return t;
        }
        else if (str == "double")
        {
            t.type_ = token::e_double;
            return t;
        }
        else if (str == "void")
        {
            t.type_ = token::e_void;
            return t;
        }
        else if (str == "bool")
        {
            t.type_ = token::e_bool;
            return t;
        }
        else if (str == "struct")
        {
            t.type_ = token::e_struct;
            return t;
        }
        else if (str == "virtual")
        {
            t.type_ = token::e_virtual;
            return t;
        }
        else if (str == "@")
        {
            str = look_ahead();

            if (str == "Table")
            {
                t.type_ = token::e_$table;
                t.str_ = "@Table";
                next_token();
                return t;
            }
            else if (str == "Model")
            {
                t.type_ = token::e_$model;
                t.str_ = "@Model";
                next_token();
                return t;
            }
            else if (str == "Models")
            {
                t.type_ = token::e_$models;
                t.str_ = "@Models";
                next_token();
                return t;
            }
            else if (str == "Column")
            {
                t.type_ = token::e_$column;
                t.str_ = "@Column";
                next_token();
                return t;
            }
            else if (str == "Delete")
            {
                t.type_ = token::e_$delete;
                t.str_ = "@Delete";
                next_token();
                return t;
            }
            else if (str == "Update")
            {
                t.type_ = token::e_$update;
                t.str_ = "@Update";
                next_token();
                return t;
            }
            else if (str == "Insert")
            {
                t.type_ = token::e_$insert;
                t.str_ = "@Insert";
                next_token();
                return t;
            }
            else if (str == "Select")
            {
                t.type_ = token::e_$select;
                t.str_ = "@Select";
                next_token();
                return t;
            }
            else if (str == "Mappers")
            {
                t.type_ = token::e_$mappers;
                t.str_ = "@Mappers";
                next_token();
                return t;
            }
            else if (str == "Mapper")
            {
                t.type_ = token::e_$mapper;
                t.str_ = "@Mapper";
                next_token();
                return t;
            }
            else if (str == "Result")
            {
                t.type_ = token::e_$result;
                t.str_ = "@Result";
                next_token();
                return t;
            }
            t.type_ = token::e_$;
            return t;
        }
        else if (str.empty())
        {
            t.type_ = token::e_eof;
            t.line_ = line_num();
            return t;
        }
        else if (str == "std")
        {
            if (next_token() == ":" && next_token() == ":")
            {
                str = next_token();
                if (str == "list")
                {
                    t.type_ = token::e_std_list;
                    t.str_ = "std::list";
                }
                else if (str == "vector")
                {
                    t.type_ = token::e_std_vector;
                    t.str_ = "std::vector";
                }
                else if (str == "string")
                {
                    t.type_ = token::e_std_string;
                    t.str_ = "std::string";
                }
                t.line_ = line_num();
                return t;
            }
        }

        t.type_ = token::e_identifier;
        t.line_ = line_num();
        t.str_ = str;
        return t;
    }



    /*" \/\r\t\n<>(){};,=-+.@?"*/
    std::string dao_generator::next_token(const std::string &delims)
    {
        if (line_buffer_.empty())
        {
            line_buffer_ = next_line();
            if (line_buffer_.empty())
                return std::string();
        }

        skip_space();
        /// read a line from file. and get token.
        if (line_buffer_.empty())
            return next_token(delims);


        token_buf_.clear();
        for (size_t i = 0; i < line_buffer_.size(); i++)
        {
            char ch = line_buffer_[i];
            if (delims.find(ch) == delims.npos)
            {
                token_buf_.push_back(ch);
            }
            else
            {
                if (token_buf_.empty())
                    token_buf_.push_back(ch);
                break;
            }
        }
        if (token_buf_.size())
        {
            line_buffer_ = line_buffer_.substr(token_buf_.size());
        }
        return token_buf_;
    }
    std::string dao_generator::next_line()
    {
        acl::string buffer;

        file_.gets(buffer, false);
        current_line_ = buffer;
        // read line ok
        if (buffer.size())
            line_num_++;

        return buffer.c_str();
    }
    std::string dao_generator::look_ahead(const std::string &delims)
    {
        std::string current_line = line_buffer_;

        long long offset = file_.ftell();

        while (current_line.empty())
        {

            acl::string line;

            if (file_.gets(line, true))
            {
                current_line = line.c_str();
            }
            skip(current_line, " \r\n\t");
        }

        file_.fseek(offset, SEEK_SET);

        std::string buf;
        for (size_t i = 0; i < current_line.size(); i++)
        {
            char ch = current_line[i];
            if (delims.find(ch) == delims.npos)
            {
                buf.push_back(ch);
            }
            else
            {
                if (buf.empty())
                {
                    buf.push_back(ch);
                }
                break;
            }
        }

        return buf;
    }

    void dao_generator::skip(std::string &line,
                             const std::string &delims)
    {
        int offset = 0;

        if (line.empty())
            return;

        for (size_t i = 0; i < line.size(); i++)
        {
            char ch = line[i];
            if (delims.find(ch) != delims.npos)
            {
                offset++;
                continue;
            }
            break;
        }

        if (offset == line.size())
        {
            line.clear();
        }
        else if (offset)
        {
            line = line.substr(offset);
        }
    }
    void dao_generator::skip_space(std::string &line)
    {
        skip(line, " \r\n\t");
    }
    void dao_generator::skip_space()
    {
        skip(line_buffer_, " \r\n\t");
    }
    int dao_generator::line_num()
    {
        return line_num_;
    }
    int dao_generator::to_field_type(int type)
    {
        if (type == token::e_int)
        {
            return field::e_int;
        }
        else if (type == token::e_float)
        {
            return field::e_float;
        }
        else if (type == token::e_double)
        {
            return field::e_double;
        }
        else if (type == token::e_std_string || type == token::e_acl_string)
        {
            return field::e_string;
        }
        else if (type == token::e_void)
        {
            return field::e_void;
        }
        else if (type == token::e_bool)
        {
            return field::e_bool;
        }
        throw syntax_error();
        return 0;
    }
    void dao_generator::entry_reset()
    {
        entry_.model_ = false;
        entry_.filepath_.clear();
        entry_.name_.clear();
        entry_.table_name_.clear();
        entry_.fields_.clear();
    }
    dao_generator::entry
        dao_generator::get_entry(const std::string &name)
    {
        std::list<entry>::iterator it = entries_.begin();
        for (; it != entries_.end(); ++it)
        {
            if (it->name_ == name)
            {
                return *it;
            }
        }
        throw syntax_error("");
        return dao_generator::entry();
    }
    std::vector<dao_generator::field>
        dao_generator::get_fields(const std::string &name)
    {
        std::list<entry>::iterator it = entries_.begin();
        for (; it != entries_.end(); ++it)
        {
            if (it->name_ == name)
                return it->fields_;
        }
        return std::vector<dao_generator::field>();
    }
    bool dao_generator::check_entry(std::string &name)
    {
        std::list<entry>::iterator it = entries_.begin();
        for (; it != entries_.end(); ++it)
        {
            if (it->name_ == name)
                return true;
        }
        return false;
    }

    bool dao_generator::parse_file(const std::string &file_path)
    {
        reset_lexer();
        if (!file_.open_read(file_path.c_str()))
        {
            logger_error("open file error %s", acl::last_serror());
            return false;
        }
        file_path_ = file_path;
        token _token = get_next_token();
        if (_token.type_ == token::e_comment)
        {
            _token = get_next_token();
            if (_token.type_ == token::e_$models)
            {
                return parse_model_file();
            }
            else
            {
                return parse_mapper_file();
            }
        }
        return false;
    }
    void dao_generator::parse_include()
    {
        token t = get_next_token();
        // <
        if (t.type_ == token::e_less)
        {
            t = get_next_token();
            if (t.type_ != token::e_identifier)
            {
                throw syntax_error();
            }
            t = get_next_token();
            // >
            if (t.type_ != token::e_greate)
                throw syntax_error();
            std::cout << current_line_.c_str();
        }
    }
    void dao_generator::parse_construct_func()
    {
        int count = 0;
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_open_curly_brace)
            {
                count++;
            }
            else if (t.type_ == token::e_close_curly_brace)
            {
                count--;
                if (count == 0)
                {
                    break;
                }
            }
            else if (t.type_ == token::e_comment)
            {
                line_buffer_.clear();
            }
            else if (t.type_ == token::e_comment_begin)
            {
                while (get_next_token().type_ != token::e_comment_end);
            }
        } while (true);
    }
    void dao_generator::parse_model_struct()
    {
        token t1 = get_next_token();
        token t2;
        token t3;

        if (t1.type_ != token::e_identifier)
            throw syntax_error();
        entry_.name_ = t1.str_;
        t2 = get_next_token();
        if (t2.type_ == token::e_colon)
        {
            t3 = get_next_token();
            if (t3.type_ != token::e_identifier)
                throw syntax_error();
            if (!check_entry(t3.str_))
            {
                throw syntax_error(std::string("not found ") + t1.str_);
            }
            entry_.fields_ = get_fields(t3.str_);
            //get { token
            t1 = get_next_token();
        }
        if (t1.type_ == token::e_open_curly_brace)
        {
            std::cout << "struct begin" << std::endl;
        }

        std::string column;
        do
        {
            t1 = get_next_token();
            field f;
            if (t1.type_ == token::e_comment)
            {
                t1 = get_next_token();
                if (t1.type_ == token::e_$column)
                {
                    t1 = get_next_token();
                    if (t1.type_ != token::e_open_curly_brace)
                        throw syntax_error();
                    t1 = get_next_token();
                    if (t1.type_ != token::e_identifier)
                        throw syntax_error();
                    column = t1.str_;
                    t1 = get_next_token();
                    if (t1.type_ != token::e_close_curly_brace)
                        throw syntax_error();
                    t1 = get_next_token();
                }
                else
                {
                    //normal comment
                    std::cout << current_line_.c_str() << std::endl;
                    line_buffer_.clear();
                    continue;
                }
            }

            if (t1.type_ == token::e_int ||
                t1.type_ == token::e_float ||
                t1.type_ == token::e_double ||
                t1.type_ == token::e_acl_string ||
                t1.type_ == token::e_std_string)
            {
                t2 = get_next_token();
                if (t2.type_ != token::e_identifier)
                    throw syntax_error();
                t3 = get_next_token();
                if (t3.type_ == token::e_semicolon)
                {
                    f.type_ = (field::type_t)to_field_type(t1.type_);
                    f.name_ = t2.str_;
                    f.type_str_ = t1.str_;

                    //set colum
                    f.column_ = t2.str_;
                    if (column.size())
                    {
                        f.column_ = column;
                        column.clear();
                    }
                    entry_.fields_.push_back(f);
                    continue;
                }
                throw syntax_error();
            }
            else if (t1.type_ == token::e_std_list || t1.type_ == token::e_std_vector)
            {
                // <
                t2 = get_next_token();
                if (t2.type_ != token::e_less)
                    throw syntax_error();
                // type
                t3 = get_next_token();
                if (t3.type_ != token::e_identifier)
                    throw syntax_error();

                if (!check_entry(t3.str_))
                    throw syntax_error(std::string("can't find ") + t3.str_);

                // > 
                token t4 = get_next_token();
                if (t4.type_ != token::e_greate)
                    throw syntax_error();

                token t5 = get_next_token();
                if (t5.type_ != token::e_identifier)
                    throw syntax_error();

                token t6 = get_next_token();

                if (t6.type_ == token::e_semicolon)
                {
                    f.type_str_ = t1.str_;
                    f.type_str_.append("<");
                    f.type_str_.append(t3.str_);
                    f.type_str_.append(">");
                    f.entry_ = new entry(get_entry(t3.str_));
                    f.type_ = field::e_vector;
                    f.name_ = t5.str_;

                    column.clear();
                    entry_.fields_.push_back(f);
                    continue;
                }
                throw syntax_error();
            }
            else if (t1.type_ == token::e_identifier)
            {
                if (t1.str_ == entry_.name_)
                {
                    t2 = get_next_token();
                    t3 = get_next_token();
                    if (t2.type_ == token::e_open_paren &&
                        t3.type_ == token::e_close_paren)
                    {
                        parse_construct_func();
                        continue;
                    }
                }
                if (!check_entry(t1.str_))
                    throw syntax_error(std::string("can't find ") + t1.str_);

                token t2 = get_next_token();
                if (t2.type_ != token::e_identifier)
                    throw syntax_error();
                f.type_ = field::e_entry;
                f.entry_ = new entry(get_entry(t1.str_));
                f.name_ = t2.str_;
                f.type_str_ = t1.str_;

                column.clear();
                entry_.fields_.push_back(f);
                continue;

            }
            else if (t1.type_ == token::e_close_curly_brace)
            {
                t1 = get_next_token();
                if (t1.type_ != token::e_semicolon)
                    throw syntax_error("not found ; ");
                entry_.filepath_ = file_path_;
                entries_.push_back(entry_);
                entry_reset();
                std::cout << "struct begin" << std::endl;
                break;
            }
        } while (true);
    }

    std::vector<dao_generator::field>
        dao_generator::get_mapper_func_params()
    {
        std::vector<field> params;

        token t = get_next_token();
        if (t.type_ != token::e_open_paren)
            throw syntax_error();
        do
        {
            field param;

            t = get_next_token();
            if (t.type_ == token::e_close_paren)
            {
                return params;
            }
            if (t.type_ == token::e_const)
            {
                t = get_next_token();
            }
            if (t.type_ == token::e_and)
            {
                t = get_next_token();
            }

            if (t.type_ == token::e_acl_string ||
                t.type_ == token::e_std_string ||
                t.type_ == token::e_int ||
                t.type_ == token::e_float ||
                t.type_ == token::e_double)
            {
                param.type_ = (field::type_t)to_field_type(t.type_);
                param.type_str_ = t.str_;
                token t2 = get_next_token();
                if (t2.type_ != token::e_identifier)
                    throw syntax_error();
                param.name_ = t2.str_;
            }

            else if (t.type_ == token::e_identifier)
            {
                if (!check_entry(t.str_))
                    throw syntax_error(std::string("not found ") + t.str_);
                param.type_ = field::e_entry;
                param.entry_ = new entry(get_entry(t.str_));
                param.type_str_ = t.str_;

                token t2 = get_next_token();
                if (t2.type_ == token::e_and)//&
                    t2 = get_next_token();

                if (t2.type_ != token::e_identifier)
                    throw syntax_error();

                param.name_ = t2.str_;
            }
            else if (t.type_ == token::e_std_list ||
                     t.type_ == token::e_std_vector)
            {
                if (get_next_token().type_ != token::e_less)
                    throw syntax_error();
                token t2 = get_next_token();
                if (t2.type_ != token::e_identifier)
                    throw syntax_error();
                if (get_next_token().type_ != token::e_greate)
                    throw syntax_error();
                token t3 = get_next_token();
                if (t3.type_ == token::e_and)
                    t3 = get_next_token();
                if (t3.type_ != token::e_identifier)
                    throw syntax_error();

                param.type_ = field::e_vector;
                param.entry_ = new entry(get_entry(t2.str_));
                param.type_str_ = t.str_;
                param.name_ = t3.str_;

            }
            params.push_back(param);

            t = get_next_token();
            if (t.type_ == token::e_close_paren)
            {
                return params;
            }
            else if (t.type_ != token::e_comma)
            {
                throw syntax_error();
            }

        } while (true);
    }

    dao_generator::field
        dao_generator::get_mappper_func_return()
    {
        token t = get_next_token();

        if (t.type_ == token::e_void ||
            t.type_ == token::e_int ||
            t.type_ == token::e_bool ||
            t.type_ == token::e_float ||
            t.type_ == token::e_double ||
            t.type_ == token::e_acl_string ||
            t.type_ == token::e_std_string)
        {
            field f;
            f.type_ = (field::type_t)to_field_type(t.type_);
            return f;
        }
        else if (t.type_ == token::e_std_list ||
                 t.type_ == token::e_std_vector)
        {
            if (get_next_token().type_ != token::e_less)
                throw syntax_error();
            token t2 = get_next_token();
            if (t2.type_ != token::e_identifier)
                throw syntax_error();
            if (get_next_token().type_ != token::e_greate)
                throw syntax_error();

            field f;
            f.type_ = field::e_vector;
            f.entry_ = new entry(get_entry(t2.str_));
            f.type_str_ = t.str_;
            return f;
        }
        else if (t.type_ == token::e_identifier)
        {
            field f;
            f.type_ = field::e_entry;
            f.entry_ = new entry(get_entry(t.str_));
            f.type_str_ = t.str_;
            return f;
        }
        throw syntax_error();
        return field();
    }
    std::vector<std::string> dao_generator::get_sql_param()
    {
        std::vector<std::string> params;
        token t = get_next_token();
        while (t.type_ != token::e_close_curly_brace)
        {
            if (t.type_ == token::e_colon)
            {
                t = get_next_token();
                params.push_back(t.str_);
            }
            t = get_next_token();
        }
        return params;
    }
    void dao_generator::update_mapper_function_columns(mapper_function &func)
    {
        if (func.type_ != mapper_function::e_select)
            return;

        //@Result column="cid" property="id"
        //@Result column="oid" property="order.id"

        for (size_t i = 0; i < func.columns_.size(); i++)
        {
            std::string property = func.columns_[i].property_;
            std::string column = func.columns_[i].column_;

            std::vector<field> &fields = func.params_[0].entry_->fields_;

            size_t npos = property.find_first_of('.');
            if (npos == property.npos)
            {
                bool found = false;
                for (size_t j = 0; j < fields.size(); j++)
                {
                    field &f = fields[j];
                    if (f.name_ == property)
                    {
                        f.column_ = column;
                        found = true;
                        break;
                    }
                }
                if (!found)
                {
                    acl::string error;
                    error.format("%s line:%d error.not match",
                                 func.columns_[i].str_.c_str(),
                                 func.columns_[i].line_);
                    throw syntax_error(error.c_str());
                }

            }
            else
            {
                std::string first = property.substr(0, property.find_first_of('.'));
                std::string second = property.substr(property.find_first_of('.') + 1);
                bool found = false;

                for (size_t j = 0; j < fields.size(); j++)
                {
                    field &f = fields[j];
                    if (f.name_ == first)
                    {
                        if (f.type_ != field::e_vector && f.type_ != field::e_entry)
                            throw syntax_error();

                        for (size_t k = 0; k < f.entry_->fields_.size(); k++)
                        {
                            field &f2 = f.entry_->fields_[k];
                            if (f2.name_ == second)
                            {
                                f2.column_ = column;
                                found = true;
                            }
                        }
                    }
                }
                if (!found)
                {
                    acl::string error;
                    error.format("%s line:%d error.not match",
                                 func.columns_[i].line_,
                                 func.columns_[i].str_.c_str());
                    throw syntax_error(error.c_str());
                }
            }
        }
    }


    std::vector<dao_generator::mapper_function::result>
        dao_generator::get_result_columns()
    {
        //@Result{column=cid, property=customer_order.id}

        std::vector<mapper_function::result> results;
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_comment)
            {
                t = get_next_token();
                if (t.type_ == t.e_$result)
                {
                    if (get_next_token().type_ != token::e_open_curly_brace)
                        throw syntax_error();
                    if (get_next_token().str_ != "column")
                        throw syntax_error();
                    if (get_next_token().type_ != token::e_equal)
                        throw syntax_error();
                    token column = get_next_token();

                    if (get_next_token().type_ != token::e_comma)
                        throw syntax_error();
                    if (get_next_token().str_ != "property")
                        throw syntax_error();
                    if (get_next_token().type_ != token::e_equal)
                        throw syntax_error();
                    std::string property = line_buffer_;
                    skip_space(property);
                    property = property.substr(0, property.find_last_of('}'));

                    mapper_function::result result;
                    result.column_ = column.str_;
                    result.property_ = property;
                    result.str_ = current_line_;
                    result.line_ = line_num_;
                    results.push_back(result);

                    line_buffer_.clear();
                }
            }
            else
            {
                push_back_token(t);
                break;
            }
        } while (true);

        return results;
    }
    void dao_generator::parse_mapper_struct()
    {
        token t = get_next_token();
        if (t.type_ != token::e_identifier)
            throw syntax_error();
        if (get_next_token().type_ != token::e_open_curly_brace)
            throw syntax_error();

        mapper_.name_ = t.str_;
        do
        {
            t = get_next_token();
            if (t.type_ == token::e_comment)
            {
                t = get_next_token();
            }
            if (t.type_ == token::e_$insert ||
                t.type_ == token::e_$update ||
                t.type_ == token::e_$delete ||
                t.type_ == token::e_$select)
            {
                mapper_function mfunc;

                mfunc.type_ = mapper_function::e_insert;
                if (t.type_ == token::e_$update)
                    mfunc.type_ = mapper_function::e_update;
                else if (t.type_ == token::e_$delete)
                    mfunc.type_ = mapper_function::e_delete;
                else if (t.type_ == token::e_$select)
                    mfunc.type_ = mapper_function::e_select;

                if (get_next_token().type_ != token::e_open_curly_brace)
                    throw syntax_error();
                mfunc.sql_ = line_buffer_.substr(0, line_buffer_.find_last_of('}'));
                mfunc.sql_params_ = get_sql_param();

                token t2 = get_next_token();
                token t3 = get_next_token();

                push_back_token(t2);
                push_back_token(t3);

                if (t2.type_ == token::e_comment &&
                    t3.type_ == token::e_$result)
                {
                    mfunc.columns_ = get_result_columns();
                }

                //skip to virtual;
                while (get_next_token().type_ != token::e_virtual);

                mfunc.return_ = get_mappper_func_return();
                if (mfunc.return_.type_ != field::e_bool)
                    throw syntax_error("function return type must be bool ");

                mfunc.declare_ = current_line_.
                    substr(0, current_line_.find_last_of('='));
                skip_space(mfunc.declare_);
                //get function name;
                t = get_next_token();
                mfunc.name_ = t.str_;

                mfunc.params_ = get_mapper_func_params();
                if (mfunc.type_ == mapper_function::e_select)
                {
                    if (mfunc.params_.empty())
                        throw syntax_error("{select} type function params must not empty");
                }
                update_mapper_function_columns(mfunc);
                t = get_next_token();
                if (t.type_ != t.e_equal)
                    throw syntax_error();
                if (get_next_token().type_ != token::e_identifier)
                    throw syntax_error();
                if (get_next_token().type_ != token::e_semicolon)
                    throw syntax_error();

                mapper_.mfuncs_.push_back(mfunc);
            }
            else if (t.type_ == token::e_close_curly_brace)
            {
                t = get_next_token();
                if (t.type_ != token::e_semicolon)
                    throw syntax_error();
                mappers_.push_back(mapper_);
                mapper_.mfuncs_.clear();
                mapper_.name_.clear();
                break;
            }
        } while (true);
    }
    bool dao_generator::parse_mapper_file()
    {
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_include)
            {
                parse_include();
            }
            else if (t.type_ == token::e_pragma_once)
            {
                std::cout << current_line_.c_str();
                continue;
            }
            else if (t.type_ == token::e_comment)
            {
                t = get_next_token();
                if (t.type_ == token::e_$mapper)
                {
                    t = get_next_token();
                    if (t.type_ == token::e_struct)
                    {
                        parse_mapper_struct();
                        continue;
                    }
                }

            }
            else if (t.type_ == token::e_eof)
            {
                return true;
            }
        } while (true);
    }
    bool dao_generator::parse_model_file()
    {
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_include)
            {
                parse_include();
            }
            else if (t.type_ == token::e_pragma_once)
            {
                std::cout << current_line_.c_str();
                continue;
            }
            else if (t.type_ == token::e_comment)
            {
                t = get_next_token();

                if (t.type_ == token::e_$model)
                {
                    entry_.model_ = true;
                }
                else if (t.type_ == token::e_$table)
                {
                    //{
                    t = get_next_token();
                    if (t.type_ != token::e_open_curly_brace)
                        throw syntax_error();
                    // table name
                    t = get_next_token();
                    if (t.type_ != token::e_identifier)
                        throw syntax_error();
                    //}
                    entry_.table_name_ = t.str_;
                    t = get_next_token();
                    if (t.type_ != token::e_close_curly_brace)
                        throw syntax_error();
                }
            }
            else if (t.type_ == token::e_struct)
            {
                parse_model_struct();
            }
            else if (t.type_ == token::e_eof)
            {
                return true;
            }

        } while (true);

        return false;
    }
    #define br  std::string("\r\n")
    #define tab std::string("\t")
    #define tab2 std::string("\t\t")
    #define tab3  std::string("\t\t\t")
    #define tab4  std::string("\t\t\t\t")

    std::string dao_generator::get_class_name(const std::string &parent_name)
    {
        acl::string class_name = parent_name.c_str();
        //class customer_dao :customer_mapper
        if (class_name.rncompare("_mapper", strlen("_mapper")) == 0)
        {
            std::string buffer(parent_name);
            buffer = buffer.substr(0, buffer.size() - strlen("_mapper"));
            buffer += "_dao";
            return buffer;
        }
        class_name += "_dao";

        return class_name.c_str();
    }
    std::string dao_generator::gen_func_impl_name(const std::string &class_name,
                                                  const std::string &declare_)
    {
        std::string code = declare_;
        skip_space(code);

        std::string token;
        for (size_t i = 0; i < code.size(); i++)
        {
            char ch = code[i];
            if (ch == ' ')
            {
                if (token == "bool")
                {
                    return "bool " + class_name + "::" + code.substr(i + 1);
                }
                else if (token == "virtual")
                {
                    token.clear();
                }
            }
            else
            {
                token.push_back(ch);
            }
        }
        throw syntax_error("error");
        return std::string();
    }
    std::string dao_generator::gen_annotation(const mapper_function &func, bool tab_)
    {
        std::string code;
        if (tab_)
            code += tab;
        if (func.type_ == mapper_function::e_insert)
            code += "//@Insert{";
        else if (func.type_ == mapper_function::e_update)
            code += "//@Update{";
        else if (func.type_ == mapper_function::e_delete)
            code += "//@Delete{";
        else if (func.type_ == mapper_function::e_select)
            code += "//@Select{";
        code += func.sql_ + "}";
        code += br;



        for (size_t i = 0; i < func.columns_.size(); i++)
        {
            if (tab_)
                code += tab;
            code += "//@Result{column=" + func.columns_[i].column_;
            code += ", property=" + func.columns_[i].property_ + "}" + br;
        }

        return code;
    }

    std::string dao_generator::gen_class_declare(const mapper &m)
    {
        std::string class_name = get_class_name(m.name_);
        std::string code;

        code += "class " + class_name + ": public " + m.name_ + br;
        code += "{" + br;
        code += "public:" + br;
        code += tab + class_name + "(acl::db_handle& handle);";
        code += br;
        code += br;

        std::list<mapper_function>::const_iterator it = m.mfuncs_.begin();
        for (; it != m.mfuncs_.end(); ++it)
        {
            code += gen_annotation(*it);
            code += tab;
            code += it->declare_;
            code += br;
            code += br;
        }

        code += "private:" + br;
        code += tab + "acl::db_handle& db_handle_;";
        code += br;
        code += "};";
        code += br;
        code += br;

        return code;
    }
    std::string dao_generator::gen_query_set_parameters(const mapper_function &func)
    {
        std::string code;
        if (func.sql_params_.empty())
            return code;

        code += br;
        for (size_t i = 0; i < func.sql_params_.size(); ++i)
        {
            std::string param = func.sql_params_[i];

            code += tab + "query.set_parameter(\"" + param + "\", ";
            bool found = false;

            size_t j = 0;
            if (func.type_ == mapper_function::e_select)
                j = 1;
            for (; j < func.params_.size(); ++j)
            {
                field f = func.params_[j];

                if (f.type_ == field::e_entry)
                {
                    std::vector<field> fileds = f.entry_->fields_;

                    for (size_t i = 0; i < fileds.size(); i++)
                    {
                        field &f2 = fileds[i];
                        if (f2.column_ == param)
                        {
                            code += f.name_ + "." + f2.column_;
                            if (f2.type_ == field::e_string)
                            {
                                code += ".c_str()";
                            }
                            code += ");" + br;
                            found = true;
                            break;
                        }
                    }
                }
                else if (f.name_ == param)
                {
                    code += f.name_;
                    if (f.type_ == field::e_string)
                    {
                        code += ".c_str()";
                    }
                    code += ");" + br;
                    found = true;
                    break;
                }
            }
            if (!found)
            {
                //todo
                throw syntax_error("not found sql param " + param);
            }
        }
        return code;
    }
    std::string dao_generator::get_assign_code(const field &f, const std::string &str)
    {
        if (f.type_ == field::e_int)
        {
            return std::string("atoi(") + str + ");";
        }
        return str;
    }

    std::string dao_generator::gen_func_implememt(const mapper_function &func)
    {
        std::string code;

        code += tab + "acl::query query;" + br;
        code += tab + "query.create_sql(\"" + func.sql_ + "\");" + br;
        code += gen_query_set_parameters(func);
        code += br;
        code += br;

        if (func.log_)
        {
            code += tab + "logger(\"sql: %s\", query.to_string().c_str());" + br;
            code += br;
        }

        if (func.type_ == mapper_function::e_delete ||
            func.type_ == mapper_function::e_insert ||
            func.type_ == mapper_function::e_update)
        {
            code += tab + "if (db_handle_.exec_update(query) == false)" + br;
            code += tab + "{" + br;
            code += tab + tab + "logger_error(\"db_.exec_update failed :%s\",db_handle_.get_error());" + br;
            code += tab + tab + "return false;" + br;
            code += tab + "}" + br;

            code += tab + "if (db_handle_.commit() == false)" + br;
            code += tab + "{" + br;
            code += tab + tab + "logger_error(\"db_.commit failed :%s\",db_handle_.get_error());" + br;
            code += tab + tab + "return false;" + br;
            code += tab + "}" + br;

            code += tab + "return true;" + br;

        }
        else if (func.type_ == mapper_function::e_select)
        {
            code += tab + "if (db_handle_.exec_select(query) == false)" + br;
            code += tab + "{" + br;
            code += tab + tab + "logger_error(\"db_.exec_update failed :%s\",db_handle_.get_error());" + br;
            code += tab + tab + "return false;" + br;
            code += tab + "}" + br;
            code += br;

            if (func.params_.front().type_ == field::e_entry)
            {
                code += tab + "for (size_t i = 0; i < db_handle_.length(); ++i)" + br;
                code += tab + "{" + br;

                code += tab + tab + "const acl::db_row* row = db_handle_[i];" + br + br;

                std::vector<field> &fields = func.params_[0].entry_->fields_;

                std::string arg = func.params_[0].name_;

                //const char* cid      = (*row)["cid"];
                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ == field::e_entry || f.type_ == field::e_vector)
                    {
                        std::vector<field> &fields2 = f.entry_->fields_;
                        code += br;
                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &f2 = fields2[i];
                            code += tab + tab + "const char* " + "$$" + f2.column_ + " = (*row)[\"" + f2.column_ + "\"];" + br;
                        }
                    }
                    else
                    {
                        code += tab + tab + "const char* " + "$" + f.column_ + " = (*row)[\"" + f.column_ + "\"];" + br;
                    }
                }

                code += br;

                //if (address)
                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ != field::e_entry && f.type_ != field::e_vector)
                    {
                        code += tab + tab + "if(" + "$" + f.column_ + ")" + br;
                        code += tab + tab + tab + arg + "." + f.name_ + " = ";
                        code += get_assign_code(f, "$" + f.column_) + br;
                    }
                }

                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ == field::e_entry)
                    {
                        std::vector<field> &fields2 = f.entry_->fields_;
                        code += br;
                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &child = fields2[i];
                            code += tab + tab + "if($$" + child.column_ + ")" + br;
                            code += tab + tab + tab + arg + "." + f.name_ + "." + child.name_ + " = ";
                            code += get_assign_code(child, "$$" + child.column_) + br;
                        }
                    }
                }

                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ == field::e_vector)
                    {
                        code += br;
                        code += tab + tab + f.entry_->name_ + " $_obj;" + br;

                        std::vector<field> &fields2 = f.entry_->fields_;

                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &child = fields2[i];
                            code += tab + tab + "if($$" + child.column_ + ")" + br;
                            code += tab + tab + tab + "$_obj." + child.name_ + " = ";
                            code += get_assign_code(child, "$$" + child.column_) + br;
                        }

                        code += tab + tab + arg + "." + f.name_ + ".push_back($_obj);" + br;
                    }
                }

                code += tab + "}" + br;
                code += tab + "return !!db_handle_.length();" + br;
            }
            else if (func.params_.front().type_ == field::e_vector)
            {
                std::string obj = "$item";

                code += tab + "for (size_t i = 0; i < db_handle_.length(); ++i)" + br;
                code += tab + "{" + br;
                code += tab + tab + func.params_.front().entry_->name_ + " " + obj + br;
                code += tab + tab + "const acl::db_row* row = db_handle_[i];" + br + br;

                std::vector<field> &fields = func.params_[0].entry_->fields_;

                //const char* cid      = (*row)["cid"];
                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (!(f.type_ == field::e_entry || f.type_ == field::e_vector))
                    {
                        code += tab + tab + "const char* " + "$" + f.column_ + " = (*row)[\"" + f.column_ + "\"];" + br;
                    }
                }

                code += br;

                //const char* cid      = (*row)["cid"];
                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ == field::e_entry)
                    {
                        std::vector<field> &fields2 = f.entry_->fields_;
                        code += br;
                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &f2 = fields2[i];
                            code += tab + tab + "const char* " + "$$" + f2.column_ + " = (*row)[\"" + f2.column_ + "\"];" + br;
                        }
                    }
                }

                code += br;

                //if (address)
                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ != field::e_entry && f.type_ != field::e_vector)
                    {
                        code += tab + tab + "if(" + "$" + f.column_ + ")" + br;
                        code += tab + tab + tab + obj + "." + f.name_ + " = ";
                        code += get_assign_code(f, "$" + f.column_) + br;
                    }
                }

                for (size_t i = 0; i < fields.size(); i++)
                {
                    field &f = fields[i];
                    if (f.type_ == field::e_entry)
                    {
                        std::vector<field> &fields2 = f.entry_->fields_;
                        code += br;
                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &child = fields2[i];
                            code += tab + tab + "if($$" + child.column_ + ")" + br;
                            code += tab + tab + tab + obj + "." + f.name_ + "." + child.name_ + " = ";
                            code += get_assign_code(child, "$$" + child.column_) + br;
                        }
                    }
                }

                for (size_t i = 0; i < fields.size(); i++)
                {

                    field &f = fields[i];
                    if (f.type_ == field::e_vector)
                    {
                        code += br;
                        code += tab + tab + "for (; i< db_.length(); i++)" + br;
                        code += tab + tab + "{" + br;
                        std::string item = f.entry_->name_ + "_item";
                        code += tab + tab + tab + f.entry_->name_ + " " + item;
                        std::vector<field> &fields2 = f.entry_->fields_;
                        code += br;

                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &f2 = fields2[i];
                            code += tab + tab + tab + "const char* " + "$$" + f2.column_ + " = (*row)[\"" + f2.column_ + "\"];" + br;
                        }
                        code += br;

                        for (size_t i = 0; i < fields.size(); i++)
                        {
                            field &f = fields[i];
                            if (!(f.type_ == field::e_entry || f.type_ == field::e_vector))
                            {
                                code += tab + tab + tab + "const char* " + "$$$" + f.column_ + " = (*row)[\"" + f.column_ + "\"];" + br;
                            }
                        }
                        code += br;
                        /*
                        if (!streq(cid, cid) ||
                            !streq($address1, address) ||
                            !streq(postcode1, postcode) ||
                            !streq(sex1, sex) ||
                            !streq(cname1, cname))
                        {
                            i--;
                            break;
                        }
                        */
                        code += tab3 + "if (";

                        for (size_t i = 0; i < fields.size(); i++)
                        {
                            field &f = fields[i];
                            if (!(f.type_ == field::e_entry || f.type_ == field::e_vector))
                            {
                                if (i != 0)
                                    code += tab4;
                                code += "!streq($$$" + f.column_ + ", $" + f.column_ + ")";
                                if (i == fields.size() - 2)
                                    code += ")" + br;
                                else
                                    code += " ||" + br;
                            }
                        }
                        use_strreq_ = true;
                        code += tab3 + "{" + br;
                        code += tab4 + "--i;" + br;
                        code += tab4 + "break;" + br;
                        code += tab3 + "}" + br;
                        /*
                         if (code)
                            orders_obj.code = code;
                         if (oid)
                            orders_obj.id = atoi(oid);
                         if (customer_id)
                            orders_obj.customer_id = atoi(customer_id);

                            customer_order_obj.orders.push_back(orders_obj);
                        */

                        for (size_t i = 0; i < fields2.size(); i++)
                        {
                            field &f2 = fields2[i];
                            code += tab3 + "if($$" + f2.column_ + ")" + br;
                            code += tab4 + item + "." + f2.name_ + " = ";
                            code += get_assign_code(f2, "$$" + f2.column_) + br;
                        }
                        code += br;
                        code += tab3 + func.params_.front().name_ + "." + f.name_ + ".push_back(" + item + ");" + br;

                        code += br;
                        code += tab + tab + "}" + br;
                    }
                }


                code += br;

                code += tab + tab + func.params_.front().name_ + ".push_back(" + obj + ");" + br;

                code += tab + "}" + br;
                code += tab + "return !!db_handle_.length();" + br;
            }
        }

        return code;
    }
    std::string dao_generator::gen_class_implememt(const mapper &m)
    {
        std::string code;
        std::string class_name = get_class_name(m.name_);

        code += class_name + "::" + class_name + "(acl::db_handle& handle)" + br;
        code += tab + ":db_handle_(handle)" + br;
        code += "{" + br;
        code += "}";

        code += br;
        code += br;

        std::list<mapper_function>::const_iterator it = m.mfuncs_.begin();
        for (; it != m.mfuncs_.end(); ++it)
        {
            code += gen_annotation(*it, false);
            code += gen_func_impl_name(class_name, it->declare_);
            code += br;
            code += "{";
            code += br;
            code += gen_func_implememt(*it);
            code += "}";
            code += br;
            code += br;

        }
        return code;
    }
    std::string dao_generator::gen_streq_code()const
    {
            return "static inline bool streq(const char *left, const char *right)" + br +
            "{" + br +
            tab + "if (!left && !right)" + br +
            tab + tab + "return true;" + br +
            tab + "if (left && right)" + br +
            tab + tab + "return strcmp(left, right) == 0;" + br +
            tab + "return false;" + br +
            "}" + br + br;

    }
    void dao_generator::gen_code(const std::string &path)
    {
        std::string declare_code;
        std::string source_code;
        std::string header_file_path;
        std::string source_file_path;
        acl::ofstream declare_file;
        acl::ofstream source_file;

        header_file_path = path;
        source_file_path = path;

        if (header_file_path.size())
        {
            char ch = header_file_path[header_file_path.size() - 1];
            if (ch != '/' && ch != '\\')
            {
                header_file_path.push_back('/');
                source_file_path.push_back('/');
            }
        }

        header_file_path.append("dao.h");
        source_file_path.append("dao.cpp");

        declare_code += "#pragma once" + br + br;

        use_strreq_ = false;

        std::list<mapper>::iterator it = mappers_.begin();
        for (; it != mappers_.end(); it++)
        {
            declare_code += gen_class_declare(*it);
            source_code += gen_class_implememt(*it);
        }


        if (!declare_file.open_trunc(header_file_path.c_str()))
            printf("open file(%s) error,%s",
                   header_file_path.c_str(),
                   acl::last_serror());

        if (!source_file.open_trunc(source_file_path.c_str()))
            printf("open file(%s) error,%s",
                   source_file_path.c_str(),
                   acl::last_serror());


        if (declare_file.write(declare_code.c_str(), declare_code.size()) == -1)
            printf("write file error,%s", acl::last_serror());


        std::string buffer;
        if (use_strreq_)
        {
            buffer += "#include \"acl_cpp/lib_acl.hpp\"" + br;
            buffer += "#include \"dao.hpp\"" + br;
            buffer += br;
            buffer += gen_streq_code();
        }
        buffer += source_code;

        if (source_file.write(buffer.c_str(), buffer.size()) == -1)
            printf("write file error,%s", acl::last_serror());

    }
    void dao_generator::print_entry(const entry &_entry)
    {
        for (size_t i = 0; i < 72; i++)
        {
            std::cout << "*";
        }
        std::cout << std::endl;
        std::cout << "name        :" << _entry.name_ << std::endl;
        std::cout << "table_name_ :" << _entry.table_name_ << std::endl;
        std::cout << "file_path   :" << _entry.filepath_ << std::endl;
        std::cout << "is_model    :" << (_entry.model_ ? "true" : "false") << std::endl;
        std::cout << "fields_     :" << std::endl;


        for (size_t i = 0; i < _entry.fields_.size(); i++)
        {
            field e = _entry.fields_[i];
            std::cout << "-----------------------------" << std::endl;
            std::cout << "name    :" << e.name_ << std::endl;
            std::cout << "column_ :" << e.column_ << std::endl;
            if (e.entry_)
                std::cout << "entry_name_:" << e.entry_->name_ << std::endl;
            std::cout << "type    :" << e.type_str_ << std::endl;
        }

    }
    void dao_generator::print_entries()
    {
        std::list<entry>::iterator it = entries_.begin();
        for (; it != entries_.end(); ++it)
        {
            print_entry(*it);
        }
    }
}