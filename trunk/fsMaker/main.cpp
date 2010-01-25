#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>

using namespace std;

int conta(string s)
{
    int n=0;
    for(int c=0;c<s.length();c++)
        if(s.c_str()[c]=='/')
            n++;
    return n;
}


int getdir (string dir)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL)
    {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }
   
    
    string space="";
    for(int c=0;c<conta(string(dir));c++)
        space=space+"    ";
    
    while ((dirp = readdir(dp)) != NULL)
    {
        if(dirp->d_type!=DT_DIR)
        {
            cout<< space <<string(dirp->d_name)<<endl;   
        }
        else
        {
            if(string(dirp->d_name)!="." && string(dirp->d_name)!="..")
            {
                cout<<space<<"["<<string(dirp->d_name)<<"]"<<endl;
                getdir(dir+"/"+string(dirp->d_name));
            }
        }
    }
    closedir(dp);
    
    return 0;
}

int main()
{
    string dir = string(".");

    getdir(dir);

    return 0;
}
