#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <iostream>
#include <sys/utsname.h>
#include <vector>
#include <iomanip>
#include <fstream>
#include <vector>
#include <ctime>
#include <sys/stat.h>
#include <dirent.h>
#include <filesystem>
#include <pwd.h>
#include <cstring>
#include <grp.h>
#include <unistd.h>
#define clear() printf("\033[H\033[J")

using namespace std;
#define PATH_MAX = 500;
string permission = "rwxrwxrwx";

string solve_pwd()
{
    char arr[2000];
    string sstr = "";
    char *ptr = getcwd(arr, sizeof(arr));
    if (ptr != NULL)
    {
        // cout<< arr<<endl;
        sstr = arr;
    }
    else
    {
        cout << "Error.." << endl;
    }
    // cout<<sstr.size()<<endl;
    return sstr;
}
void solve_echo(vector<string> &str)
{
    for (int i = 1; i < str.size(); i++)
    {
        cout << str[i] << " ";
    }
    cout << endl;
}
void solve_cd(vector<string> &str, string prev)
{
    const char *next_comm = str[1].c_str();
    if (next_comm == nullptr)
    {

        // int status=chdir(home);
        // if(status!=0){
        //  cout<<"Error in reverting back to home directory"<<endl;
        // }
    }
    else if (!strcmp(next_comm, ".."))
    {
        if (!prev.empty())
        {
            chdir(prev.c_str());
        }
        else
        {
            cout << "Error in revert back to previous directory" << endl;
        }
    }
    else if (!strcmp(next_comm, "~"))
    {
        const char *new_dir = getenv("HOME");
        if (new_dir != NULL)
        {
            prev = getcwd(nullptr, 0);
            chdir(new_dir);
        }
        else
        {
            cout << "Error in redirecting to home directory" << endl;
        }
    }

    else
    {
        const char *new_dir = str[1].c_str();
        int s1 = chdir(new_dir);
        prev = getcwd(nullptr, 0);
        if (s1 != 0)
        {

            cout << "Error changin directory." << endl;
        }
        else
        {
            // cout<<"here"<<endl;
            cout << "Usage: cd<directoru>" << endl;
        }
    }
}
void solve_ls(vector<string> &str)
{

    const char *next_command = str[1].c_str();
    const char *up_next_command = str[2].c_str();
    /*---------------------------------------------LS---------------------------------------*/
    if (next_command == nullptr || strcmp(next_command, ".") == 0)
    {
        DIR *dir;
        string directorypath = solve_pwd();
        string directory_path = directorypath.c_str();
        struct dirent *fileentry;
        if ((dir = opendir(directorypath.c_str())) == NULL)
        {
            cout << "Error in reading directory" << endl;
            return;
        }
        while ((fileentry = readdir(dir)) != nullptr)
        {
            if (fileentry->d_name[0] != '.')
            {
                cout << fileentry->d_name << endl;
            }
        }

        // cout<<"only ls"<<endl;
        // return ;
    }
    /*----------------------------------------------ls -l----------------------------------------------*/

    else if (strcmp(next_command, "-l") == 0)
    {

        DIR *dir;

        // string file_ans="";
        string directorypath = solve_pwd();
        string directory_path = directorypath.c_str();

        struct dirent *fileentry;
        if ((dir = opendir(directorypath.c_str())) == NULL)
        {
            cout << "Error in opening directory" << endl;
            return;
        }

        // cout<<directory_path<<endl;
        struct stat filestat;
        off_t num_block = filestat.st_blocks;
        // cout<<num_block<<endl;

        while ((fileentry = readdir(dir)) != nullptr)
        {
            //  struct stat filestat;
            if (fileentry->d_name != ".")
            {
                string file_ans = " ";
                string file_path = directory_path + "/" + fileentry->d_name;
                if (lstat(file_path.c_str(), &filestat) == -1)
                {
                    continue;
                }
                // cout<<fileentry->d_name<<endl;

                // cout<<file_path<<endl;
                struct passwd *user = getpwuid(filestat.st_uid);
                struct group *grp = getgrgid(filestat.st_gid);
                int filesize = filestat.st_size;
                char time[200];
                strftime(time, sizeof(time), "%b %d  %H:%M", localtime(&filestat.st_mtime));
                // cout<<time<<endl;
                // cout<<filesize<<endl;
                // cout<<user->pw_name<<endl;

                // file type and permission
                string temp = "";
                if (S_ISREG(filestat.st_mode))
                {
                    temp = "-";
                }
                else if (S_ISDIR(filestat.st_mode))
                {
                    temp = "d";
                }
                file_ans += temp;
                // permission
                string per = permission;
                for (int i = 0; i < 9; i++)
                {
                    if (!(filestat.st_mode & (1 << (8 - i))))
                    {
                        per[i] = '-';
                    }
                }
                file_ans += per;
                file_ans += " ";
                file_ans += "1";
                file_ans += " ";
                file_ans += user->pw_name;
                file_ans += " ";
                file_ans += grp->gr_name;
                file_ans += " ";
                file_ans += to_string(filesize);
                file_ans += " ";
                file_ans += time;
                file_ans += " ";
                file_ans += fileentry->d_name;
                // cout<<fileentry->d_name<<endl;
                if (fileentry->d_name[0] != '.')
                {

                    cout << file_ans << endl;
                }
            }
            // cout<<file_ans<<endl;
        }
    }
    /*----------------------------------ls ~--------------------------------------------*/

    /*-----------------------------------ls -a-------------------------------------------*/
    else if (strcmp(next_command, "-a") == 0)
    {
        const char *next_command1 = str[2].c_str();
        /* cout<<"Here"<<endl;
            if(strcmp(next_command1,"-l")==0){
             cout<<"Here2"<<endl;
                return ;

            }*/
        cout << "here" << endl;
        if (next_command1 == nullptr)
        {
            DIR *dir;
            string directorypath = solve_pwd();
            string directory_path = directorypath.c_str();
            struct dirent *fileentry;
            if ((dir = opendir(directorypath.c_str())) == NULL)
            {
                cout << "Error in reading directory" << endl;
                return;
            }
            while ((fileentry = readdir(dir)) != nullptr)
            {

                cout << fileentry->d_name << endl;
            }
        }
        else
        {
            cout << "here2" << endl;
        }
    }
    /*---------------------------------------------ls .--------------------------------------------------*/

    /*-------------------------------------------ls .. ---------------------------------------------------*/
    else if ((!strcmp(next_command, "..")) || (!strcmp(next_command, "~")))
    {
        string current_dir = solve_pwd();
        cout << current_dir << endl;
        string parent_dir = "";
        int size = current_dir.size();
        int last = size;
        while (current_dir[last] != '/')
        {
            last--;
        }

        parent_dir = current_dir.substr(0, last);
        //  cout<<parent_dir<<endl;

        DIR *dir;
        string directorypath = parent_dir;
        string directory_path = directorypath.c_str();
        struct dirent *fileentry;
        if ((dir = opendir(directorypath.c_str())) == NULL)
        {
            cout << "Error in reading directory" << endl;
            return;
        }
        while ((fileentry = readdir(dir)) != nullptr)
        {
            if (fileentry->d_name[0] != '.')
            {
                cout << fileentry->d_name << endl;
            }
        }
    }
    /*------------------------------------------------ls -al ----------------------------------------------*/
    /*-----------------------------------------------ls -la---------------------------------------------------*/
    else if (!strcmp(next_command, "-al") || (!strcmp(next_command, "-la")) || ((!strcmp(next_command, "-a")) && (!strcmp(up_next_command, "-l"))))
    {
        // list directory content
        string current_dir = solve_pwd();
        // cout<<current_dir<<endl;
        string parent_dir = "";
        int size = current_dir.size();
        int last = size;
        while (current_dir[last] != '/')
        {
            last--;
        }
        parent_dir = current_dir.substr(0, last);
        // cout<<parent_dir<<endl;
        DIR *dir;
        // string file_ans="";
        string directorypath = parent_dir;
        string directory_path = directorypath.c_str();
        struct dirent *fileentry;
        if ((dir = opendir(directorypath.c_str())) == NULL)
        {
            cout << "Error in opening directory" << endl;
            return;
        }
        // cout<<directory_path<<endl;
        struct stat filestat;
        off_t num_block = filestat.st_blocks;
        // cout<<num_block<<endl;

        while ((fileentry = readdir(dir)) != nullptr)
        {
            //  struct stat filestat;
            string file_ans = " ";
            string file_path = directory_path + "/" + fileentry->d_name;
            if (lstat(file_path.c_str(), &filestat) == -1)
            {
                continue;
            }
            // cout<<fileentry->d_name<<endl;

            // cout<<file_path<<endl;
            struct passwd *user = getpwuid(filestat.st_uid);
            struct group *grp = getgrgid(filestat.st_gid);
            int filesize = filestat.st_size;
            char time[200];
            strftime(time, sizeof(time), "%b %d  %H:%M", localtime(&filestat.st_mtime));
            // cout<<time<<endl;
            // cout<<filesize<<endl;
            // cout<<user->pw_name<<endl;

            // file type and permission
            string temp = "";
            if (S_ISREG(filestat.st_mode))
            {
                temp = "-";
            }
            else if (S_ISDIR(filestat.st_mode))
            {
                temp = "d";
            }
            file_ans += temp;
            // permission
            string per = permission;
            for (int i = 0; i < 9; i++)
            {
                if (!(filestat.st_mode & (1 << (8 - i))))
                {
                    per[i] = '-';
                }
            }
            file_ans += per;
            file_ans += " ";
            file_ans += "1";
            file_ans += " ";
            file_ans += user->pw_name;
            file_ans += " ";
            file_ans += grp->gr_name;
            file_ans += " ";
            file_ans += to_string(filesize);
            file_ans += " ";
            file_ans += time;
            file_ans += " ";
            file_ans += fileentry->d_name;

            cout << file_ans << endl;
            temp = "";
        }
    }
    /*-----------------------------------------------------------------------------------ls -a -l-----------------------------*/

    // NEED WORK HERE
    /*
    else if(!strcmp(next_command,"-a")&&!strcmp(up_next_command,"-l")){
         string current_dir=solve_pwd();
        //cout<<current_dir<<endl;
        string parent_dir="";
        int size=current_dir.size();
        int last=size;
        while(current_dir[last]!='/'){
            last--;
        }
        parent_dir=current_dir.substr(0,last);
       // cout<<parent_dir<<endl;
       DIR *dir;
        // string file_ans="";
        string directorypath=parent_dir;
        string directory_path=directorypath.c_str();
        struct dirent *fileentry;
        if((dir = opendir(directorypath.c_str()))==NULL){
            cout<<"Error in opening directory"<<endl;
            return ;
        }
           // cout<<directory_path<<endl;
           struct stat filestat;
           off_t num_block=filestat.st_blocks;
           //cout<<num_block<<endl;


           while ((fileentry = readdir(dir))!=nullptr){
          //  struct stat filestat;
            string file_ans=" ";
            string file_path=directory_path+"/"+fileentry->d_name;
              if(lstat(file_path.c_str(),&filestat)==-1){
                continue;
              }
              //cout<<fileentry->d_name<<endl;

              //cout<<file_path<<endl;
              struct passwd*user=getpwuid(filestat.st_uid);
              struct group*grp=getgrgid(filestat.st_gid);
              int filesize=filestat.st_size;
               char time[200];
               strftime(time,sizeof(time),"%b %d  %H:%M",localtime(&filestat.st_mtime));
             // cout<<time<<endl;
             // cout<<filesize<<endl;
              //cout<<user->pw_name<<endl;

              //file type and permission
              string temp="";
            if(S_ISREG(filestat.st_mode)){
                temp="-";
                }
                else if(S_ISDIR(filestat.st_mode)){
                    temp="d";
                }
    file_ans+=temp;
    //permission
    string per=permission;
    for(int i=0;i<9;i++){
        if(!(filestat.st_mode&(1 << (8-i)))){
         per[i]='-';
        }
    }
    file_ans+=per;
    file_ans+=" ";
    file_ans+="1";
    file_ans+=" ";
    file_ans+=user->pw_name;
    file_ans+=" ";
    file_ans+=grp->gr_name;
    file_ans+=" ";
    file_ans+=to_string(filesize);
    file_ans+=" ";
    file_ans+=time;
    file_ans+=" ";
    file_ans+=fileentry->d_name;

    cout<<file_ans<<endl;
    temp="";

    }



    }
    */
}

