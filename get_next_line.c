#include "get_next_line.h"

int read_node(int fd, t_list **stash, t_list **tail)
{
    int bytes;
    t_list *node;
    node = new_node();
    if (!node)
        return (-1);
    bytes = read(fd, node->content, BUFFER_SIZE);
    if (bytes <= 0)
    {
        free(node->content);
        free(node);
        return (bytes);
    }
    node->len = bytes;
    node->content[bytes] = '\0';
    if (*tail)
        (*tail)->next = node;
    else
        *stash = node;
    *tail = node;
    return (bytes);
}

int fill_stash(int fd, t_list **stash)
{
    t_list *tail;
    int bytes;
    if (has_new_line(*stash))
        return (1);
    tail = *stash;
    while(tail && tail->next)
        tail = tail->next;
    while(1)
    {
        bytes = read_node(fd , stash, &tail);
        if (bytes <= 0)
            return (bytes);
        if (has_new_line(tail))
            return (1);
    }
    return (1);
}

size_t get_line_len(t_list *stash)
{
    size_t len;
    int i;

    len = 0;
    while(stash)
    {
        i = 0;
        while(i < stash->len)
        {
            len++;
            if (stash->content[i] == '\n')
                return (len);
            i++;
        }
        stash = stash->next;
    }
    return (len);
}

char *extract_line(t_list *stash, size_t len) 
{
    size_t i;
    char *line;
    int j;

    line = malloc(len + 1);
    if (!line)
        return (NULL);
    i = 0;
    while(stash && i < len)
    {
        j = 0;
        while(j < stash->len && i < len)
            line[i++] = stash->content[j++];
        stash = stash->next;
    }
    line[i] = 0x00;
    return (line);
}

char *get_next_line(int fd)
{
    size_t len;
    int bytes;
    static t_list *stash;
    char *line;
    char c;

    if (fd < 0 || BUFFER_SIZE <= 0 || read(fd, &c, 0) < 0)
        return (NULL);
    bytes = fill_stash(fd, &stash);

    if (bytes < 0)
    {
        free_list(stash);
        stash = NULL;
        return (NULL);
    }
     if (!stash)
        return (NULL);
    len = get_line_len(stash);
    line = extract_line(stash, len);
    if (!line)
        return (NULL);
    clean_stash(&stash, len);
    return (line);
}
