#pragma once
#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include <string>
#include <vector>
namespace acl
{
    inline std::string get_filename(const std::string &file_path)
    {
        size_t nps = file_path.find_last_of('/');
        if (nps == file_path.npos)
        {
            nps = file_path.find_last_of('\\');
        }
        if (nps != file_path.npos)
            return file_path.substr(nps + 1);
        return file_path;
    }

    inline std::string skip_all(const std::string &str,
                                const std::string &skip_str)
    {
        std::string buffer;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char ch = str[i];
            if (skip_str.find(ch) == std::string::npos)
            {
                buffer.push_back(ch);
            }
        }
        return buffer;
    }
    inline void skip(std::string &line,
                     const std::string &delims)
    {
        size_t offset = 0;

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
    inline std::string replace(const std::string &str,
                               char from,
                               char to)
    {
        std::string buffer;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char ch = str[i];
            if (ch == from)
            {
                buffer.push_back(to);
            }
            else
            {
                buffer.push_back(ch);
            }
        }
        return buffer;
    }
    inline std::string skip_multi_space(const std::string &str)
    {
        std::string buffer;
        for (size_t i = 0; i < str.size(); ++i)
        {
            char ch = str[i];
            if (ch == ' ')
            {
                buffer.push_back(' ');
                for (; i < str.size(); ++i)
                {
                    ch = str[i];
                    if (ch != ' ')
                        break;
                }
            }

            buffer.push_back(ch);
        }
        return buffer;
    }
    inline std::string lower_case(const std::string &str)
    {
        std::string buffer;
        for (size_t i = 0; i < str.size(); i++)
        {
            char ch = str[i];
            if (ch >= 'A' && ch <= 'Z')
            {
                buffer.push_back(ch + ('a' - 'A'));
            }
            else
            {
                buffer.push_back(ch);
            }
        }
        return buffer;
    }

    inline std::vector<std::string>
        list_dir(const std::string &path, const std::string &ext_)
    {
        std::vector<std::string> files;
        const char* file_path = NULL;

        acl::scan_dir scan;
        if (!scan.open(path.c_str(), true))
        {
            printf("scan open error %s\n",
                   acl::last_serror());
            return files;
        }
        while ((file_path = scan.next_file(true)))
        {
            if (ext_.size())
            {
                if (acl_strrncasecmp(file_path,
                                     ext_.c_str(),
                                     ext_.size()) == 0)
                {
                    files.push_back(file_path);
                }
            }
            else
            {
                files.push_back(file_path);
            }
        }
        return files;
    }
    inline std::string namespace_to_string(const std::vector<std::string> &nss)
    {
        std::string buffer;
        for (size_t i = 0; i < nss.size(); ++i)
        {
            buffer.append(nss[i]);
            buffer.append("::");
        }
        return buffer;
    }
    inline std::vector<std::string> get_lines(const std::string &str)
    {
        std::vector<std::string> vec;
        std::string buffer;

        for (size_t i = 0; i < str.size(); i++)
        {
            char ch = str[i];
            if (ch == '\n' || ch == '\r')
            {
                skip(buffer, " \t");
                if (buffer.size())
                    vec.push_back(buffer);
                buffer.clear();
            }
            else
            {
                buffer.push_back(ch);
            }
        }
        skip(buffer, " \t");
        if (buffer.size())
            vec.push_back(buffer);
        
        return vec;
    }
    inline std::string to_cstring(const std::string &str,const std::string &tabs)
    {
        std::string buffer;
        std::vector<std::string> vec = get_lines(str);

        if (vec.empty())
            return "\"\"";
        buffer += "\"" + vec[0] + "\"\r\n";

        for (size_t i = 1; i < vec.size(); i++)
        {
            buffer += tabs + "\"" + vec[i] + "\"\r\n";
        }
        return buffer;
    }
}