# myls
`myls` emulates the functionality of the `ls` command from unix-like systems. This project uses system calls to navigate the UNIX file system from a user-level program.

- Build this program by running `make` in the root directory.

- Execute myls: ```./myls [options] [directory]```

## Options
The following options are supported: -i, -l, -R. These options can be run in any combination; for example ```./myls -ilR /somedir/``` and ```./myls -i -l /somedir/``` are both valid.

- ```./myls``` prints all files and folders located in the current directory.
- ```./myls [directory]``` prints all files and folders located in the specified directory.
- ```-i``` prints the index number of each file alongside it.
- ```-l``` prints using a long listing format.
- ```-R``` recursively prints subdirectories.
