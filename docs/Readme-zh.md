[English](../Readme.md) | [日本語](./Readme-ja.md) | 繁體中文

# Minishell - 如shell般美麗
<p align="left">
  <img src="https://img.shields.io/badge/-C-213a70.svg?logo=C&style=flat">
  <img src="https://img.shields.io/badge/-Linux-ea5520.svg?logo=linux&style=flat">
  <img src="https://img.shields.io/badge/Subject_Version-8.02-3E72BC.svg">
  <img src="https://github.com/cuajarsaki/minishell/actions/workflows/test.yml/badge.svg" alt="minishell">
</p>
<p align="center">
  <img src="https://github.com/cuajarsaki/minishell/blob/a5fad336ac54386db5af09fb4ca2ee568a382864/docs/minishell_icon.png" alt="Minishell 42 project badge" style="width:10%;"/>
</p>

Minishell 是一個用 C 語言編寫的迷你Shell專題，模仿 Bash 的基本功能。
在學習程序管理、檔案描述符和訊號處理的同時，構建了自己的命令列介面。
我們特別注重精確地複製 bash 在退出狀態值和錯誤訊息方面的行為，確保minishell以一種一致且熟悉的方式回應錯誤，適合已習慣使用 bash 的使用者。

## 功能特點
- **互動提示符：** 顯示使用者輸入提示符。
- **命令執行：** 使用 `PATH` 變數或相對/絕對路徑執行命令。
- **重定向和管道：** 支援輸入/輸出重定向（`<`、`>`、`<<`、`>>`）和管道（`|`）。
- **環境變數：** 展開變數（例如，`$VAR` 和 `$?` 用於退出狀態）。
- **訊號處理：** 正確回應 Ctrl-C、Ctrl-D 和 Ctrl-\。

## 內建命令
| 命令      | 描述                                                          |
|-----------|--------------------------------------------------------------|
| `echo`    | 在終端上輸出文字（支援 `-n` 選項省略換行符）。                 |
| `cd`      | 更改當前目錄。                                                |
| `pwd`     | 顯示當前工作目錄。                                            |
| `export`  | 設定環境變數。                                                |
| `unset`   | 移除環境變數。                                                |
| `env`     | 列出所有環境變數。                                            |
| `exit`    | 退出minishell。                                                    |

## Norminette 規範遵循
本專題嚴格遵循 42 School Norminette 規則（v3.3.55）。
Norminette 在所有 42 的 C 專題中強制執行一致的編碼風格，包括：

- 每個函數最多 25 行
- 每行最多 80 個字元
- 每個檔案最多 5 個函數
- 特定的變數宣告和命名慣例
- 限制使用標準庫函數
- 結構化的函數標頭格式
- 禁止使用 for 、switch 或 do-while 
- 必須使用 tab 作為縮排

本儲存庫中的所有程式碼都能通過官方 42 Norminette 驗證器而不產生錯誤。

><img src="https://github.com/cuajarsaki/minishell/blob/a5fad336ac54386db5af09fb4ca2ee568a382864/docs/norminette.png" alt="grade" style="width:40%;"/>

## 最終成績
<img src="https://github.com/user-attachments/assets/765ec3c2-7927-4a92-b718-ec39fc93c64d" alt="grade" style="width:20%;"/>

提交日期：2025年3月9日

必修部分：100/100

bonus部分：0/15

## flowchart（主幹部分）

```mermaid
  
flowchart LR
    A[開始 main] --> B[初始化 env_list] --> C[設置終端機] --> D[執行 shell] --> E[清理資源] --> F[退出]
    
    D --> G[run_shell 循環]
    G --> H[處理信號] --> I[獲取輸入] --> J{有輸入？}
    J -->|沒有| G
    J -->|有| K[解析語法樹] --> L[執行語法樹] --> M[釋放語法樹] --> G

```