/*--------------------------------------------------------------search ------------------------------------------------------------------*/

bool solve_search(string file_name, string dir_name)
{
    DIR *dir = opendir(dir_name.c_str());
    struct dirent *fileentry;

    if (!dir)
    {
        cout << "error in opening directory" << endl;
        return false;
    }
    // cout<<fileentry->d_name<<endl;
    // cout<<file_name<<endl;

    if (strcmp(fileentry->d_name, file_name.c_str()) == 0)
        return true;
    bool ans = false;
    while ((fileentry = readdir(dir)) != nullptr)
    {
        // cout<<"Here1"<<endl;
        if (fileentry->d_type == DT_DIR)
        {
            //  cout<<"Here2"<<endl;
            if ((strcmp(fileentry->d_name, ".") == 0) || (strcmp(fileentry->d_name, "..") == 0))
            {
                continue;
            }

            string temp = dir_name + "/" + fileentry->d_name;
            //    cout<<"Here"<<endl;
            if (solve_search(file_name, temp))
            {
                closedir(dir);
                return true;
            }
        }
        else
        {
            if (fileentry->d_name == file_name)
            {
                closedir(dir);
                return true;
            }
        }
    }
    /*else{

        //Check for extension
        string temp=fileentry->d_name;
        if(temp.size()>=file_name)&&
    }*/

    // main function
    closedir(dir);
    return false;
}

