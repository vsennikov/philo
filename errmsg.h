/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   errmsg.h                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vsenniko <vsenniko@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/04/16 20:25:52 by vsenniko          #+#    #+#             */
/*   Updated: 2025/05/02 11:58:28 by vsenniko         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERRMSG_H
# define ERRMSG_H

# define NO_ARGS "Not enought/too many arguments\n"
# define ALLOC_GENERAL "Memory allocation for main structure failed\n"
# define NON_VALID_INPUT "Some of the arguments is not valid\n"
# define ALLOC_FORKS "Memory allocation for forks array failed\n"
# define ALLOC_PHILOS "Memory allocation for philos array failed\n"
# define FORK_MUTEXS "pthread_mutex_init for forks failed\n"
# define PRINTING_MUTEXS "pthread_mutex_init for printing lock failed\n"
# define DEAD_MUTEXS "pthread_mutex_init for dead lock failed\n"
# define CHECK_MUTEXS "pthread_mutex_init for check lock failed\n"
# define FINISHED_MUTEX "pthread_mutex_init for finished lock failed\n"
# define MEAL_MUTEX "pthread_mutex_init for meals lock failed\n"

# define NOT_VALID_ERR 0
# define CALLOC_ERR 1
# define DEAD_MUT_ERR 2
# define PRINT_MUT_ERR 3
# define FINISH_MUT_ERR 4
# define DATA_ERR 5
# define L_FORK_ERR 6
# define MEAL_ERR 7
# define D_INST_LOCK 8
#endif