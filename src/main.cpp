#include "acl_cpp/lib_acl.hpp"
#include "lib_acl.h"
#include "dao_generator.h"
#include "model_generator.h"


static void usage(...)
{
    printf("usage: \n"
           "    -h                        'help'\r\n"
           "    -o                        'parse mapper + model generate dao'\r\n"
           "    -u                        'parse sql files generate model + mapper file'\r\n"
           "    -m                        'generate multifile,file per table,default generate single'\r\n"
           "    -x                        'generate dao to 'dao',\r\n"
           "                              'generate mapper to 'mapper',\r\n"
           "                              'generate model to 'model',\r\n"
           "                              'generate create_tables.hpp to create_tables '\r\n"
           "    -s scan_path              'path to scan,default current dir'\r\n"
           "    -d dao_path               'generate dao path,default current dir'\r\n"
           "    -c mapper_path            'generate mapper file path,default current dir'\r\n"
           "    -k model_path             'generate model file path,default current dir'\r\n"
           "    -j create_tables_path     'generate create_tables.hpp file path,default current dir'\r\n\r\n\r\n"
           "about:\r\n"
           "     scan_path:the path cowboy to scan.it will scan files with *.sql when with -u\r\n"
           "               and scan *.h files with -o \r\n\r\n"
           "eg:\r\n"
           "./cow_boy -u -o\r\n"
           "./cow_boy -u -o -x\r\n"
           "./cow_boy -u -o -x -m\r\n"
           "./cow_boy -u -o -d daos -c mappers -k models -j create_table\r\n\r\n");
}


int main(int argc, char *argv[])
{
    char ch;
    std::string source_filepath("./");
    std::string dao_path("./");
    std::string mapper_path("./");
    std::string model_path("./");
    std::string create_table_path("./");

    bool mutilfile = false;
    bool print = false;
    bool o = false;
    bool u = false;
    bool x = false;


    while ((ch = getopt(argc, argv, "pmouhxs:d:c:k:j:")) > 0)
    {
        switch (ch)
        {
            case 'c':
                mapper_path = optarg;
                acl_make_dirs(optarg, 0755);
                break;
            case 'k':
                model_path = optarg;
                acl_make_dirs(optarg, 0755);
                break;
            case 'j':
                create_table_path = optarg;
                acl_make_dirs(optarg, 0755);
                break;
            case 'o':
                o = true;
                break;
            case 'u':
                u = true;
                break;
            case 'x':
                x = true;
                break;
            case 's':
                source_filepath = optarg;
                break;
            case 'd':
                dao_path = optarg;
                acl_make_dirs(optarg, 0755);
                break;
            case 'm':
                mutilfile = true;
                break;
            case 'p':
                print = true;
                break;
            case 'h':
                usage(argv[0]);
                return 0;
            default:
                break;
        }
    }
    if (x)
    {
        if (dao_path == ("./"))
        {
            dao_path = "dao";
            acl_make_dirs(dao_path.c_str(), 0755);
        }
        if (mapper_path == ("./"))
        {
            mapper_path = "mapper";
            acl_make_dirs(mapper_path.c_str(), 0755);
        }
        if (model_path == ("./"))
        {
            model_path = "model";
            acl_make_dirs(model_path.c_str(), 0755);
        }
        if (create_table_path == ("./"))
        {
            create_table_path = "create_table";
            acl_make_dirs(create_table_path.c_str(), 0755);
        }
    }

    if (u)
    {
        acl::model_generator gen;

        if (gen.parse(source_filepath))
        {
            if (mutilfile)
            {
                gen.gen_mappers(mapper_path);
                gen.gen_models(model_path);
            }
            else
            {
                gen.gen_mapper(mapper_path);
                gen.gen_model(model_path);
            }

        }
        gen.gen_create_tables(create_table_path);
    }
    if (o)
    {
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

        if (mutilfile)
        {
            gen.gen_code_multi_files(dao_path);
        }
        else
        {
            gen.gen_code(dao_path);
        }
        try
        {
        }
        catch (const std::exception&e)
        {
            printf("gen code error:%s", e.what());
            return -1;
        }

        if (print)
        {
            gen.print_mappers();
        }
    }
    return 0;
}
