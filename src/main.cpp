#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "dao_generator.h"



static void usage(const char* procname)
{
    printf("usage: %s -h [help]\r\n"
           " -m [generate multifile,default generate single dao.h and dao.cpp]\r\n"
           " -s path [model and mapper files path,default current dir]\r\n"
           " -d path [generate dao files path,default current dir]\r\n", procname);
}


int main(int argc, char *argv[])
{
    char ch;
    std::string source_filepath("./");
    std::string dest_filepath("./");
    bool mutilfile = false;

    while ((ch = getopt(argc, argv, "mhs:d:")) > 0)
    {
        switch (ch)
        {
            case 'h':
            case 'H':
                usage(argv[0]);
                return 0;
            case 's':
            case 'S':
                source_filepath = optarg;
                break;
            case 'd':
            case 'D':
                dest_filepath = optarg;
                break;
            case 'm':
            case 'M':
                mutilfile = true;
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
    if (mutilfile)
    {
        gen.gen_code_mutil_files(dest_filepath);
    }
    else
    {
        gen.gen_code(dest_filepath);
    }
    printf("ok\n");
    return 0;
}