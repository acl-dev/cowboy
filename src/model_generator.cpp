#include <iostream>
#include "common.hpp"
#include "syntax_error.hpp"
#include "model_generator.h"

#define br  std::string("\r\n")
#define br2  std::string("\r\n\r\n")
#define tab std::string("\t")
#define tab2 std::string("\t\t")
#define tab3  std::string("\t\t\t")
#define tab4  std::string("\t\t\t\t")


namespace acl
{
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
            for (size_t i = 0; i < data.size(); ++i)
            {
                char ch = data[i];
                if (ch != end)
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
            data = next_line();
        } while (true);
    }

    std::string model_generator::next_token(const std::string &delimiters)
    {
        std::string buffer;
        skip(line_buffer_, " \t\r\n");

        while (line_buffer_.empty())
        {
            if (line_buffer_.empty())
                line_buffer_ = next_line();
            if (line_buffer_.empty())
                return buffer;
            skip(line_buffer_, " \t\r\n");
        }

        for (size_t i = 0; i < line_buffer_.size(); i++)
        {
            char ch = line_buffer_[i];
            if (delimiters.find(ch) == delimiters.npos)
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
    void model_generator::push_back_token(const token&t)
    {
        tokens_.push_back(t);
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
        const std::string delimiters = " \r\n\t,()=`;-/";

        std::string str = next_token(delimiters);

        t.str_ = str;
        str = lower_case(str);

        if (str.empty())
        {
            t.type_ = token::e_eof;
        }
        else if (str == "-")
        {
            if (look_ahead() == '-')
            {
                next_token(delimiters);
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
                next_token(delimiters);
                t.type_ = token::e_comment_begin;
                t.str_ = "/*";
            }
            else
            {
                t.type_ = token::e_sub;
            }
        }
        else if (str == "*")
        {
            if (look_ahead() == '/')
            {
                next_token(delimiters);
                t.type_ = token::e_comment_end;
                t.str_ = "*/";
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
        else if (str == "auto_increment")
        {
            t.type_ = token::e_auto_increment;
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
            t.type_ = token::e_sql_comment;
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
        else if (str == "unsigned")
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
        t.line_ = lines_;
        current_token_ = t;

        return t;
    }
    model_generator::token
        model_generator::current_token()
    {
        return current_token_;
    }
    model_generator::field::type
        model_generator::to_field_type(const token& t)
    {
        switch (t.type_)
        {
            case token::e_tinyint:
                return field::e_char;
            case token::e_mediumint:
            case token::e_int:
            case token::e_integer:
                return field::e_int;
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
    std::string model_generator::get_string()
    {
        std::string buffer;
        token t = get_next_token();
        if (t.type_ == token::e_quote)
        {
            buffer = next_token("'");
        }
        else if (t.type_ == token::e_double_quote)
        {
            buffer = next_token("\"");
        }
        else if (t.type_ == token::e_now)
        {
            if (get_next_token().type_ == token::e_open_paren &&
                get_next_token().type_ == token::e_close_paren)
                buffer = t.str_ + "()";
            else
            {

            }
        }
        return buffer;
    }
    std::string model_generator::get_name()
    {
        token t = get_next_token();

        if (t.type_ == token::e_backtick)
        {
            token t1 = get_next_token();
            token t2 = get_next_token();
            if (t2.type_ != token::e_backtick)
                throw syntax_error();

            return t1.str_;
        }
        return t.str_;
    }

    model_generator::field model_generator::parse_field()
    {
        field f;

        f.name_ = get_name();
        f.type_ = to_field_type(get_next_token());

        token t = get_next_token();
        if (t.type_ == token::e_unsigned)
        {
            f.unsigned_ = true;
            t = get_next_token();
        }

        //eg: varchar(255) or double(10,10)
        if (t.type_ == token::e_open_paren)
        {
            t = get_next_token();
            f.type_len_.append(t.str_);
            while (true)
            {
                t = get_next_token();
                if (t.type_ == token::e_close_paren)
                {
                    t = get_next_token();
                    break;
                }
                else
                    f.type_len_.append(t.str_);
            }
        }

        while (t.type_ != token::e_comma)
        {
            if (t.type_ == token::e_primary)
            {
                t = get_next_token();
                if (t.type_ == token::e_key)
                {
                    f.primary_key_ = true;
                }

            }
            else if (t.type_ == token::e_auto_increment)
            {
                f.auto_increase_ = true;
            }
            else if (t.type_ == token::e_not)
            {
                t = get_next_token();
                if (t.type_ == token::e_null)
                {
                    f.not_null_ = true;
                }
                else
                {
                    throw syntax_error("unknown token " + t.str_);
                }
            }
            else if (t.type_ == token::e_default)
            {
                f.default_ = get_string();
            }
            else if (t.type_ == token::e_unique)
            {
                f.unique_ = true;
            }
            else if (t.type_ == token::e_sql_comment)
            {
                f.comment_ = get_string();
            }
            else
            {
                std::cout << t.str_ << std::endl;
            }
            t = get_next_token();
        }

        return f;
    }
    void model_generator::parse_table()
    {
        table_.name_ = get_name();

        token t = get_next_token();
        if (t.type_ != token::e_open_paren)
            throw syntax_error();

        t = get_next_token();

        while (t.type_ != token::e_semicolon)
        {
            if (t.type_ == token::e_backtick ||
                t.type_ == token::e_identifier)
            {
                push_back_token(t);
                field f = parse_field();
                table_.fields_.push_back(f);
            }
            else if (t.type_ == token::e_primary)
            {
                t = get_next_token();
                if (t.type_ != token::e_key)
                    throw syntax_error("unknown token " + t.str_);
                t = get_next_token();
                if (t.type_ != token::e_open_paren)
                    throw syntax_error("unknown token " + t.str_);
                table_.primary_key_ = get_name();
                t = get_next_token();
                if (t.type_ != token::e_close_paren)
                    throw syntax_error("unknown token " + t.str_);
            }
            else if (t.type_ == token::e_engine)
            {
                t = get_next_token();
                if (t.type_ == token::e_eq)
                {
                    table_.engine_ = get_next_token().str_;
                }
            }
            else if (t.type_ == token::e_auto_increment)
            {
                get_next_token();
                table_.auto_increment_ = get_next_token().str_;
            }
            else if (t.type_ == token::e_default)
            {
                t = get_next_token();
                if (t.type_ == token::e_charset)
                {
                    get_next_token();
                    table_.charset_ = get_next_token().str_;
                }
            }
            t = get_next_token();
        }

    }
    void model_generator::parse()
    {
        do
        {
            token t = get_next_token();
            if (t.type_ == token::e_double_sub)
            {
                line_buffer_.clear();
            }
            else if (t.type_ == token::e_comment_begin)
            {
                while (t.type_ != token::e_comment_end)
                    t = get_next_token();
            }
            else if (t.type_ == token::e_create)
            {
                t = get_next_token();
                if (t.type_ == token::e_table)
                {
                    table_ = table();
                    table_.sql_ = get_string(';');
                    table_.sql_.push_back(';');

                    parse_table();
                    tables_.push_back(table_);
                }
            }
            else if (t.type_ == token::e_eof)
            {
                return;
            }
        } while (true);
    }
    void model_generator::set_namespace(const std::vector<std::string> &namespaces)
    {
        namespaces_ = namespaces;
    }
    bool model_generator::parse(const std::string &path)
    {
        std::vector<std::string> files = list_dir(path, ".sql");

        for (size_t i = 0; i < files.size(); i++)
        {
            if (file_.open_read(files[i].c_str()))
            {
                try
                {
                    parse();
                }
                catch (const std::exception& e)
                {
                    std::cout << "parse error:" << e.what() << std::endl;
                    std::cout << "line:" << lines_ << std::endl;
                    std::cout << current_line_ << "  >>" << line_buffer_ << std::endl;
                    return false;
                }
            }
        }
        return true;
    }
    std::string model_generator::get_type(field &f)
    {
        std::string code;
        if (f.unsigned_)
            code += "unsigned ";

        switch (f.type_)
        {
            case field::e_char:
                code += "char";
                break;
            case field::e_short:
                code += "short";
                break;
            case field::e_int:
                code += "int";
                break;
            case field::e_long_long_int:
                code += "long long int";
                break;
            case field::e_float:
                code += "float";
                break;
            case field::e_double:
                code += "double";
                break;
            case field::e_std_string:
                code = "std::string";
                break;
            default:
                throw syntax_error("unknown type " + f.type_str_);
        }
        return code;
    }
    std::string model_generator::gen_field(field &f)
    {
        std::string code;

        code += tab + "//@Column{" + f.name_ + "}" + br;
        code += tab + get_type(f) + " " + f.name_ + ";";

        return code;
    }
    std::string model_generator::gen_construct(table &t)
    {
        std::string code;

        code += tab + t.name_ + "()";
        bool first = false;

        for (size_t i = 0; i < t.fields_.size(); i++)
        {
            field &f = t.fields_[i];
            if (f.type_ != field::e_std_string)
            {
                if (!first)
                {
                    code += br + tab + " :" + f.name_ + "(0),";
                    first = true;
                }
                else
                {
                    code += br + tab + " " + f.name_ + "(0),";
                }
            }
        }
        if (code[code.size() - 1] == ',')
        {
            code.erase(code.end()-1);
        }
            

        code += br + tab + "{" + br;

        code += tab + "}" + br;

        return code;
    }
    std::string model_generator::gen_model(table &t)
    {
        std::string code;

        code += "//@Model" + br;
        code += "//@Table{" + t.name_ + "}" + br;
        code += "struct " + t.name_ + br;
        code += "{" + br;

        code += gen_construct(t);
        code += br;

        for (size_t i = 0; i < t.fields_.size(); i++)
        {
            code += gen_field(t.fields_[i]);
            if (i < t.fields_.size())
                code += br2;
        }
        code += "};";
        return code;
    }

    model_generator::field
        model_generator::get_primary_key(const table &t)
    {
        for (size_t i = 0; i < t.fields_.size(); i++)
        {
            if (t.primary_key_ == t.fields_[i].name_)
                return t.fields_[i];
        }
        return model_generator::field();
    }
    std::string model_generator::gen_mapper(const table &t)
    {
        std::string code;
        std::string pkey = t.primary_key_;
        std::string name = t.name_;
        std::string pkey_type;

        field primary_key = get_primary_key(t);
        pkey_type = get_type(primary_key);
        if (pkey_type == "std::string")
            pkey_type = "const std::string &";
        else
        {
            pkey_type += " ";
        }

        code += "//@Mapper" + br;
        code += "struct " + name +"_mapper"+ br;
        code += "{" + br;

        std::string update;
        std::string insert;
        std::string insert_value;
        std::string $delete;
        std::string select;
        std::string select_all;

        update += tab + "//@Update{update " + name + " set ";
        insert += tab + "//@Insert{insert into " + name + "(";
        insert_value += "value(";
        $delete += tab + "//@Delete{delete from " + name 
            + " where " + pkey + "=:" + pkey + ")}" + br;

        select += tab + "//@Select{select * from " + 
            name + " where " + pkey + "=:" + pkey + ")}" + br;

        select_all += tab + "//@Select{select * from " + name + ")}" + br;
        for (size_t i = 0; i < t.fields_.size(); i++)
        {
            const field &f = t.fields_[i];
            if (!f.auto_increase_)
            {
                update += f.name_ + "=:" + f.name_;
                insert += f.name_;
                insert_value += ":" + f.name_;
                if (i < t.fields_.size()-1)
                {
                    update.push_back(',');
                    insert.push_back(',');
                    insert_value.push_back(',');
                }
            }
        }
        insert_value += ")}";

        insert += ")";
        insert += insert_value + br;
        insert += tab + "virtual bool insert(const " + 
            name + " &obj)=0;" + br;

        select += tab + "virtual bool select(" + name + 
            " &obj, " + pkey_type + primary_key.name_ + ")=0;" + br;

        select_all += tab + "virtual bool select_all(std::list<" + 
            name + "> &objs)=0;" + br;

        $delete += tab + "virtual bool del(" + pkey_type + 
            primary_key.name_ + ")=0;" + br;

        update += " where " + pkey + " = :" + pkey + " }" + br;
        update += tab + "virtual bool update(const " + name + " &obj)=0;" + br;

        code += insert + br;
        code += update + br;
        code += $delete + br;
        code += select + br;
        code += select_all + br;
        code += "};" + br;
        return code;
    }
    static inline std::string 
        namespace_begin(const std::vector<std::string> &namespaces)
    {
        std::string code;

        for (size_t i = 0; i < namespaces.size(); i++)
        {
            std::string ns = namespaces[i];
            code += "namespace " + ns + br;
            code += "{" + br;
        }
        return code;
    }

    static inline std::string 
        namespace_end(const std::vector<std::string> &namespaces)
    {
        std::string code;
        for (int i = (int)namespaces.size() - 1; i >= 0; i--)
        {
            code += "}//end of " + namespaces[i] + br;
        }
        return code;
    }

    void model_generator::gen_model(const std::string &file_path)
    {
        std::string code;

        code += "//@Models" + br;
        code += "#pragma once" + br;

        code += namespace_begin(namespaces_);

        for (size_t i = 0; i < tables_.size(); i++)
        {
            code += gen_model(tables_[i]);
            code += br2;
        }

        code += namespace_end(namespaces_);

        acl::ofstream file;
        if (!file.open_trunc(file_path.c_str()))
            printf("open file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());

        if (file.write(code.c_str(), code.size()) == -1)
            printf("write file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());
    }
    void model_generator::gen_models(const std::string &path)
    {
        std::string base_path(path);

        if (base_path[base_path.size() - 1] != '/' &&
            base_path[base_path.size() - 1] != '\\')
            base_path.push_back('/');

        for (size_t i = 0; i < tables_.size(); i++)
        {
            std::string file_path;
            std::string code;
            code += "//@Models" + br;
            code += "#pragma once" + br;

            file_path = base_path + tables_[i].name_ + ".h";

            code += namespace_begin(namespaces_);
            code += gen_model(tables_[i]);
            code += br;
            code += namespace_end(namespaces_);
            acl::ofstream file;
            if (!file.open_trunc(file_path.c_str()))
                printf("open file(%s) error,%s",
                       file_path.c_str(),
                       acl::last_serror());

            if (file.write(code.c_str(), code.size()) == -1)
                printf("write file(%s) error,%s",
                       file_path.c_str(),
                       acl::last_serror());
        }
    }
    void model_generator::gen_create_tables(const std::string &path)
    {
        std::string base_path(path);

        if (base_path[base_path.size() - 1] != '/' &&
            base_path[base_path.size() - 1] != '\\')
            base_path.push_back('/');

        std::string file_path;
        std::string code;
        code += "#pragma once" + br;

        file_path = base_path + "create_tables.h";

        code += namespace_begin(namespaces_);

        for (size_t i = 0; i < tables_.size(); i++)
        {
            table &t = tables_[i];

            code += "inline bool create_table(acl::handle_db &handle, const " + t.name_ + "&)" + br;
            code += "{" + br;
            code += tab + "const char *sql = " + to_cstring(t.sql_, std::string(5, '\t')) + br;
            code += tab + "if (handle.tbl_exists(\"" + t.name_ + "\"))" + br;
            code += tab + "{" + br;
            code += tab2 + "logger(\"table " + t.name_ + " exist\");" + br;
            code += tab2 + "return true;" + br;
            code += tab + "}" + br;
            code += tab + "else if(handle.sql_update(sql) == false)" + br;
            code += tab + "{" + br;
            code += tab2 + "logger_error(\"create table " + t.name_ + " error. sql:%s\",sql);" + br;
            code += tab2 + "return false;" + br;
            code += tab + "}" + br;
            code += tab + "logger(\"create table " + t.name_ + " ok\");" + br;
            code += tab + "return true" + br;
            code += "}" + br;

        }

        code += namespace_end(namespaces_);

        acl::ofstream file;
        if (!file.open_trunc(file_path.c_str()))
            printf("open file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());

        if (file.write(code.c_str(), code.size()) == -1)
            printf("write file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());
    }
    void model_generator::gen_mapper(const std::string &path)
    {
        std::string base_path(path);

        if (base_path[base_path.size() - 1] != '/' &&
            base_path[base_path.size() - 1] != '\\')
            base_path.push_back('/');


        std::string file_path;
        std::string code;
        code += "//@Mappers" + br;
        code += "#pragma once" + br2;

        file_path = base_path + "mappers.h";

        code += namespace_begin(namespaces_);

        for (size_t i = 0; i < tables_.size(); i++)
        {
            table &t = tables_[i];
            code += gen_mapper(t) + br;
        }
        code += namespace_end(namespaces_);

        acl::ofstream file;
        if (!file.open_trunc(file_path.c_str()))
            printf("open file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());

        if (file.write(code.c_str(), code.size()) == -1)
            printf("write file(%s) error,%s",
                   file_path.c_str(),
                   acl::last_serror());
    }
    void model_generator::gen_mappers(const std::string &path)
    {
        std::string base_path(path);

        if (base_path[base_path.size() - 1] != '/' &&
            base_path[base_path.size() - 1] != '\\')
            base_path.push_back('/');

        for (size_t i = 0; i < tables_.size(); i++)
        {
            table &t = tables_[i];
            std::string file_path;
            std::string code;
            code += "//@Mappers" + br;
            code += "#pragma once" + br2;

            file_path = base_path + t.name_ + "_mapper.h";

            code += namespace_begin(namespaces_);
            code += gen_mapper(t) + br;
            code += namespace_end(namespaces_);

            acl::ofstream file;
            if (!file.open_trunc(file_path.c_str()))
                printf("open file(%s) error,%s",
                       file_path.c_str(),
                       acl::last_serror());

            if (file.write(code.c_str(), code.size()) == -1)
                printf("write file(%s) error,%s",
                       file_path.c_str(),
                       acl::last_serror());
        }

    }
}