*This project has been created as part of the 42 curriculum by molaoui.*
 
# get_next_line
 
## Description
 
`get_next_line` is a C function that reads a file descriptor and returns one line each time it is called, including the `\n` at the end (if there is one). When there is nothing left to read, or an error happens, it returns `NULL`.
 
```c
char *get_next_line(int fd);
```
 
The goal of the project is to learn about static variables, dynamic memory management, and reading from a file descriptor with a fixed-size buffer.
 
## Files
 
| File | Content |
|------|---------|
| `get_next_line.c` | `get_next_line`, `read_node`, `fill_stash`, `get_line_len`, `extract_line` |
| `get_next_line_utils.c` | `has_new_line`, `new_node`, `free_list`, `clean_stash` |
| `get_next_line.h` | Prototypes, the `t_list` struct, and the default `BUFFER_SIZE` |
 
## Instructions
 
### Compile
 
`BUFFER_SIZE` can be set at compile time. If you don't set it, the default is 42.
 
```sh
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c
```
 
### Example
 
```c
#include "get_next_line.h"
#include <fcntl.h>
#include <stdio.h>
 
int main(void)
{
    int     fd;
    char    *line;
 
    fd = open("file.txt", O_RDONLY);
    while ((line = get_next_line(fd)) != NULL)
    {
        printf("%s", line);
        free(line);
    }
    close(fd);
    return (0);
}
```
 
The caller must `free` every line that is returned.
 
## Algorithm
 
The leftover data between calls is kept in a **linked list** called the stash. It is stored in a `static` variable, so it is still there the next time the function is called.
 
```c
typedef struct s_list
{
    char            *content;
    int             len;
    struct s_list   *next;
}   t_list;
```
 
Each node holds one chunk of up to `BUFFER_SIZE` bytes that came from one `read`.
 
Each call to `get_next_line` does these steps:
 
1. **Check the input.** If `fd < 0`, `BUFFER_SIZE <= 0`, or `read(fd, &c, 0) < 0`, return `NULL`.
2. **`fill_stash`**: read chunks with `read` and add each one as a new node at the end of the list, until a chunk contains `\n` or the file ends. If the stash already has a `\n` from the previous call, it does not read.
3. **`get_line_len`**: count the characters from the start of the stash up to and including the first `\n` (or to the end if there is none).
4. **`extract_line`**: allocate a string of that length and copy the characters from the nodes into it.
5. **`clean_stash`**: remove the characters that were returned. Nodes that were fully used are freed, and the first remaining node is shifted so that only the leftover stays.
### Why a linked list
 
Each `read` gives a chunk of `BUFFER_SIZE` bytes, and a line can be longer or shorter than a chunk. With a linked list, each chunk is stored in its own node as it arrives, so the data already read is never copied or reallocated when more data comes in. The line is only built once, at the end, with a single `malloc` of the exact size.
 
### Return values of the helper functions
 
`read_node` and `fill_stash` return:
 
- `> 0`: data was read (or a `\n` was already in the stash)
- `0`: end of file
- `-1`: error (`read` or `malloc` failed)
On error, the stash is freed and `NULL` is returned.
 
### Performance
 
`fill_stash` keeps a pointer to the last node (`tail`). New nodes are attached directly to it, and only the newest node is checked for `\n`. Because of this, the function does not walk or rescan the whole list on every read, which keeps it fast even with `BUFFER_SIZE = 1` and very long lines.
 
## Testing
 
Tested with [gnlTester](https://github.com/Tripouille/gnlTester) with `BUFFER_SIZE` = 1, 42 and 10000000, and with Valgrind to check for memory errors and leaks.
 
## Resources
 
- `man 2 read`, `man 3 malloc`, `man 3 free`
- [gnlTester](https://github.com/Tripouille/gnlTester)
- Valgrind documentation: https://valgrind.org/docs/manual/
### Use of AI
 
AI was used to help for understanding Low level Concepts . The code was written by me.
