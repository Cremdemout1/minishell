# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdbool.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <signal.h>
# include <fcntl.h>
# include <limits.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>

typedef struct s_token
{
	char			*token;
	int				type;
	struct s_token	*next;
}				t_token;

typedef struct variable_holder
{
	char	*new;
	char	*before;
	char	*quoted;
	char	*after;
	char	*temp;
	char	*temp2;
	char	*expanded;
	char	*result;
	int		i;
	int		start;
	int		j;
	int		wc;
	int		k;
	char	**array;
}				t_var_holder;

char	*ft_substr(char const *s, unsigned int start, size_t len)
{
	unsigned int	i;
	size_t			length;
	char			*dest;

	i = 0;
	length = strlen(s);
	if (s == NULL)
		return (NULL);
	if (start >= length)
		return (strdup(""));
	if (start + len > length)
		len = length - start;
	dest = (char *)malloc((len + 1) * sizeof(char));
	if (dest == NULL)
		return (NULL);
	while (len != 0 && i < len)
	{
		dest[i] = s[start + i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

t_token	*addtok(void *content)
{
	t_token	*list;

	list = malloc(sizeof(t_token));
	if (!list)
		return (NULL);
	list->token = content;
	list->next = NULL;
	return (list);
}

t_token	*get_last_tok(t_token *lst)
{
	if (!lst)
		return (NULL);
	while (lst->next)
		lst = lst->next;
	return (lst);
}

void	go_to_next(t_token **lst, t_token *new)
{
	t_token	*current;

	if (!new)
		return ;
	current = get_last_tok(*lst);
	if (lst && current)
		current->next = new;
	else
		*lst = new;
}


void handle_quotes(t_var_holder *h, char *str)
{
    char quote = str[h->i];
    h->i++;
    while (str[h->i] && str[h->i] != quote)
        h->i++;
    if (str[h->i] == quote)
        h->i++;
}

int count_words(char *str)
{
    t_var_holder h;

    h.i = 0;
    h.wc = 0;
    while (str[h.i])
    {
        while (str[h.i] && (str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
        	h.i++;
        if (str[h.i] == '\'' || str[h.i] == '\"')
        {
            handle_quotes(&h, str);
            h.wc++;
        }
        else if (str[h.i] && !(str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
        {
            while (str[h.i] && !(str[h.i] == ' ' || str[h.i] == '\t' || str[h.i] == '\n'))
                h.i++;
            h.wc++;
        }
    }
    return h.wc;
}

void update_iterator(t_var_holder *h, char *str)
{
    while (str[h->i])
    {
        while (str[h->i] && (str[h->i] == ' ' || str[h->i] == '\t' || str[h->i] == '\n'))
			h->i++;
        h->j = h->i;
        if (str[h->i] == '\'' || str[h->i] == '\"')
        {
            handle_quotes(h, str);
        }
        else
        {
            while (str[h->i] && !(str[h->i] == ' ' || str[h->i] == '\t' || str[h->i] == '\n'))
                h->i++;
        }
        if (h->i > h->j)
		{
			h->array[h->k] = ft_substr(str, h->j, h->i - h->j);
			h->k++;
		}
    }
}

char	**token_array(char *str)
{
	t_var_holder	h;

	h.i = 0;
	h.j = 0;
	h.k = 0;
	h.wc = count_words(str);
	h.array = malloc(sizeof(char *) * (h.wc + 1));
	if (!h.array)
		return (NULL);
	update_iterator(&h, str);
	h.array[h.k] = NULL;
	return (h.array);
}


void	tokenize(t_token **list, char *str)
{
	int		i;
	char	**array;
	t_token	*new_node;

	i = 0;
	array = token_array(str);
	if (!array)
		return;
	while (array[i])
	{
		new_node = addtok(strdup(array[i]));
		new_node->type = 0;
		go_to_next(list, new_node);
		i++;
	}
	for (int k = 0, i = 0; array[k]; k++)
    {
        free(array[k]);
    }
}

int main(void)
{
    t_token *list = NULL;
    t_token *tmp;
    char *str = "echo \"   hello     world\"";
    tokenize(&list, str);
    tmp = list;
    while (tmp)
    {
        printf("%s\n", tmp->token);
        tmp = tmp->next;
    }
    return 0;
}
