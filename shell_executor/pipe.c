/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jidler <jidler@student.42tokyo.jp>         +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/14 23:30:59 by pchung            #+#    #+#             */
/*   Updated: 2025/02/06 15:42:56 by jidler           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../shell.h"

void handle_pipe(ASTreeNode *node)
{
    int pipe_fd[2];
    if (pipe(pipe_fd) == -1)
    {
        perror("pipe");
        exit(1);
    }

    pid_t pid = fork();
    if (pid == 0)
    {
        // 子プロセス：左側のコマンドを処理
        dup2(pipe_fd[1], STDOUT_FILENO); // 標準出力をパイプの書き込み側にリダイレクト
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execute_syntax_tree(node->left); // 左側のノードを再帰的に処理
        exit(0);
    }
    else if (pid > 0)
    {
        // 親プロセス：右側のコマンドを処理
        dup2(pipe_fd[0], STDIN_FILENO); // 標準入力をパイプの読み取り側にリダイレクト
        close(pipe_fd[0]);
        close(pipe_fd[1]);
        execute_syntax_tree(node->right); // 右側のノードを再帰的に処理
        waitpid(pid, NULL, 0);            // 子プロセスの終了を待機
    }
    else
    {
        perror("fork");
        exit(1);
    }
}
