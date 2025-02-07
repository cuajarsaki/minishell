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

void handle_pipe(ASTreeNode *node, t_env *env_list)
{
    int pipe_fd[2];
    pid_t pid1, pid2;

    if (pipe(pipe_fd) == -1) {
        perror("[ERROR] Failed to create pipe");
        exit(EXIT_FAILURE);
    }
    fprintf(stderr, "[DEBUG] Pipe created successfully: [%d, %d]\n", pipe_fd[0], pipe_fd[1]);

    // First child: Executes left built-in command
    if ((pid1 = fork()) == -1) {
        perror("[ERROR] Failed to fork first child process");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) { // Left built-in process
        fprintf(stderr, "[DEBUG] First child executing built-in command: %s\n", node->left->command);

        // Redirect stdout to the write end of the pipe
        dup2(pipe_fd[1], STDOUT_FILENO);
        close(pipe_fd[0]); // Close unused read end
        close(pipe_fd[1]); // Close write end after duplication

        // Execute left built-in command
        if (!execute_builtin(node->left, &env_list)) {
            fprintf(stderr, "[ERROR] Left command is not a built-in: %s\n", node->left->command);
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    }

    // Second child: Executes right built-in command
    if ((pid2 = fork()) == -1) {
        perror("[ERROR] Failed to fork second child process");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) { // Right built-in process
        fprintf(stderr, "[DEBUG] Second child executing built-in command: %s\n", node->right->command);

        // Redirect stdin to the read end of the pipe
        dup2(pipe_fd[0], STDIN_FILENO);
        close(pipe_fd[1]); // Close unused write end
        close(pipe_fd[0]); // Close read end after duplication

        // Execute right built-in command
        if (!execute_builtin(node->right, &env_list)) {
            fprintf(stderr, "[ERROR] Right command is not a built-in: %s\n", node->right->command);
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    }

    // Parent process: Close both pipe ends
    close(pipe_fd[0]);
    close(pipe_fd[1]);

    // Wait for both child processes to finish
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}






// void handle_pipe(ASTreeNode *node)
// {
//     int pipe_fd[2];
//     if (pipe(pipe_fd) == -1)
//     {
//         perror("pipe");
//         exit(1);
//     }

//     pid_t pid = fork();
//     if (pid == 0)
//     {
//         // 子プロセス：左側のコマンドを処理
//         dup2(pipe_fd[1], STDOUT_FILENO); // 標準出力をパイプの書き込み側にリダイレクト
//         close(pipe_fd[0]);
//         close(pipe_fd[1]);
//         execute_syntax_tree(node->left); // 左側のノードを再帰的に処理
//         exit(0);
//     }
//     else if (pid > 0)
//     {
//         // 親プロセス：右側のコマンドを処理
//         dup2(pipe_fd[0], STDIN_FILENO); // 標準入力をパイプの読み取り側にリダイレクト
//         close(pipe_fd[0]);
//         close(pipe_fd[1]);
//         execute_syntax_tree(node->right); // 右側のノードを再帰的に処理
//         waitpid(pid, NULL, 0);            // 子プロセスの終了を待機
//     }
//     else
//     {
//         perror("fork");
//         exit(1);
//     }
// }
