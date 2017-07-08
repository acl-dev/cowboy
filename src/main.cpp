#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "dao_generator.h"



static void usage(...)
{
    printf("usage: \n"
           "    -h          [help]\r\n"
           "    -m          [generate multifile,default generate single dao.h and dao.cpp]\r\n"
           "    -p          [print parse result status]\r\n"
           "    -s path     [path of mapper,model files,default current dir]\r\n"
           "    -d path     [generate dao files to the path,default current dir]\r\n");
}


int main(int argc, char *argv[])
{
    char ch;
    std::string source_filepath("./");
    std::string dest_filepath("./");
    bool mutilfile = false;
    bool print = false;

    while ((ch = getopt(argc, argv, "pmhs:d:")) > 0)
    {
        switch (ch)
        {
            case 'h':
                usage(argv[0]);
                return 0;
            case 's':
                source_filepath = optarg;
                break;
            case 'd':
                dest_filepath = optarg;
                break;
            case 'm':
                mutilfile = true;
                break;
            case 'p':
                print = true;
                break;
            default:
                break;
        }
    }
    acl::dao_generator gen;

    if (!gen.parse_path(source_filepath))
    {
        printf("parse_file error,exit()\n");
        return -1;
    }
    if (print)
    {
        gen.print_entries();
    }
    try
    {
        if (mutilfile)
        {
            gen.gen_code_mutil_files(dest_filepath);
        }
        else
        {
            gen.gen_code(dest_filepath);
        }
    }
    catch (const std::exception&e)
    {
        printf("gen code error:%s",e.what());
        return -1;
    }
    
    
    if (print)
    {
        gen.print_mappers();
    }


    printf("ok\n");
    return 0;
}