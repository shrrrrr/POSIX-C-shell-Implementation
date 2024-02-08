
# POSIX SHELL

This is POSIX shell implementation using c++.

1) Given Shell capable of running commands like cd,echo,pwd,ls with their options

2) 'search' command also included in shell which is used to search for file.

3) system commands like background/foreground are also implemented.
## Authors

- [@soham kale](https://github.com/shrrrrr)


## Running Tests

To run shell, run the following command on your terminal after downloading cpp file.

```bash
  g++ 2023202018.cpp

```
```bash
./a.out 2023202018.cpp
```



## Shell Description

1) (cd,echo,pwd) commands: Basic Logic is used to implement tese commands from scratch. 'chdir' system call used for cd. pwd command implemented using 'getcwd'

2) 'ls' command comes with multiple options and directories.Here in this you are allowed to enter only three argument. I will further update my code with new ideas to implement multiple options and directorise with ls commands.

3) search command included which is capable of searching given file in present working directory.

4) For background and foreground process 'fork' system call used. 