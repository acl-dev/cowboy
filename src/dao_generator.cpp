#include <iostream>
#include "syntax_error.hpp"
#include "common.hpp"
#include "dao_generator.h"

#define br  std::string("\r\n")
#define br2  std::string("\r\n\r\n")
#define tab std::string("\t")
#define tab2 std::string("\t\t")
#define tab3  std::string("\t\t\t")
#define tab4  std::string("\t\t\t\t")

namespace acl
{

    dao_generator::dao_generator()
    {
        lexer_ = NULL;
    }
    void dao_generator::push_back(dao_generator::token t)
    {
        lexer_->tokens_.push_back(t);
    }
    std::string dao_generator::get_string(char end_ch)
    {
        std::string buffer;
        std::string line_buffer = lexer_->line_buffer_;
        bool end = false;

        store_file_point();
        if (line_buffer.empty())
            line_buffer = get_line();
        do
        {
            for (int i = 0; i < line_buffer.size(); ++i)
            {
                char ch = line_buffer[i];
                if (ch != end_ch)
                    buffer.push_back(ch);
                else
                {
                    end = true;
                    break;
                }
            }
            if (end)
                break;
            line_buffer = get_line();
        } while (true);
        reload_file_point();
        return buffer;
    }
    void dao_generator::store_file_point()
    {
        lexer_->file_offset_ = lexer_->file_->ftell();
    }
    void dao_generator::reload_file_point()
    {
        lexer_->file_->fseek(lexer_->file_offset_, SEEK_SET);
    }
    dao_generator::token dao_generator::current_token()
    {
        return lexer_->current_token_;
    }
    dao_generator::token dao_generator::get_next_token(bool auto_skip_comment)
    {
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_comment)
            {
                t = get_next_token();
                if (t.type_ != token::e_$column &&
                    t.type_ != token::e_$delete &&
                    t.type_ != token::e_$include &&
                    t.type_ != token::e_$insert &&
                    t.type_ != token::e_$mapper &&
                    t.type_ != token::e_$mappers &&
                    t.type_ != token::e_$model &&
                    t.type_ != token::e_$models &&
                    t.type_ != token::e_$result &&
                    t.type_ != token::e_$select &&
                    t.type_ != token::e_$table &&
                    t.type_ != token::e_$update)
                {
                    lexer_->line_buffer_.clear();
                }
                else
                {
                    return t;
                }
            }
            else if (t.type_ == token::e_comment_begin)
            {
                t = get_next_token();
                if (t.type_ != token::e_$column &&
                    t.type_ != token::e_$delete &&
                    t.type_ != token::e_$include &&
                    t.type_ != token::e_$insert &&
                    t.type_ != token::e_$mapper &&
                    t.type_ != token::e_$mappers &&
                    t.type_ != token::e_$model &&
                    t.type_ != token::e_$models &&
                    t.type_ != token::e_$result &&
                    t.type_ != token::e_$select &&
                    t.type_ != token::e_$table &&
                    t.type_ != token::e_$update)
                {
                    while (get_next_token().type_ != token::e_comment_end);
                }
                else
                {
                    return t;
                }
            }
            return t;
        } while (true);
    }
    dao_generator::token dao_generator::get_next_token()
    {
        token t;

        if (!lexer_->tokens_.empty())
        {
            t = lexer_->tokens_.front();
            lexer_->tokens_.pop_front();
            lexer_->current_token_ = t;
            return t;
        }
        const std::string delimiters = " /\r\t\n<>(){};,:=-+.@?&*%";

        std::string str = next_token(delimiters);

        t.str_ = str;
        t.line_ = line_num();


        if (str == ",")
        {
            t.type_ = token::e_comma;
        }
        else if (str == ".")
        {
            t.type_ = token::e_dot;
        }
        else if (str == ";")
        {
            t.type_ = token::e_semicolon;
        }
        else if (str == ":")
        {
            t.type_ = token::e_colon;
            if (look_ahead(delimiters) == ":")
            {
                next_token(delimiters);
                t.type_ = token::e_double_colon;
                t.str_ = "::";
            }
        }
        else if (str == "(")
        {
            t.type_ = token::e_open_paren;
        }
        else if (str == ")")
        {
            t.type_ = token::e_close_paren;
        }
        else if (str == "{")
        {
            t.type_ = token::e_open_brace;
        }
        else if (str == "}")
        {
            t.type_ = token::e_close_brace;
        }
        else if (str == "<")
        {
            t.type_ = token::e_less;
        }
        else if (str == ">")
        {
            t.type_ = token::e_greate;
        }
        else if (str == "=")
        {
            t.type_ = token::e_equal;
        }
        else if (str == "&")
        {
            t.type_ = token::e_and;
        }
        else if (str == "/")
        {
            if (look_ahead(delimiters) == "/")
            {
                str = next_token(delimiters);
                t.type_ = token::e_comment;
                t.str_ = "//";
                t.line_ = line_num();
            }
            else if (look_ahead(delimiters) == "*")
            {
                str = next_token(delimiters);
                t.type_ = token::e_comment_begin;
                t.str_ = "/*";
                t.line_ = line_num();
            }
            else
                t.type_ = token::e_div;
        }
        else if (str == "*")
        {
            if (look_ahead(delimiters) == "/")
            {
                str = next_token(delimiters);
                t.type_ = token::e_comment_end;
                t.str_ = "*/";
                t.line_ = line_num();
            }
            else
                t.type_ = token::e_asterisk;
        }
        else if (str == "#include")
        {
            t.type_ = token::e_include;
        }
        else if (str == "#pragma")
        {
            str = next_token(delimiters);
            if (str == "once")
            {
                t.type_ = token::e_pragma_once;
                t.line_ = line_num();
                t.str_ = "#pragma once";
            }
        }
        else if (str == "unsigned")
        {
            str = next_token(delimiters);
            if (str == "int")
            {
                t.type_ = token::e_unsigned_int;
                t.str_ = "unsigned int";

            }
            else if (str == "char")
            {
                t.type_ = token::e_unsigned_char;
                t.str_ = "unsigned char";
            }
            else if (str == "long")
            {
                if (next_token(delimiters) == "long" &&
                    next_token(delimiters) == "int")
                {
                    t.type_ = token::e_unsigned_long_long_int;
                    t.str_ = "unsigned long long int";
                }
                else
                {
                    throw syntax_error("unknow type: " + lexer_->current_line_);
                }
            }
        }
        else if (str == "long")
        {
            if (next_token(delimiters) == "long" &&
                next_token(delimiters) == "int")
            {
                t.type_ = token::e_long_long_int;
                t.str_ = "long long int";
            }
            else
            {
                throw syntax_error("unknow type");
            }
        }
        else if (str == "char")
        {
            t.type_ = token::e_char;
        }
        else if (str == "short")
        {
            t.type_ = token::e_short;
        }
        else if (str == "int")
        {
            t.type_ = token::e_int;
        }
        else if (str == "float")
        {
            t.type_ = token::e_float;
        }
        else if (str == "const")
        {
            t.type_ = token::e_const;
        }
        else if (str == "double")
        {
            t.type_ = token::e_double;
        }
        else if (str == "void")
        {
            t.type_ = token::e_void;
        }
        else if (str == "bool")
        {
            t.type_ = token::e_bool;
        }
        else if (str == "struct")
        {
            t.type_ = token::e_struct;
        }
        else if (str == "virtual")
        {
            t.type_ = token::e_virtual;
        }
        else if (str == "using")
        {
            t.type_ = token::e_using;
        }
        else if (str == "namespace")
        {
            t.type_ = token::e_namespace;
        }
        else if (str == "@")
        {
            str = look_ahead(delimiters);

            if (str == "Table")
            {
                t.type_ = token::e_$table;
                t.str_ = "@Table";
                next_token(delimiters);
            }
            else if (str == "Model")
            {
                t.type_ = token::e_$model;
                t.str_ = "@Model";
                next_token(delimiters);
            }
            else if (str == "Models")
            {
                t.type_ = token::e_$models;
                t.str_ = "@Models";
                next_token(delimiters);
            }
            else if (str == "Column")
            {
                t.type_ = token::e_$column;
                t.str_ = "@Column";
                next_token(delimiters);
            }
            else if (str == "Delete")
            {
                t.type_ = token::e_$delete;
                t.str_ = "@Delete";
                next_token(delimiters);
            }
            else if (str == "Update")
            {
                t.type_ = token::e_$update;
                t.str_ = "@Update";
                next_token(delimiters);
            }
            else if (str == "Insert")
            {
                t.type_ = token::e_$insert;
                t.str_ = "@Insert";
                next_token(delimiters);
            }
            else if (str == "Select")
            {
                t.type_ = token::e_$select;
                t.str_ = "@Select";
                next_token(delimiters);
            }
            else if (str == "Mappers")
            {
                t.type_ = token::e_$mappers;
                t.str_ = "@Mappers";
                next_token(delimiters);
            }
            else if (str == "Mapper")
            {
                t.type_ = token::e_$mapper;
                t.str_ = "@Mapper";
                next_token(delimiters);
            }
            else if (str == "Result")
            {
                t.type_ = token::e_$result;
                t.str_ = "@Result";
                next_token(delimiters);
            }
            else if (str == "Include")
            {
                t.type_ = token::e_$include;
                t.str_ = "@Include";
                next_token(delimiters);
            }
            else
                t.type_ = token::e_$;
        }
        else if (str.empty())
        {
            t.type_ = token::e_eof;
            t.line_ = line_num();
        }
        else if (str == "std")
        {
            if (next_token(delimiters) == ":" && next_token(delimiters) == ":")
            {
                str = next_token(delimiters);
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
            }
        }
        else
        {
            t.type_ = token::e_identifier;
            t.str_ = str;
        }

        t.line_ = line_num();
        lexer_->current_token_ = t;
        return t;
    }



    /*" \/\r\t\n<>(){};,=-+.@?"*/
    std::string dao_generator::next_token(const std::string &delims)
    {
        if (lexer_->line_buffer_.empty())
        {
            lexer_->line_buffer_ = next_line();
            if (lexer_->line_buffer_.empty())
                return std::string();
        }

        skip_space();
        /// read a line from file. and get token.
        if (lexer_->line_buffer_.empty())
            return next_token(delims);


        lexer_->token_buf_.clear();
        for (size_t i = 0; i < lexer_->line_buffer_.size(); i++)
        {
            char ch = lexer_->line_buffer_[i];
            if (delims.find(ch) == delims.npos)
            {
                lexer_->token_buf_.push_back(ch);
            }
            else
            {
                if (lexer_->token_buf_.empty())
                    lexer_->token_buf_.push_back(ch);
                break;
            }
        }
        if (lexer_->token_buf_.size())
        {
            lexer_->line_buffer_ = lexer_->line_buffer_.substr(lexer_->token_buf_.size());
        }
        return lexer_->token_buf_;
    }
    std::string dao_generator::get_line()
    {
        acl::string buffer;
        lexer_->file_->gets(buffer, false);
        return buffer.c_str();
    }
    std::string dao_generator::next_line()
    {
        acl::string buffer;

        lexer_->file_->gets(buffer, false);
        lexer_->current_line_ = buffer;
        // read line ok
        if (buffer.size())
            lexer_->line_num_++;

        return buffer.c_str();
    }
    std::string dao_generator::look_ahead(const std::string &delims)
    {
        std::string current_line = lexer_->line_buffer_;

        long long offset = lexer_->file_->ftell();

        while (current_line.empty())
        {

            acl::string line;

            if (lexer_->file_->gets(line, true))
            {
                current_line = line.c_str();
            }
            skip(current_line, " \r\n\t");
        }

        lexer_->file_->fseek(offset, SEEK_SET);

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
    void dao_generator::skip_space(std::string &line)
    {
        skip(line, " \r\n\t");
    }
    void dao_generator::skip_space()
    {
        skip(lexer_->line_buffer_, " \r\n\t");
    }
    int dao_generator::line_num()
    {
        return lexer_->line_num_;
    }
    dao_generator::field::type_t 
        dao_generator::to_field_type(const token &t)
    {
        if (t.type_== token::e_int)
        {
            return field::e_int;
        }
        else if (t.type_ == token::e_unsigned_int)
        {
            return field::e_unsigned_int;
        }
        else if (t.type_ == token::e_float)
        {
            return field::e_float;
        }
        else if (t.type_ == token::e_double)
        {
            return field::e_double;
        }
        else if (t.type_ == token::e_std_string ||
                 t.type_ == token::e_acl_string)
        {
            return field::e_string;
        }
        else if (t.type_ == token::e_void)
        {
            return field::e_void;
        }
        else if (t.type_ == token::e_bool)
        {
            return field::e_bool;
        }
        else if (t.type_ == token::e_long_long_int)
        {
            return field::e_long_long_int;
        }
        else if (t.type_ == token::e_unsigned_long_long_int)
        {
            return field::e_unsigned_long_long_int;
        }
        else if (t.type_ == token::e_char)
        {
            return field::e_char;
        }
        else if (t.type_ == token::e_unsigned_char)
        {
            return field::e_unsigned_char;
        }
        else if (t.type_ == token::e_short)
        {
            return field::e_short;
        }
        else if (t.type_ == token::e_unsigned_short)
        {
            return field::e_unsigned_short;
        }
        throw syntax_error();
        return field::e_void;
    }

    dao_generator::model_t
        dao_generator::get_model(const std::string &name,
                                 const std::vector<std::string> &namespaces)
    {
        model_t e;
        std::string error = "not find " + namespace_to_string(namespaces) + name;

        std::vector<model_t >::iterator it = models_.begin();
        for (; it != models_.end(); ++it)
        {
            if (it->name_ == name)
            {
                if (it->namespaces_.empty() ||
                    it->namespaces_ == namespaces||
                    it->namespaces_ == cur_namespaces())
                    return *it;
            }
        }
        throw syntax_error(error);
        return e;
    }
    std::vector<dao_generator::field>
        dao_generator::get_fields(const std::string &name,
                                  const std::vector<std::string> &namespaces)
    {
        std::vector<dao_generator::field> fields;
        std::string error = "not find " + namespace_to_string(namespaces) + name;

        std::vector<model_t>::iterator it = models_.begin();
        for (; it != models_.end(); ++it)
        {
            if (it->name_ == name)
            {
                if (it->namespaces_.empty() ||
                    it->namespaces_ == namespaces ||
                    it->namespaces_ == cur_namespaces())
                    return it->fields_;

            }
        }
        throw syntax_error(error);
        return std::vector<dao_generator::field>();
    }
    bool dao_generator::check_model_exist(std::string &name,
                                    const std::vector<std::string> &namespaces)
    {
        std::vector<model_t>::iterator it = models_.begin();
        for (; it != models_.end(); ++it)
        {
            if (it->name_ == name)
            {
                if (it->namespaces_.empty() || 
                    it->namespaces_ == namespaces||
                    it->namespaces_ == cur_namespaces())
                    return true;
            }
        }
        return false;
    }

    bool dao_generator::parse_file(const std::string &file_path)
    {
        try
        {
            lexer_ = new_lexer(file_path);
            if (!lexer_)
                throw std::runtime_error("new_lexer error" + file_path);
            lexers_.push_back(lexer_);
            analyzers_.push_back(analyzer_t());
            bool rc = false;

            token t = get_next_token(true);
            if (t.type_ == token::e_$models)
            {
                rc = parse_model_file();
                model_files_.push_back(get_filename(file_path));
            }
            else
            {
                rc = parse_mapper_file();
                mapper_files_.push_back(get_filename(file_path));
            }
            delete_lexer(lexer_);
            lexers_.pop_back();
            analyzers_.pop_back();
            if (lexers_.size() != 0)
                lexer_ = lexers_.back();
        }
        catch (std::exception &e)
        {
            printf("%s\n", e.what());
            printf("file_path:%s\n", lexer_->file_path_.c_str());
            printf("line:%d   %s   >>%s",
                   lexer_->line_num_,
                   lexer_->current_line_.c_str(),
                   lexer_->line_buffer_.c_str());
            return false;
        }
        return true;
    }
    std::vector<std::string> dao_generator::get_namespace()
    {
        std::vector<std::string> namespaces;
        do
        {
            token t1 = get_next_token(true);
            token t2 = get_next_token(true);
            //:namespaces::namespaces::...class
            if (t2.type_ == token::e_double_colon)
            {
                namespaces.push_back(t1.str_);
            }
            else
            {
                push_back(t1);
                push_back(t2);
                break;
            }
        } while (true);

        return namespaces;
    }
    //@include{}
    void dao_generator::parse_include()
    {
        std::string file_path;

        token t = get_next_token();
        if (t.type_ != token::e_open_brace)
            throw syntax_error("not find {");

        do
        {
            t = get_next_token();
            if (t.type_ != token::e_close_brace)
                file_path.append(t.str_);
            else
                break;
        } while (true);
        for (size_t i = 0; i < model_files_.size(); ++i)
        {
            if (model_files_[i] == get_filename(file_path))
                return;
        }
        parse_file(file_path);
    }
    void dao_generator::skip_construct_func()
    {
        int count = 0;
        do
        {
            token t = get_next_token(true);
            if (t.type_ == token::e_open_brace)
                count++;
            else if (t.type_ == token::e_close_brace)
            {
                count--;
                if (count == 0)
                    break;
            }

        }while (true);
    }
    void dao_generator::parse_model()
    {
        cur_model().name_ = get_next_token(true).str_;
        token t = get_next_token(true);
        if (t.type_ == token::e_colon)
        {
            std::vector<std::string> namespaces = get_namespace();
            t = get_next_token(true);
            if (!check_model_exist(t.str_, namespaces))
            {
                throw syntax_error(std::string("not found ") + t.str_);
            }
            cur_model().fields_ = get_fields(t.str_, namespaces);
            //get { token
            t = get_next_token(true);
        }
        if (t.type_ != token::e_open_brace)
            throw syntax_error("not find {");

        std::string column;
        do
        {
            field f;
            t = get_next_token(true);
            if (t.type_ == token::e_$column)
            {
                t = get_next_token(true);
                if (t.type_ != token::e_open_brace)
                    throw syntax_error();
                t = get_next_token(true);
                if (t.type_ != token::e_identifier)
                    throw syntax_error();
                column = t.str_;
                t = get_next_token(true);
                if (t.type_ != token::e_close_brace)
                    throw syntax_error();
            }
            else if (t.type_ == token::e_unsigned_int ||
                     t.type_ == token::e_unsigned_char ||
                     t.type_ == token::e_unsigned_short ||
                     t.type_ == token::e_unsigned_long_long_int ||
                     t.type_ == token::e_bool ||
                     t.type_ == token::e_char ||
                     t.type_ == token::e_short ||
                     t.type_ == token::e_int ||
                     t.type_ == token::e_long_long_int ||
                     t.type_ == token::e_float ||
                     t.type_ == token::e_double ||
                     t.type_ == token::e_acl_string ||
                     t.type_ == token::e_std_string)
            {
                std::string field_name = get_next_token(true).str_;

                if (get_next_token(true).type_ != token::e_semicolon)
                    throw syntax_error("not find ;");

                f.type_ = to_field_type(t);
                f.name_ = field_name;
                f.type_str_ = t.str_;
                f.column_ = field_name;
                if (column.size())
                {
                    f.column_ = column;
                    column.clear();
                }
                cur_model().fields_.push_back(f);
            }
            else if (t.type_ == token::e_std_list ||
                     t.type_ == token::e_std_vector)
            {
                // <
                if (get_next_token(true).type_ != token::e_less)
                    throw syntax_error();
                std::vector<std::string> namespaces = get_namespace();

                std::string model_name = get_next_token(true).str_;

                if (!check_model_exist(model_name, namespaces))
                    throw syntax_error(std::string("can't find ") + model_name);

                if (get_next_token(true).type_ != token::e_greate)
                    throw syntax_error();

                std::string field_name = get_next_token(true).str_;

                if (get_next_token(true).type_ != token::e_semicolon)
                    throw syntax_error("not find ; ");

                f.type_str_ = t.str_;
                f.type_str_.append("<");
                f.type_str_.append(model_name);
                f.type_str_.append(">");
                f.model_ = new model_t(get_model(model_name, namespaces));
                f.type_ = field::e_vector;
                f.name_ = field_name;
                column.clear();
                cur_model().fields_.push_back(f);
            }
            else if (t.type_ == token::e_identifier)
            {
                if (t.str_ == cur_model().name_)
                {
                    if (get_next_token().type_ == token::e_open_paren &&
                        get_next_token().type_ == token::e_close_paren)
                    {
                        skip_construct_func();
                        continue;
                    }
                }
                push_back(t);

                std::vector<std::string> namespaces= get_namespace();
                t = get_next_token(true);

                if (!check_model_exist(t.str_, namespaces))
                    throw syntax_error(std::string("can't find model:") + t.str_);

                std::string field_name = get_next_token().str_;
                f.type_ = field::e_entry;
                f.model_ = new model_t(get_model(t.str_, namespaces));
                f.name_ = field_name;
                f.type_str_ = t.str_;

                column.clear();
                cur_model().fields_.push_back(f);
                continue;

            }
            else if (t.type_ == token::e_close_brace)
            {
                t = get_next_token();
                if (t.type_ != token::e_semicolon)
                    throw syntax_error("not found ; ");
                cur_model().filepath_ = lexer_->file_path_;
                models_.push_back(cur_model());
                //reset current model
                cur_model() = model_t();
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

            t = get_next_token(true);

            if (t.type_ == token::e_close_paren)
            {
                return params;
            }
            else if (t.type_ == token::e_const)
            {
                continue;
            }
            else if (t.type_ == token::e_unsigned_int ||
                     t.type_ == token::e_unsigned_char ||
                     t.type_ == token::e_unsigned_short ||
                     t.type_ == token::e_unsigned_long_long_int ||
                     t.type_ == token::e_bool ||
                     t.type_ == token::e_char ||
                     t.type_ == token::e_short ||
                     t.type_ == token::e_int ||
                     t.type_ == token::e_long_long_int ||
                     t.type_ == token::e_float ||
                     t.type_ == token::e_double ||
                     t.type_ == token::e_acl_string ||
                     t.type_ == token::e_std_string)
            {
                param.type_ = to_field_type(t);
                param.type_str_ = t.str_;
                t = get_next_token();
                if (t.type_ == token::e_and)
                    t = get_next_token();
                param.name_ = t.str_;
                params.push_back(param);
            }

            else if (t.type_ == token::e_identifier)
            {
                push_back(t);

                std::vector<std::string> namespaces = get_namespace();
                t = get_next_token(true);
                if (!check_model_exist(t.str_, namespaces))
                    throw syntax_error(std::string("not found model:") + t.str_);

                param.type_ = field::e_entry;
                param.model_ = new model_t(get_model(t.str_, namespaces));
                param.type_str_ = t.str_;

                t = get_next_token();
                if (t.type_ == token::e_and)//&
                    t = get_next_token();

                param.name_ = t.str_;
                cur_mapper().model_files_.insert(param.model_->filepath_);
                params.push_back(param);
            }
            else if (t.type_ == token::e_std_list ||
                     t.type_ == token::e_std_vector)
            {
                param.type_str_ = t.str_;
                if (get_next_token().type_ != token::e_less)
                    throw syntax_error("not find <");

                std::vector<std::string> namespaces = get_namespace();
                t = get_next_token(true);
                if (!check_model_exist(t.str_, namespaces))
                    throw syntax_error(std::string("not found model:") + t.str_);

                if (get_next_token().type_ != token::e_greate)
                    throw syntax_error("not find >");

                param.model_ = new model_t(get_model(t.str_, namespaces));

                t = get_next_token();
                if (t.type_ == token::e_and)
                    t = get_next_token();

                param.type_ = field::e_vector;
                param.name_ = t.str_;
                cur_mapper().model_files_.insert(param.model_->filepath_);
                params.push_back(param);
            }

            t = get_next_token();
            if (t.type_ == token::e_close_paren)
            {
                return params;
            }
            else if (t.type_ != token::e_comma)
            {
                throw syntax_error("not find \",\"");
            }

        } while (true);
    }

    std::vector<std::string> dao_generator::get_sql_param()
    {
        std::vector<std::string> params;
        token t = get_next_token();
        while (t.type_ != token::e_close_brace)
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
    void dao_generator::update_function_columns(mapper_function &func)
    {
        if (func.type_ != mapper_function::e_select)
            return;

        //@Result column="cid" property="id"
        //@Result column="oid" property="order.id"

        for (size_t i = 0; i < func.columns_.size(); i++)
        {
            std::string property = func.columns_[i].property_;
            std::string column = func.columns_[i].column_;

            std::vector<field> &fields = func.params_[0].model_->fields_;

            size_t npos = property.find_first_of('.');
            if (npos == std::string::npos)
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
                    throw syntax_error("not find property:"+property);
            }
            else
            {
                std::string first = property.substr(0, npos);
                std::string second = property.substr(npos + 1);

                bool found = false;

                for (size_t j = 0; j < fields.size(); j++)
                {
                    field &f = fields[j];
                    if (f.name_ == first)
                    {
                        for (size_t k = 0; k < f.model_->fields_.size(); k++)
                        {
                            field &f2 = f.model_->fields_[k];
                            if (f2.name_ == second)
                            {
                                f2.column_ = column;
                                found = true;
                            }
                        }
                    }
                }
                if (!found)
                    throw syntax_error("not find property:" + property);
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
            token t = get_next_token(true);
            if (t.type_ == t.e_$result)
            {
                if (get_next_token().type_ != token::e_open_brace)
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
                std::string property = lexer_->line_buffer_;
                skip_space(property);
                property = property.substr(0, property.find_last_of('}'));

                mapper_function::result result;
                result.column_ = column.str_;
                result.property_ = property;
                result.str_ = lexer_->current_line_;
                result.line_ = lexer_->line_num_;
                results.push_back(result);

                lexer_->line_buffer_.clear();
            }
            else if(t.type_ == token::e_virtual)
            {
                push_back(t);
                break;
            }

        } while (true);

        return results;
    }
    dao_generator::mapper_function::type_t 
        dao_generator::token_to_function_type(const token &t)
    {
        
        if (t.type_ == token::e_$update)
            return mapper_function::e_update;
        else if (t.type_ == token::e_$delete)
            return mapper_function::e_delete;
        else if (t.type_ == token::e_$select)
            return mapper_function::e_select;
        else if(t.type_ == token::e_$insert)
            return mapper_function::e_insert;
        throw syntax_error("unknown function type "+t.str_);
        return mapper_function::e_insert;;
    }
    void dao_generator::parse_mapper_struct()
    {
        cur_mapper().name_= get_next_token(true).str_;

        if (get_next_token().type_ != token::e_open_brace)
            throw syntax_error();

        do
        {
            token t = get_next_token(true);

            if (t.type_ == token::e_$insert ||
                t.type_ == token::e_$update ||
                t.type_ == token::e_$delete ||
                t.type_ == token::e_$select)
            {
                mapper_function fun;

                fun.type_ = token_to_function_type(t);
                if (get_next_token(true).type_ != token::e_open_brace)
                    throw syntax_error();

                fun.sql_ = get_string('}');
                fun.sql_params_ = get_sql_param();


                do
                {
                    t = get_next_token();
                    if (t.type_ == token::e_$result ||
                        t.type_ == token::e_virtual)
                    {
                        push_back(t);
                        break;
                    }
                } while (true);

                t = get_next_token();
                push_back(t);
                if (t.type_ == token::e_$result)
                    fun.columns_ = get_result_columns();

                if (get_next_token(true).type_ != token::e_virtual ||
                    get_next_token(true).type_ != token::e_bool)
                    throw syntax_error("not find \"virtual bool\"");

                fun.declare_ = "virtual bool " + get_string('=');

                fun.return_.type_ = field::e_bool;

                //get function name;
                fun.name_ = get_next_token().str_;

                fun.params_ = get_mapper_func_params();

                if (fun.params_.empty())
                    throw syntax_error("not find params error");

                update_function_columns(fun);

                t = get_next_token(true);
                if (t.type_ != t.e_equal)
                    throw syntax_error("not find \"=\"");
                if (get_next_token(true).type_ != token::e_identifier)
                    throw syntax_error("not find \"0\"");
                if (get_next_token(true).type_ != token::e_semicolon)
                    throw syntax_error("not find \";\"");
                cur_mapper().mfuncs_.push_back(fun);
            }
            else if (t.type_ == token::e_close_brace)
            {
                t = get_next_token();
                if (t.type_ != token::e_semicolon)
                    throw syntax_error("not find ;");
                cur_mapper().mapper_path_ = lexer_->file_path_;
                mappers_.push_back(cur_mapper());
                //reset cur_mapper();
                cur_mapper() = mapper_t();
                break;
            }
        } while (true);
    }
    bool dao_generator::parse_mapper_file()
    {
        do
        {
            token t = get_next_token(true);
            if (t.type_ == token::e_namespace)
            {
                std::string name = get_next_token().str_;
                if (get_next_token(true).type_ != token::e_open_brace)
                    throw syntax_error("not find {");
                cur_namespaces().push_back(name);
            }
            else if (t.type_ == token::e_close_brace)
            {
                if (cur_namespaces().empty())
                    throw syntax_error("unknown } token");
                cur_namespaces().pop_back();
            }
            else if (t.type_ == token::e_$mapper)
            {
                t = get_next_token(true);
                if (t.type_ == token::e_struct)
                {
                    parse_mapper_struct();
                    continue;
                }
            }
            else if (t.type_ == token::e_eof)
            {
                if (cur_namespaces().size())
                    throw syntax_error("\"{\" not match \"}\"");
                return true;
            }
        } while (true);
    }
    bool dao_generator::parse_model_file()
    {
        do
        {
            token t = get_next_token(true);

            if (t.type_ == token::e_$include)
            {
                parse_include();
            }
            else if (t.type_ == token::e_$model)
            {
                cur_model().model_ = true;
            }
            else if (t.type_ == token::e_$table)
            {
                //{
                t = get_next_token(true);
                if (t.type_ != token::e_open_brace)
                    throw syntax_error();
                // table name
                cur_model().table_name_ = get_next_token(true).str_;

                //}
                t = get_next_token();
                if (t.type_ != token::e_close_brace)
                    throw syntax_error();
            }
            else if (t.type_ == token::e_namespace)
            {
                t = get_next_token(true);
                if (get_next_token(true).type_ != token::e_open_brace)
                    throw syntax_error("not find {");
                cur_namespaces().push_back(t.str_);

            }
            else if (t.type_ == token::e_struct)
            {
                parse_model();
            }
            else if (t.type_ == token::e_close_brace)
            {
                if (cur_namespaces().empty())
                    throw syntax_error("error }");
                cur_namespaces().pop_back();
            }
            else if (t.type_ == token::e_eof)
            {
                return true;
            }

        } while (true);

        return false;
    }

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
    std::string dao_generator::gen_func_impl_name(
        const std::string &class_name,
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
                    std::string str = code.substr(i + 1);
                    skip_space(str);
                    return "bool " + class_name + "::" + str;
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
    std::string dao_generator::gen_annotation(
        const mapper_function &func, bool tab_)
    {
        std::string code;
        if (func.type_ == mapper_function::e_insert)
            code += "@Insert{";
        else if (func.type_ == mapper_function::e_update)
            code += "@Update{";
        else if (func.type_ == mapper_function::e_delete)
            code += "@Delete{";
        else if (func.type_ == mapper_function::e_select)
            code += "@Select{";
        code += func.sql_ + "}";

        code = "/*" + code + "*/";
        code += br;
        if (tab_)
            code = tab + code;


        for (size_t i = 0; i < func.columns_.size(); i++)
        {
            if (tab_)
                code += tab;

            code += "//@Result{column=" + func.columns_[i].column_;
            code += ", property=" + func.columns_[i].property_ + "}" + br;
        }

        return code;
    }

    std::string dao_generator::gen_class_declare(const mapper_t &m)
    {
        std::string class_name = get_class_name(m.name_);
        std::string code;

        code += "class " + class_name + ": public " +
            namespace_to_string(m.namespaces_) + m.name_ + br;

        code += "{" + br;
        code += "public:" + br;
        code += tab + class_name + "(acl::db_handle& handle);";
        code += br;
        code += br;

        std::vector<mapper_function>::const_iterator it = m.mfuncs_.begin();
        for (; it != m.mfuncs_.end(); ++it)
        {
            code += gen_annotation(*it);
            code += tab;
            code += it->declare_ + ";";
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
    std::string dao_generator::gen_query_set_parameters(
        const mapper_function &func)
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
                    std::vector<field> fields = f.model_->fields_;

                    for (size_t k = 0; k < fields.size(); k++)
                    {
                        field &f2 = fields[k];
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
    std::string dao_generator::get_assign_code(const field &f,
                                               const std::string &str)
    {
        if (f.type_ == field::e_int)
        {
            return std::string("atoi(") + str + ");";
        }
        else if (f.type_ == field::e_unsigned_int)
        {
            return std::string("(unsigned int)atoi(") + str + ");";
        }
        else if(f.type_ == field::e_short)
        {
            return std::string("(short)atoi(") + str + ");";
        }
        else if (f.type_ == field::e_unsigned_short)
        {
            return std::string("(unsigned short)atoi(") + str + ");";
        }
        else if (f.type_ == field::e_long_long_int)
        {
            return std::string("atoll(") + str + ");";
        }
        else if (f.type_ == field::e_unsigned_long_long_int)
        {
            return std::string("(unsigned long long int)atoll(") + str + ");";
        }
        else if (f.type_ == field::e_float ||f.type_ == field::e_double)
        {
            return std::string("atof(") + str + ");";
        }
        return str + ";";
    }


    std::string dao_generator::get_define_columns(
        const std::vector<field> &fields,
        const std::string &prefix,
        const std::string &tabs,
        bool _br)
    {
        std::string code;

        for (size_t j = 0; j < fields.size(); j++)
        {
            code += tabs + get_define_column(fields[j], prefix, _br);
        }
        return code;
    }
    std::string dao_generator::get_define_column(const field &f,
                                                 const std::string &prefix,
                                                 bool _br)
    {
        std::string buffer;
        buffer = std::string("const char* ") + prefix + f.column_ +
            " = (*row)[\"" + f.column_ + "\"];";
        if (_br)
            buffer += br;
        return buffer;
    }
    std::string dao_generator::gen_query_code(const mapper_function &func)
    {
        std::string code;

        std::string sql = skip_multi_space(
            replace(skip_all(func.sql_, "\r\n"), '\t', ' '));


        code += tab + "acl::query query;" + br;
        code += tab + "query.create_sql(\"" + sql + "\");" + br;
        code += gen_query_set_parameters(func);
        code += br;
        code += br;

        if (func.log_)
        {
            code += tab + "logger(\"sql: %s\", "
                "query.to_string().c_str());" + br;
            code += br;
        }
        return code;
    }
    std::string dao_generator::gen_exec_update_code()
    {
        std::string code;

        code += tab + "if (!db_handle_.exec_update(query))" + br;
        code += tab + "{" + br;
        code += tab + tab + "logger_error(\"exec_update() failed :%s\","
            "db_handle_.get_error());" + br;
        code += tab + tab + "return false;" + br;
        code += tab + "}" + br;

        code += tab + "if (!db_handle_.commit())" + br;
        code += tab + "{" + br;
        code += tab + tab + "logger_error(\"commit() failed :%s\","
            "db_handle_.get_error());" + br;
        code += tab + tab + "return false;" + br;
        code += tab + "}" + br;

        code += tab + "return true;" + br;
        
        return code;
    }
    std::string dao_generator::gen_exec_select_once_code(const mapper_function &func)
    {
        std::string code;

        code += tab + "for (size_t i = 0; i < db_handle_.length(); ++i)" + br;
        code += tab + "{" + br;
        code += tab + tab + "const acl::db_row* row = db_handle_[i];" + br + br;

        std::vector<field> &fields = func.params_[0].model_->fields_;
        std::string result = func.params_[0].name_;
        //const char* column      = (*row)["column"];
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ == field::e_entry ||
                f.type_ == field::e_vector)
            {
                code += br;
                code += get_define_columns(f.model_->fields_, "$$", tab2);
            }
            else
            {
                code += tab2 + get_define_column(f, "$");
            }
        }

        code += br;

        //if (address)
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ != field::e_entry &&
                f.type_ != field::e_vector)
            {
                code += tab2 + "if(" + "$" + f.column_ + ")" + br;
                code += tab3 + result +"." + f.name_ + " = ";
                code += get_assign_code(f, "$" +f.column_) + br;
            }
        }

        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ == field::e_entry)
            {
                std::vector<field> &fields2 = f.model_->fields_;
                code += br;
                for (size_t j = 0; j < fields2.size(); j++)
                {
                    field &child = fields2[j];
                    code += tab2 + "if($$" + child.column_ + ")" + br;
                    code += tab3 + result + "." + f.name_ + "." + child.name_ + " = ";
                    code += get_assign_code(child, "$$" + child.column_) + br;
                }
            }
        }

        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ == field::e_vector)
            {
                std::string item = "$item";
                code += br;
                code += tab2 + namespace_to_string(f.model_->namespaces_);
                code += f.model_->name_ + " " + item + ";" + br;

                std::vector<field> &fields2 = f.model_->fields_;

                for (size_t j = 0; j < fields2.size(); j++)
                {
                    field &child = fields2[j];
                    code += tab2 + "if($$" + child.column_ + ")" + br;
                    code += tab3 + item + "." + child.name_ + " = ";
                    code += get_assign_code(child, "$$" + child.column_) + br;
                }

                code += tab + tab + result + "." + f.name_ +
                    ".push_back(" + item + ");" + br;
            }
        }

        code += tab + "}" + br;
        code += tab + "return !!db_handle_.length();" + br;
        return code;
    }
    std::string dao_generator::gen_exec_select_many_code(const mapper_function &func)
    {
        std::string code;

        std::string obj = "$item";

        code += tab + "for (size_t i = 0; i < db_handle_.length(); ++i)" + br;
        code += tab + "{" + br;
        code += tab2 + namespace_to_string(func.params_.front().model_->namespaces_);
        code += func.params_.front().model_->name_ +" " + obj + ";" + br;

        code += tab + tab + "const acl::db_row* row = db_handle_[i];" +br + br;

        std::vector<field> &fields = func.params_[0].model_->fields_;

        //const char* column = (*row)["column"];
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ != field::e_entry && f.type_ == field::e_vector)
            {
                code += tab2 + get_define_column(f, "$");
            }
        }

        code += br;

        //const char* column  = (*row)["column"];
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ == field::e_entry)
            {
                code += br;
                code += get_define_columns(f.model_->fields_, "$$", tab2);
            }
        }

        code += br;

        //if (address)
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ != field::e_entry &&
                f.type_ != field::e_vector)
            {
                code += tab + tab +
                    "if(" + "$" + f.column_ + ")" + br;
                code += tab + tab + tab +
                    obj + "." + f.name_ + " = ";
                code += get_assign_code(f, "$" + f.column_) + br;
            }
        }

        // one to once
        for (size_t i = 0; i < fields.size(); i++)
        {
            field &f = fields[i];
            if (f.type_ == field::e_entry)
            {
                std::vector<field> &fields2 = f.model_->fields_;
                code += br;
                for (size_t l = 0; l < fields2.size(); l++)
                {
                    field &child = fields2[l];
                    code += tab2 + "if($$" +child.column_ + ")" + br;
                    code += tab3 + obj +"." + f.name_ + "." +child.name_ + " = ";
                    code += get_assign_code(child, "$$" + child.column_) + br;
                }
            }
        }

        // one to many 
        for (size_t i = 0; i < fields.size(); i++)
        {

            field &f = fields[i];
            if (f.type_ == field::e_vector)
            {
                code += br;
                code += tab + tab +
                    "for (; i< db_handle_.length(); i++)" + br;
                code += tab + tab + "{" + br;
                std::string item = "$$item";
                code += tab + tab + tab +
                    namespace_to_string(f.model_->namespaces_) +
                    f.model_->name_ + " " + item + ";";

                std::vector<field> &fields2 = f.model_->fields_;
                code += br;

                for (size_t k = 0; k < fields2.size(); k++)
                {
                    field &f2 = fields2[k];
                    code += tab + tab + tab + "const char* " +
                        "$$" + f2.column_ +
                        " = (*row)[\"" + f2.column_ + "\"];" + br;
                }
                code += br;

                for (size_t ii = 0; ii < fields.size(); ii++)
                {
                    field &child = fields[ii];
                    if (!(child.type_ == field::e_entry ||
                          child.type_ == field::e_vector))
                    {
                        code += tab + tab + tab +
                            "const char* " +
                            "$$$" + child.column_ +
                            " = (*row)[\"" +
                            child.column_ + "\"];" + br;
                    }
                }
                code += br;
                code += tab3 + "if (";

                for (size_t j = 0; j < fields.size(); j++)
                {
                    field &child = fields[j];
                    if (!(child.type_ == field::e_entry ||
                          child.type_ == field::e_vector))
                    {
                        if (j != 0)
                            code += tab4;
                        code += "!streq($$$" + child.column_ + ", $" +
                            child.column_ + ")";

                        if (j == fields.size() - 2)
                            code += ")" + br;
                        else
                            code += " ||" + br;
                    }
                }
                use_streq_ = true;
                code += tab3 + "{" + br;
                code += tab4 + "--i;" + br;
                code += tab4 + "break;" + br;
                code += tab3 + "}" + br;

                for (size_t h = 0; h < fields2.size(); h++)
                {
                    field &f2 = fields2[h];
                    code += tab3 + "if($$" + f2.column_ + ")" + br;
                    code += tab4 + item + "." + f2.name_ + " = ";
                    code += get_assign_code(f2, "$$" + f2.column_) + br;
                }
                code += br;
                code += tab3 + obj + "." +
                    f.name_ + ".push_back(" + item + ");" + br;

                code += br;
                code += tab + tab + "}" + br;
            }
        }
        code += br;
        code += tab + tab + func.params_.front().name_ +
            ".push_back(" + obj + ");" + br;
        code += tab + "}" + br;
        code += tab + "return db_handle_.length() != 0;" + br;

        return code;
    }
    std::string dao_generator::gen_exec_select_code(const mapper_function &func)
    {
        std::string code;

        code += tab + "if (!db_handle_.exec_select(query))" + br;
        code += tab + "{" + br;
        code += tab + tab + "logger_error(\"exec_select() "
            "failed :%s\",db_handle_."
            "get_error());" + br;
        code += tab + tab + "return false;" + br;
        code += tab + "}" + br;
        code += br;

        if (func.params_.front().type_ == field::e_entry)
        {
            code += gen_exec_select_once_code(func);
        }
        else if (func.params_.front().type_ == field::e_vector)
        {
            code += gen_exec_select_many_code(func);
        }

        return code;
    }
    std::string dao_generator::gen_func_implement(const mapper_function &func)
    {
        std::string code = gen_query_code(func);

        if (func.type_ == mapper_function::e_delete ||
            func.type_ == mapper_function::e_insert ||
            func.type_ == mapper_function::e_update)
        {
            code += gen_exec_update_code();
            return code;
        }
        else if (func.type_ == mapper_function::e_select)
        {
            code += gen_exec_select_code(func);
        }

        return code;
    }
    //implement
    std::string dao_generator::gen_class_implement(const mapper_t &m)
    {
        std::string code;
        std::string class_name = get_class_name(m.name_);

        code += class_name + "::" + class_name +
            "(acl::db_handle& handle)" + br;

        code += tab + ":db_handle_(handle)" + br;
        code += "{" + br;
        code += "}";

        code += br;
        code += br;

        std::vector<mapper_function>::const_iterator it = m.mfuncs_.begin();
        for (; it != m.mfuncs_.end(); ++it)
        {
            code += gen_annotation(*it, false);
            code += gen_func_impl_name(class_name, it->declare_);
            code += br;
            code += "{";
            code += br;
            code += gen_func_implement(*it);
            code += "}";
            code += br;
            code += br;

        }
        return code;
    }
    std::string dao_generator::gen_streq_code()const
    {
        return "static inline bool streq(const char *left, "
            "const char *right)" + br +
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

        use_streq_ = false;

        std::vector<mapper_t>::iterator it = mappers_.begin();
        for (; it != mappers_.end(); it++)
        {
            declare_code += gen_class_declare(*it);
            source_code += gen_class_implement(*it);
        }


        if (!declare_file.open_trunc(header_file_path.c_str()))
            printf("open file(%s) error,%s",
                   header_file_path.c_str(),
                   acl::last_serror());

        if (!source_file.open_trunc(source_file_path.c_str()))
            printf("open file(%s) error,%s",
                   source_file_path.c_str(),
                   acl::last_serror());


        if (declare_file.write(declare_code.c_str(),
                               declare_code.size()) == -1)
            printf("write file error,%s", acl::last_serror());


        std::string buffer;
        buffer += "#include \"acl_cpp/lib_acl.hpp\"" + br;
        for (size_t i = 0; i < model_files_.size();++i)
        {
            buffer += "#include \"" + model_files_[i]+ "\"" + br;
        }
        for (size_t i = 0; i < mapper_files_.size();++i)
        {
            buffer += "#include \"" + mapper_files_[i] + "\"" + br;
        }

        buffer += "#include \"dao.h\"" + br;
        buffer += br;

        if (use_streq_)
        {
            buffer += gen_streq_code();
        }

        if (source_file.write(buffer.c_str(), buffer.size()) == -1)
        {
            printf("write file error,%s", acl::last_serror());
            return;
        }

        if (source_file.write(source_code.c_str(), source_code.size()) == -1)
        {
            printf("write file error,%s", acl::last_serror());
            return;
        }
    }


    void dao_generator::gen_code_multi_files(const std::string &path)
    {
        std::string base_path(path);
        std::vector<std::string> dao_includes;
        if (base_path.size())
        {
            char ch = base_path[base_path.size() - 1];
            if (ch != '/' && ch != '\\')
            {
                base_path.push_back('/');
            }
        }

        for (size_t i = 0; i < mappers_.size(); ++i)
        {
            use_streq_ = false;
            mapper_t &m = mappers_[i];

            std::string header_code;
            std::string source_code;
            std::string header_file;
            std::string source_file;
            acl::ofstream file;

            std::string class_name = get_class_name(m.name_);
            header_file = base_path + class_name + ".h";
            source_file = base_path + class_name + ".cpp";

            header_code += "#pragma once" + br2;
            header_code += gen_class_declare(m);

            source_code += gen_class_implement(m);


            if (!file.open_trunc(header_file.c_str()))
            {
                printf("open file error%s\n", acl::last_serror());
                return;
            }
            if (file.write(header_code.c_str(), header_code.size()) == -1)
            {
                printf("write file error%s\n", acl::last_serror());
                file.close();
                return;
            }

            file.close();

            if (!file.open_trunc(source_file.c_str()))
            {
                printf("open file error%s\n", acl::last_serror());
                return;
            }
            std::string code;
            code += "#include \"acl_cpp/lib_acl.hpp\"" + br;
            for (std::set<std::string>::iterator it = m.model_files_.begin();
                 it != m.model_files_.end(); ++it)
            {
                code += "#include \"" + get_filename(*it) + "\"" + br;
            }
            code += "#include \"" + get_filename(m.mapper_path_) + "\"" + br2;
            code += "#include \"" + class_name + ".h\"" + br2;
            if (use_streq_)
                code += gen_streq_code();

            if (file.write(code.c_str(), code.size()) == -1)
            {
                printf("write file error%s\n", acl::last_serror());
                file.close();
                return;
            }
            if (file.write(source_code.c_str(), source_code.size()) == -1)
            {
                printf("write file error%s\n", acl::last_serror());
                file.close();
                return;
            }
            file.close();
            dao_includes.push_back(header_file);
        }
        acl::ofstream daos;
        std::string daos_filepath;
        std::string includes_codes;

        daos_filepath = base_path + "daos.h";
        if (!daos.open_trunc(daos_filepath.c_str()))
        {
            printf("create file(%s) error:%s",
                   daos_filepath.c_str(),
                   acl::last_serror());
            return;
        }

        includes_codes += "#pragma once" + br;

        for (size_t i = 0; i < dao_includes.size(); i++)
        {
            includes_codes += "#include \"" + get_filename(dao_includes[i]) + "\"" + br;
        }
        if (daos.write(includes_codes.c_str(), includes_codes.size()) == -1)
        {
            printf("write file error%s\n", acl::last_serror());
            daos.close();
            return;
        }
    }
    std::string dao_generator::get_type(const mapper_function &func)
    {
        if (func.type_ == mapper_function::e_insert)
            return "insert";
        else if (func.type_ == mapper_function::e_update)
            return "update";
        else if (func.type_ == mapper_function::e_delete)
            return "delete";
        return "select";
    }

    bool dao_generator::parse_path(const std::string &path)
    {
        std::vector<std::string> file_paths = list_dir(path, ".h");
        std::vector<std::string> mapper_files;
        std::vector<std::string> model_files;

        for (size_t i = 0; i < file_paths.size(); ++i)
        {
            std::string file_path = file_paths[i];
            lexer_ = new_lexer(file_path);
            if (!lexer_)
                continue;
            token t = get_next_token(true);
            if (t.type_ == token::e_$mappers)
            {
                mapper_files.push_back(file_path);
            }
            else if (t.type_ == token::e_$models)
            {
                model_files.push_back(file_path);
            }
            delete_lexer(lexer_);
        }
        model_files.insert(model_files.end(), 
                           mapper_files.begin(), mapper_files.end());

        for (size_t i = 0; i < model_files.size(); ++i)
        {
            if (!parse_file(model_files[i]))
            {
                printf("parse_file error %s\n", model_files[i].c_str());
                return false;
            }
        }
        return  true;
    }
    void dao_generator::delete_lexer(lexer_t *lexer)
    {
        if (lexer->file_)
            delete lexer->file_;
        delete lexer;
    }
    dao_generator::lexer_t
        *dao_generator::new_lexer(const std::string &file_path)
    {
        lexer_t *lexer = new lexer_t;
        lexer->file_ = new acl::ifstream;
        if (!lexer->file_->open_read(file_path.c_str()))
        {
            std::cout << "open file error" << acl::last_serror() << std::endl;
            delete lexer->file_;
            delete lexer;
            return NULL;
        }
        lexer->file_path_ = file_path;
        lexer->file_offset_ = 0;
        lexer->line_num_ = 0;
        return lexer;
    }
    std::vector<std::string> &dao_generator::cur_namespaces()
    {
        return analyzers_[0].namespaces_;
    }
    dao_generator::model_t & dao_generator::cur_model()
    {
        return analyzers_[0].model_;
    }
    dao_generator::mapper_t &dao_generator::cur_mapper()
    {
        return analyzers_[0].mapper_;
    }
}