/*---------------------------------------------------- background------------------------------------------*/
void solve_background(vector<string> &tok)
{
    const char *curr_command = tok[0].c_str();
    int total_command = tok.size() - 1;

    tok.pop_back();
    // solve_background(tok);
    pid_t tt = fork();
    // cout<<tt<<endl;
    if (tt == 0)
    {
        char *arr[tok.size() + 1];
        for (int i = 0; i < total_command; i++)
            arr[i] = const_cast<char *>(tok[i].c_str());
        arr[total_command - 1] = nullptr;
        // int status = execvp(curr_command, arr);
        int status = system(tok[0].c_str());
        if (status == -1)
        {
            cout << "Process failed" << endl;
            exit(1);
        }
    }
    else if (tt > 0)
        cout << tt << endl;
    else
        cout << "Error in creating child process" << endl;
    // else{
    //   cout<<"failed to create child process"<<endl;
    //}
}
void solve_foreground(vector<string> &tok)
{
    const char *curr_command = tok[0].c_str();
    int total_command = tok.size() - 1;

    tok.pop_back();
    // total_command=tok.size();
    //  solve_background(tok);
    pid_t tt = fork();
    // cout<<tt<<endl;
    if (tt == 0)
    {
        char *arr[tok.size() + 1];
        for (int i = 0; i < total_command; i++)
            arr[i] = const_cast<char *>(tok[i].c_str());
        arr[total_command - 1] = nullptr;
        // int status = execvp(curr_command, arr);
        int status = system(tok[0].c_str());
        if (status == -1)
        {
            cout << "Process failed" << endl;
            exit(1);
        }
    }
    else if (tt > 0)
    {
        int num;
        waitpid(tt, &num, 0);
    }
    else
        cout << "Error in creating child process" << endl;
    // else{
    //   cout<<"failed to create child process"<<endl;
    //}
}
/*------------------------------------------------solve_pinfo----------------------------------------------*/

