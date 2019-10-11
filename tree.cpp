#include <iostream>
#include <string>
#include <sys/stat.h>
#include <dirent.h>
#include <cstring>
#include <set>

using namespace std;

bool flags[100];
void help(); // 帮助信息
void printTree(string path, int level = 0); // 打印目录
bool isDir(string path); // 是否是目录

int main(int argc, char** argv)
{
    if(argc < 2)
    {
        help();
        return 0;
    }
    
    string path = argv[1];
    cout << "." << endl;
    printTree(path);

    return 0;
}

void help()
{
    cout << "tree path" << endl;
}

void printTree(string path, int level)
{
    if(!isDir(path))
    {
        cerr << "path is not a directory" << endl;
        exit(1);
    }

    DIR* dp;
    if(nullptr == (dp = opendir(path.c_str())))
    {
        cerr << "open directory failed" << endl;
        exit(1);
    }
    
    dirent* dirp;
    set<string> files;
    
    while(nullptr != (dirp = readdir(dp)))
    {
        // 过滤 . 和 .. 目录
        if(!strcmp(dirp->d_name, ".") || !strcmp(dirp->d_name, ".."))
        {
            continue;
        }
        
        files.insert(dirp->d_name);
    }

    for(set<string>::iterator it = files.begin(); it != files.end(); ++it)
    {
        set<string>::iterator temp = it;
        ++temp;
        for(int i = 0; i < level; ++i)
        {
            if(flags[i])
            {
                cout << "    ";
            }
            else
            {
                cout << "│   ";
            }
        }
        if(temp == files.end())
        {
            cout << "└── " << *it << endl;
            flags[level] = true;
        }
        else
        {
            cout << "├── " << *it << endl;
            flags[level] = false;
        }

        if(isDir(path + "/" + *it))
        {
            printTree(path + "/" + *it, level + 1);
        }
    }
}

bool isDir(string path)
{
    struct stat s_buf;
    stat(path.c_str(), &s_buf);

    if(S_ISDIR(s_buf.st_mode))
    {
        return true;
    }

    return false;
}
