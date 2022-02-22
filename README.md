# myls
This is a project I wrote in C which mimics the 'ls' command from the UNIX file system. In this project, I learned how to use system calls to navigate the UNIX file system from a user-level program.

To run this program, simply run make in the root directory and execute myls using two optional arguments: ```./myls [options] [directory]```

# Options
Currently, the following options are supported: -i, -l, -R. These options can be run in any combination; for example ```./myls -ilR /somedir/``` and ```./myls -i -l /somedir/``` are both valid. The following screenshots use another repository 'DiscordOnTheGo' as a sample directory.

### No Options
```./myls``` will print all files and folders located in the current directory.

![image](images/myls-only.PNG)

```./myls ../DiscordOnTheGo/``` will print all files and folders located in the specified directory.

![image](images/myls-dir.PNG)

### myls -i
The ```-i``` option prints the index number of each file alongside it.

![image](images/myls-i.PNG)

### myls -l
The ```-l``` option prints using a long listing format.

![image](images/myls-l.PNG)

### myls -R
The ```-R``` option recursively prints subdirectories.

![image](images/myls-R.PNG)

### myls -ilR
An example of using all three options.

![image](images/myls-ilR.PNG)
