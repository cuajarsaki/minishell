/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/04 17:00:00 by jidler            #+#    #+#             */
/*   Updated: 2025/03/04 17:15:29 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# define ENV_H

# include "../../shell.h"
# include <stdlib.h>
# include <unistd.h>
# include <string.h>

/* ************************************************************************** */
/*                        ENVIRONMENT VARIABLES HANDLING                      */
/* ************************************************************************** */

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}	t_env;

typedef struct s_env_manager
{
	t_env *env_list;  // Pointer to environment linked list
}	t_env_manager;

/* Initialization & Cleanup */
t_env_manager	*init_env_manager(char **envp);
void			free_env_manager(t_env_manager *env_mgr);

/* Environment List Operations */
t_env	*new_env_node(char *key, char *value);
char	*get_env_value(t_env_manager *env_mgr, const char *key);
void	set_env_value(t_env_manager *env_mgr, const char *key, const char *value);
void	unset_env_value(t_env_manager *env_mgr, const char *key);
void	free_env_list(t_env *env_list);

/* Environment Array Operations */
char	**convert_env_list_to_array(t_env_manager *env_mgr);
void	free_env_array(char **envp);


