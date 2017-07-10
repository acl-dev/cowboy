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
                if (!skip_all && !!i)
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
    std::string model_generator::next_token(const std::string &delims)
    {
        std::string buffer;
        skip(line_buffer_, " \t\r\n", false);
        do
        {
            if (line_buffer_.empty())
                line_buffer_ = next_line();
            if (line_buffer_.empty())
                return buffer;
            skip(line_buffer_," \t\r\n", false);
        } while (true);

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


        std::string str = next_token();

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
    bool model_generator::parse_sql(const std::string &file_path)
    {
        return false;
    }
    void model_generator::gen_model(const std::string &path)
    {

    }
}