##  flowchart（執行部分）
```mermaid

graph TD
    A[執行語法樹 exec_ast] --> B[執行命令組 exec_command_group]
    B --> C{沒有命令？}
    C -->|是| D[返回 0]
    C -->|否| E{是單一命令？}
    E -->|是| F{是 exit/cd/unset/export？}
    F -->|是| G[執行內置命令 exec_cmd_builtin]
    F -->|否| H[創建子進程 fork]
    H -->|子進程| I[執行子進程 exec_child_process]
    H -->|父進程| J[執行父進程 exec_parent]
    E -->|否| K[處理命令 process_command]
    K --> L[設置管道並創建子進程]
    L --> M[創建子進程 fork]
    M -->|子進程| N[執行子進程 execute_child]
    M -->|父進程| O[設置父進程管道]
    K --> P[執行父進程 exec_parent]
    B --> Q[釋放命令組 free_command_group]
    A --> R[返回退出狀態]
    I --> S[執行命令 exec_cmd]
    N --> S
    S --> T[設置文件目錄]
    T --> U[處理追加輸出]
    T --> V[處理文字塊 heredoc]
    T --> W[處理覆寫輸出]
    T --> X[處理輸入重定向]
    S --> Y[保存文件描述符]
    S --> Z[應用重定向]
    S --> AA[處理命令]
    AA --> AB{是內置命令？}
    AB -->|是| AC[處理內置命令]
    AB -->|否| AD[執行非內置命令]
    AC --> AE[執行內置命令]
    AD --> AF[處理帶路徑命令]
    AD --> AG[處理無路徑命令]
    AF --> AH[檢查執行權限]
    AG --> AI[在 PATH 中查找可執行文件]
    AI --> AJ[在路徑中搜索可執行文件]
    AD --> AK[執行外部命令]
    AK --> AL[ft_execvp]
    AL --> AM[在路徑中嘗試 execve]
    AL --> AN[execve]
    AK --> AO[輸出錯誤 perror]
    AE --> AP[shell_echo]
    AE --> AQ[shell_cd]
    AE --> AR[shell_exit]
    AE --> AS[shell_pwd]
    AE --> AT[shell_export]
    AE --> AU[shell_unset]
    AE --> AV[shell_env]
    V --> AW[執行文字塊 heredoc]
    AW --> AX[文字塊循環]
    U --> AY[打開文件 open]
    W --> AZ[打開文件 open]
    X --> BA[打開文件 open]
    Z --> BB[複製文件描述符 dup2]
    Y --> BC[複製文件描述符 dup]
    BB --> BD[關閉文件 close]
    BC --> BE[關閉文件 close]
    J --> BF[等待子進程 waitpid]
    O --> BG[關閉文件 close]
    L --> BH[創建管道 pipe]
    BH --> BI[輸出錯誤 perror]
    BI --> BJ[退出 exit]
    
    subgraph "初始化和執行"
        A[執行語法樹 exec_ast]
        B[執行命令組 exec_command_group]
        C{沒有命令？}
        D[返回 0]
        E{是單一命令？}
        F{是 exit/cd/unset/export？}
        G[執行內置命令 exec_cmd_builtin]
        H[創建子進程 fork]
        I[執行子進程 exec_child_process]
        J[執行父進程 exec_parent]
        Q[釋放命令組 free_command_group]
        R[返回退出狀態]
    end
    
    subgraph "命令執行"
        K[處理命令 process_command]
        L[設置管道並創建子進程]
        M[創建子進程 fork]
        N[執行子進程 execute_child]
        O[設置父進程管道]
        P[執行父進程 exec_parent]
        S[執行命令 exec_cmd]
        T[設置文件目錄]
        U[處理追加輸出]
        V[處理文字塊 heredoc]
        W[處理覆寫輸出]
        X[處理輸入重定向]
        Y[保存文件描述符]
        Z[應用重定向]
        AA[處理命令]
        AB{是內置命令？}
        AC[處理內置命令]
        AD[執行非內置命令]
        AE[執行內置命令]
        AF[處理帶路徑命令]
        AG[處理無路徑命令]
        AH[檢查執行權限]
        AI[在 PATH 中查找可執行文件]
        AJ[在路徑中搜索可執行文件]
        AK[執行外部命令]
        AL[ft_execvp]
        AM[在路徑中嘗試 execve]
        AN[execve]
        AO[輸出錯誤 perror]
        AP[shell_echo]
        AQ[shell_cd]
        AR[shell_exit]
        AS[shell_pwd]
        AT[shell_export]
        AU[shell_unset]
        AV[shell_env]
        AW[執行文字塊 heredoc]
        AX[文字塊循環]
        AY[打開文件 open]
        AZ[打開文件 open]
        BA[打開文件 open]
        BB[複製文件描述符 dup2]
        BC[複製文件描述符 dup]
        BD[關閉文件 close]
        BE[關閉文件 close]
        BF[等待子進程 waitpid]
        BG[關閉文件 close]
        BH[創建管道 pipe]
        BI[輸出錯誤 perror]
        BJ[退出 exit]
    end

```

## 安裝
1. **複製儲存庫：**
   ```bash
   git clone https://github.com/yourusername/minishell.git
   ```
2. **進入專題目錄：**
   ```bash
   cd minishell
   ```
3. **使用Makefile編譯(Compile)專題**：
   ```bash
    make
   ```

## 使用方法
- 啟動minishell：
  ```bash
  ./minishell
  ```
- 在提示符處輸入命令。
- 輸入 exit 或按 Ctrl-D 退出。
