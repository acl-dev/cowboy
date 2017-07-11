#include <iostream>
#include <stdexcept>
#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "model_generator.h"
namespace acl
{
    struct syntax_error :std::logic_error
    {
        syntax_error(const std::string &error = "syntax_error")
            :std::logic_error(error) 
        {

        }
    };

    model_generator::model_generator()
    {
        lines_ = 0;
    }
    std::string model_generator::next_line()
    {
        acl::string buffer;
        file_.gets(buffer, false);
        if (buffer.empty())
            return std::string();
        lines_++;
        current_line_ = buffer.c_str();
        return current_line_;
    }
    std::string model_generator::get_string(char end)
    {
        std::string buffer;
        std::string data = current_line_;
        std::string current_lien = current_line_;
        long long offset = file_.ftell();

        do
        {
            for (size_t i = 0; i < data; ++i)
            {
                char ch = data[i];
                if(ch != end)
                {
                    buffer.push_back(ch);
                }
                else
                {
                    current_line_ = current_lien;
                    file_.fseek(offset, SEEK_SET);
                    return buffer;
                }
            }
            buffer = next_line();
        }while(true);
    }
    void skip(std::string &str, 
              const std::string &skip_chars, 
              bool skip_all)
    {
        if (str.empty())
            return;
        std::string buffer;
        for (size_t i = 0; i < str.size(); i++)
        {
            char ch = str[i];
            if (skip_chars.find(ch) == skip_chars.npos)
            {
                if (!skip_all && i > 0)
                {
                    str = str.substr(i);
                    return;
                }
                else
                {
                    buffer.push_back(ch);
                }
            }                
        }
        if (buffer.size() != str.size())
            str = buffer;
    }
    std::string model_generator::next_token(const std::string &delimiters)
    {
        std::string buffer;
        skip(line_buffer_, " \t\r\n", false);

        while(line_buffer_.empty())
        {
            if (line_buffer_.empty())
                line_buffer_ = next_line();
            if (line_buffer_.empty())
                return buffer;
            skip(line_buffer_," \t\r\n", false);
        }

        for (size_t i = 0; i < line_buffer_.size(); i++)
        {
            char ch = line_buffer_[i];
            if (delims.find(ch) == delims.npos)
            {
                buffer.push_back(ch);
            }
            else
            {
                if (buffer.empty())
                {
                    buffer.push_back(ch);
                }
                break;
            }
        }
        line_buffer_ = line_buffer_.substr(buffer.size());
        return buffer;
    }
    std::string lower_case(const std::string &str)
    {
        std::string lower_str;
        for (size_t i = 0; i < str.size(); i++)
        {
            char ch = str[i];
            if (ch >= 'A' && ch <= 'Z')
            {
                lower_str.push_back(ch + ('a' - 'A'));
            }
            else
            {
                lower_str.push_back(ch);
            }
        }
        return lower_str;
    }
    char model_generator::look_ahead()
    {
        if (line_buffer_.empty())
        {
            line_buffer_ = next_line();
        }
        if (line_buffer_.empty())
            throw syntax_error("");

        return line_buffer_[0];
    }
    model_generator::token model_generator::get_next_token()
    {
        token t;
        if (tokens_.size() != 0)
        {
            t = tokens_.back();
            tokens_.pop_back();
            current_token_ = t;
            return t;
        }


        std::string str = next_token(" \r\n\t,()=`;-/");

        t.str_ = str;
        str = lower_case(str);

        if (str == "-")
        {
            if (look_ahead() == '-')
            {
                t.type_ = token::e_double_sub;
                t.str_ = "--";
            }
            else
            {
                t.type_ = token::e_sub;
            }
        }
        else if (str == "/")
        {
            if (look_ahead() == '*')
            {
                t.type_ = token::e_comment_begin;
                t.str_ = "/*";
            }
            else
            {
                t.type_ = token::e_sub;
            }
        }
        else if (str == ",")
        {
            t.type_ = token::e_comma;
        }
        else if (str == ";")
        {
            t.type_ = token::e_semicolon;
        }
        else if (str == "`")
        {
            t.type_ = token::e_backtick;
        }
        else if (str == "\"")
        {
            t.type_ = token::e_double_quote;
        }
        else if (str == "\'")
        {
            t.type_ = token::e_quote;
        }
        else if (str == "(")
        {
            t.type_ = token::e_open_paren;
        }
        else if (str == ")")
        {
            t.type_ = token::e_close_paren;
        }
        else if (str == "=")
        {
            t.type_ = token::e_eq;
        }
        else if (str == "*")
        {
            t.type_ = token::e_asterisk;
        }
        else if (str == "create")
        {
            t.type_ = token::e_create;
        }
        else if (str == "table")
        {
            t.type_ = token::e_table;
        }
        else if (str == "unique")
        {
            t.type_ = token::e_unique;
        }
        else if (str == "primary")
        {
            t.type_ = token::e_primary;
        }
        else if (str == "auto_increase")
        {
            t.type_ = token::e_auto_increase;
        }
        else if (str == "if")
        {
            t.type_ = token::e_if;
        }
        else if (str == "not")
        {
            t.type_ = token::e_not;
        }
        else if (str == "exist")
        {
            t.type_ = token::e_exist;
        }
        else if (str == "engine")
        {
            t.type_ = token::e_engine;
        }
        else if (str == "default")
        {
            t.type_ = token::e_default;
        }
        else if (str == "charset")
        {
            t.type_ = token::e_charset;
        }
        else if (str == "temporary")
        {
            t.type_ = token::e_temporary;
        }
        else if (str == "as")
        {
            t.type_ = token::e_as;
        }
        else if (str == "not")
        {
            t.type_ = token::e_not;
        }
        else if (str == "null")
        {
            t.type_ = token::e_null;
        }
        else if (str == "foreign")
        {
            t.type_ = token::e_foreign;
        }
        else if (str == "reference")
        {
            t.type_ = token::e_reference;
        }
        else if (str == "comment")
        {
            t.type_ = token::e_comment;
        }
        else if (str == "key")
        {
            t.type_ = token::e_key;
        }
        else if (str == "pack_keys")
        {
            t.type_ = token::e_pack_keys;
        }
        else if (str == "tinyint")
        {
            t.type_ = token::e_tinyint;
        }
        else if (str == "smallint")
        {
            t.type_ = token::e_smallint;
        }
        else if (str == "mediumint")
        {
            t.type_ = token::e_mediumint;
        }
        else if (str == "integer")
        {
            t.type_ = token::e_integer;
        }
        else if(str == "unsigned")
        {
            t.type_ = token::e_unsigned;
        }
        else if (str == "int")
        {
            t.type_ = token::e_int;
        }
        else if (str == "bigint")
        {
            t.type_ = token::e_bigint;
        }
        else if (str == "float")
        {
            t.type_ = token::e_float;
        }
        else if (str == "double")
        {
            t.type_ = token::e_double;
        }
        else if (str == "decimal")
        {
            t.type_ = token::e_decimal;
        }
        else if (str == "char")
        {
            t.type_ = token::e_char;
        }
        else if (str == "varchar")
        {
            t.type_ = token::e_varchar;
        }
        else if (str == "tinytext")
        {
            t.type_ = token::e_tinytext;
        }
        else if (str == "text")
        {
            t.type_ = token::e_text;
        }
        else if (str == "blob")
        {
            t.type_ = token::e_blob;
        }
        else if (str == "mediumtext")
        {
            t.type_ = token::e_mediumtext;
        }
        else if (str == "mediumblob")
        {
            t.type_ = token::e_mediumblob;
        }
        else if (str == "longtext")
        {
            t.type_ = token::e_longtext;
        }
        else if (str == "longblob")
        {
            t.type_ = token::e_longblob;
        }
        else if (str == "enum")
        {
            t.type_ = token::e_enum;
        }
        else if (str == "set")
        {
            t.type_ = token::e_set;
        }
        else if (str == "date")
        {
            t.type_ = token::e_date;
        }
        else if (str == "datetime")
        {
            t.type_ = token::e_datetime;
        }
        else if (str == "timestamp")
        {
            t.type_ = token::e_timestamp;
        }
        else if (str == "time")
        {
            t.type_ = token::e_time;
        }
        else if (str == "year")
        {
            t.type_ = token::e_year;
        }
        else 
        {
            t.type_ = token::e_identifier;
        }
        current_token_ = t;

        return t;
    }
    token model_generator::current_token()
    {
        return current_token_;
    }
    field::type model_generator::to_field_type(const token& t)
    {
        switch (t.type_)
        {
            case token::e_mediumint:
            case token::e_int:
            case token::e_integer:
                return field::e_int;
            case token::e_smallint:
                return field::e_char;
            case token::e_smallint:
                return field::e_short;
            case token::e_bigint:
                return field::e_long_long_int;
            case token::e_double:
            case token::e_decimal:
                return field::e_double;
            case token::e_float:
                return field::e_float;

            case token::e_date:
            case token::e_datetime:
            case token::e_year:
            case token::e_time:

            case token::e_char:
            case token::e_varchar:
            case token::e_text:
            case token::e_tinytext:
            case token::e_blob:
            case token::e_mediumblob:
            case token::e_mediumtext:
            case token::e_longtext:
            case token::e_longblob:
                return field::e_std_string;
            default:
                throw syntax_error("unknown type " + t.type_);
        }
        return field::e_void;
    }
    std::string model_generator::get_default()
    {
        std::string buffer;
        token t = get_next_token();
        if(t.type_ == token::e_quote ||
                t.type_ == token::e_double_quote)
        {
            buffer = next_token("'");
            line_buffer_ = line_buffer_.substr(buffer.size());

        }else
        {
            ///todo check if str_ is function,eg:now()
            buffer = t.str_;
        }
        return buffer;
    }
    std::string model_generator::get_string()
    {
        token t = get_next_token();

        if(t.type_ == token::e_backtick)
        {
            token t1 = get_next_token();
            token t2 = get_next_token();
            if(t2.type_ == token::e_backtick)
            {
                return t1.str_;
            }
            else
            {
                throw syntax_error();
            }
        }else if(t.type_ == token::e_identifier)
        {
             return t.str_;
        }
        return std::string();
    }

