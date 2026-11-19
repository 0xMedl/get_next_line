#ifndef GET_NEXT_LINE_H
#define GET_NEXT_LINE_H

#include <unistd.h>
# include <stdlib.h>

#ifndef BUFFER_SIZE
# define BUFFER_SIZE 42
#endif


typedef struct s_list 
{
    char *content;
    int len;
    struct s_list *next;
}   t_list;

int fill_stash(int fd, t_list **stash);
char *extract_line(t_list *stash, size_t len);
void clean_stash(t_list **stash, size_t len);
size_t get_line_len(t_list *stash);
void    free_list(t_list *stash);
char *get_next_line(int fd);
t_list *new_node();
int read_node(int fd, t_list **stash, t_list **tail);
int has_new_line(t_list *stash);

#endif
