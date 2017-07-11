#include "common.hpp"
#include "model_generator.h"


static void usage(...)
{
    printf("usage: \n"
           "    -h               [help]\r\n"
           "    -m               [generate multifile,default generate single models.h]\r\n"
           "    -n [n1,n2,...]   [set namespaces.]\r\n"
           "    -s path          [path of sql,default current dir]\r\n"
           "    -d path          [generate models files to this path,default current dir]\r\n");
}


int main(int argc, char* argv[])
{
    std::vector<acl::string> tokens;

    std::string sql_path("./");
    std::string gen_path("./");
    std::string create_table_path("./");
    bool mutilfile = false;

    char ch;
    while ((ch = getopt(argc, argv, "pmhs:d:n:")) > 0)
    {
        switch (ch)
        {
            case 'h':
                usage(argv[0]);
                return 0;
            case 's':
                sql_path = optarg;
                break;
            case 'd':
                gen_path = optarg;
                break;
            case 'n':
            {
                acl::string buffer = optarg;
                tokens = buffer.split2(",");
                break;
            }
            case 'm':
                mutilfile = true;
                break;
            default:
                break;
        }
    }
    std::vector<std::string> namespaces;
    acl::model_generator gen;
    
    for (size_t i = 0; i < tokens.size(); i++)
    {
        namespaces.push_back(tokens[i].c_str());
    }

    gen.set_namespace(namespaces);

    if (gen.parse(sql_path))
    {
        if (mutilfile)
        {
            gen.gen_mappers(create_table_path);
            gen.gen_models(gen_path);
        }
        else
        {
            gen.gen_mapper(create_table_path);
            gen.gen_model(gen_path + "models.h");
        }
            
    }
    gen.gen_create_tables(create_table_path);
        
    return 0;
}