#include "get_next_line.h"

int has_new_line(t_list *stash)
{
    int i;

    while(stash)
    {
        i = 0;
        while(i < stash->len)
        {
            if (stash->content[i] == '\n')
                return (1);
            i++;
        }
        stash = stash->next;
    }
    return (0);
}

t_list *new_node()
{
    t_list *node;

    node = malloc(sizeof(t_list));
    if (!node)
        return (NULL);
    node->content = malloc(BUFFER_SIZE + 1);
    if (!node->content)
    {
        free(node);
        return (NULL);
    }
    node->len = 0;
    node->next = NULL;
    return (node);
}

void    free_list(t_list *stash)
{
    t_list *next;
    while(stash)
    {
        next = stash->next;
        free(stash->content);
        free(stash);
        stash = next;
    }
}

void    append_node(t_list **stash, t_list *node)
{
    t_list *current;
    if(!*stash)
    {
        *stash = node;
        return ;
    }
    current = *stash;
    while(current->next)
        current = current->next;
    current->next = node;
}
void clean_stash(t_list **stash, size_t len)
{
    t_list *current;
    size_t i;
    while(*stash && len >= (size_t)(*stash)->len)
    {
        len -= (*stash)->len;
        current = *stash;
        *stash = current->next;
        free(current->content);
        free(current);
    }
    if (*stash && len > 0)
    {
        i = 0;
        current = *stash;
        while(i < (size_t)current->len - len)
        {
            current->content[i] = current->content[i + len];
            i++;
        }
        current->len -= len;
        current->content[current->len] = '\0';
    }
}