/*void solve_pinfo(){
    string pid=to_string(getpid));
cout<<"pid -- "<<to_string(getpid)<<endl;
cout<<"Process Status -- "<<solve_pinfo_status(pid);

}*/

/*-------------------------------------------------solve_pinfo_pid-----------------------------------------------*/
void solve_pinfo_pid()
{
}

int main(void)
{

    /*------------------------------creating PROMT--------------------------------------*/
    // init_shell();
    const char *env[3] = {"HOME", "PATH", "LOGNAME"};
    /*for(int i=0;i<3;i++){
        const char *content=getenv(env[i]);
        if(content!=NULL){
            cout<<env[i]<<" ="<<content<<endl;

        }else{
            cout<<"Not found"<<endl;
        }
    }*/
    struct utsname uts;

    if (uname(&uts) < 0)
    {
        cout << "Erroe" << endl;
    }
    else
    {
        cout << uts.sysname << endl;
    }
    string name1 = uts.sysname;
    //  cout<<getenv(env[2])<<"@"<<name1<<":";
    // sprintf(getenv(env[2]));
    char buffer[1024];
    getcwd(buffer, sizeof(buffer));
    string curr_dir = buffer;
    char *home_dir = getenv(env[0]);
    if (home_dir && curr_dir.find(home_dir) == 0)
    {
        curr_dir.replace(0, strlen(home_dir), "~");
    }
    // cout << getenv(env[2]) << "@" << name1 << ":" << curr_dir << " ";

    /*-----------------------------------taking input----------------------------*/

    while (true)
    {
        string name2 = uts.sysname;
        cout << getenv(env[2]) << "@" << name2 << ":" << curr_dir << " ";

        string str1;
        getline(cin, str1);

        vector<string> tok;
        char st[str1.size() + 1];
        strcpy(st, str1.c_str());
        const char *delim = " \t";

        char *token = strtok(st, delim);
        // tok.push_back(token);
        while (token != NULL)
        {
            tok.push_back(token);
            token = strtok(NULL, delim);
        }
        /*----------------------------------printing tokens-----------------*/
        /*for(int i=0;i<tok.size();i++){
           cout<<tok[i]<<endl;
        }*/
        if (tok.size() > 0)
        {
            // cout<<"hello"<<endl;
            int total_command = tok.size() - 1;
            cout << tok[total_command];
            const char *curr_command = tok[0].c_str();
            const char *next_command1 = tok[1].c_str();

            const char *next_command2 = tok[2].c_str();
            const char *last_command = tok[total_command].c_str();
            // const char *next_command=tok[1].c_str();
            // cout<<curr_command<<endl;
            string prev_d;
            if (!strcmp(curr_command, "cd"))
            {
                solve_cd(tok, prev_d);
            }

            /*-------------------------Background process-----------------------------------------*/
            else if (!strcmp(last_command, "&"))
            {
                //
                solve_background(tok);
                /*
                tok.pop_back();
                //solve_background(tok);
                pid_t tt=fork();
                //cout<<tt<<endl;
                if(tt==0){
                char *arr[tok.size()+1];
                for(int i=0;i<total_command;i++)arr[i]=const_cast<char*>(tok[i].c_str());
                arr[total_command]=nullptr;
                int status=execvp(curr_command,arr);
                if(status==-1){
                    cout<<"Process failed"<<endl;
                    exit(1);
                }
                }else if(tt>0)cout<<tt<<endl;
                else cout<<"Error in creating child process"<<endl;
                //else{
                  //  cout<<"failed to create child process"<<endl;
                //}


               }*/
            }
            else if (!strcmp(curr_command, "echo"))
            {
                // cout<<"echo here"<<endl;
                solve_echo(tok);
            }
            else if (!strcmp(curr_command, "pwd"))
            {
                cout << solve_pwd() << endl;
            }
            else if (!strcmp(curr_command, "ls"))
            {
                // solve_ls(tok);
                if (next_command1 != nullptr)
                {
                    if (next_command2 != nullptr)
                    {
                        int idx = 2;

                        struct stat filestat;
                        if (stat(next_command2, &filestat) == 0)
                        {
                            if (S_ISDIR(filestat.st_mode))
                            {
                                cout << "it is directory" << endl;
                                chdir(next_command2);
                                solve_ls(tok);
                                chdir(curr_command);
                            }
                        }
                        else
                        {
                            vector<string> temp;
                            temp.push_back("ls");
                            temp.push_back("..");
                            solve_ls(temp);
                        }
                        idx++;
                        next_command2 = tok[idx].c_str();
                    }
                    else
                    {
                        solve_ls(tok);
                    }
                    // directory section
                }
                else
                {
                    cout << "I am here";
                    vector<string> temp;
                    temp.push_back("ls");
                    struct stat filestat;
                    if (stat(next_command1, &filestat) == 0)
                    {
                        if (S_ISDIR(filestat.st_mode))
                        {
                            cout << "it is directory" << endl;
                            chdir(next_command1);
                            solve_ls(temp);
                            chdir(curr_command);
                        }
                        else
                        {
                            solve_ls(tok);
                        }
                    }
                    else
                    {
                        solve_ls(tok);
                    }
                }
            }
            else if (!strcmp(curr_command, "search"))
            {

                string file_search = next_command1;
                // cout<<file_search<<" "<<endl;;
                string present_dir = solve_pwd();
                int n = present_dir.size();
                string new_dirr = "";
                int i = 0;
                for (i = n - 1; i >= 0; i--)
                {
                    if (present_dir[i] == '/')
                    {
                        break;
                    }
                }
                new_dirr = present_dir.substr(0, i);
                // cout<<new_dirr<<endl;

                cout << present_dir << endl;
                if (solve_search(next_command1, present_dir))
                    cout << "True" << endl;
                else
                    cout << "False" << endl;
            }
            /*   else if(!strcmp(tok[0].c_str()),"pinfo"){
                   if(next_command1==nullptr){
                       //only pinfo
                       solve_pinfo();
                   }else{
                       //pinfo with pid
                       solve_pinfo_pid();
                   }
               }
                      */
            else if (!strcmp(tok[0].c_str(), "exit"))
            {
                return 0;
            }
            else
            {
                solve_foreground(tok);
            }
        }
    }
}