    model_generator::field model_generator::parse_field()
    {
        field f;

        f.name_ = get_string();
        f.type_ = to_field_type(get_next_token());

        token t = get_next_token();
        if(t.type_ == token::e_unsigned)
        {
            f.unsigned_ = true;
            t = get_next_token();
        }

        //eg: varchar(255) or double(10,10)
        if(t.type_ == token::e_open_paren)
        {
            t = get_next_token();
            f.type_len_.append(t.str_);
            while(true)
            {
                t = get_next_token();
                if(t.type_ == token::e_close_paren)
                {
                    t = get_next_token();
                    break;
                }
                else
                    f.type_len_.append(t.str_);
            }
        }

        while(t.type_ != token::e_comma)
        {
            if(t.type_ == token::e_primary)
            {
                t = get_next_token();
                if(t.type_ == token::e_key)
                {
                    f.primary_key_ = true;
                }

            }else if(t.type_ == token::e_auto_increase)
            {
                f.auto_increase_ = true;
            }
            else if(t.type_ == token::e_not)
            {
                t = get_next_token();
                if(t.type_ == token::e_null)
                {
                    f.not_null_ = true;
                }
                else
                {
                    throw syntax_error("unknown token " + t.str_);
                }
            }
            else if(t.type_ == token::e_default)
            {
                f.default_ = get_default();
            }
            else if(t.type_ == token::e_unique)
            {
                f.unique_ = true;
            }
            else
            {
                std::cout << t.str_ << std::endl;
            }
        }

        return f;
    }
    void model_generator::parse_table()
    {
        table1_.name_ = get_string();

        token t = get_next_token();
        if(t.type_ != token::e_open_paren)
            throw syntax_error();

        t = get_next_token();

        while(t.type_ != token::e_semicolon)
        {
            if(t.type_ == token::e_backtick ||
                    t.type_ == token::e_identifier)
            {
                field f = parse_field();
                table1_.fields_.push_back(f);
            }
            else if(t.type_ == token::e_primary)
            {
                t = get_next_token();
                if(t.type_ != token::e_key)
                    throw syntax_error("unknown token "+ t.str_);
            }
        }

    }
    void model_generator::parse()
    {
        do
        {
            token t = get_next_token();
            if(t.type_ == token::e_double_sub)
            {
                line_buffer_.clear();
            }
            else if(t.type_ == token::e_comment_begin)
            {
                while(t.type_ != token::e_comment_end)
                    t = get_next_token();
            }
            else if(t.type_ == token::e_create)
            {
                t = get_next_token();
                if(t.type_ == token::e_table)
                {
                    table1_ = table();
                    table1_.sql_  = get_string(';');
                    table1_.sql_.push_back(';');

                    parse_table();
                }
            }
        }while(true);
    }
    bool model_generator::parse_sql(const std::string &file_path)
    {
        return false;
    }
    void model_generator::gen_model(const std::string &path)
    {

    }
}