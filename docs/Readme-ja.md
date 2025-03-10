[English](../Readme.md) | 日本語 | [繁體中文](./Readme-zh.md)

# Minishell - 美しいシェル
<p align="left">
  <img src="https://img.shields.io/badge/-C-213a70.svg?logo=C&style=flat">
  <img src="https://img.shields.io/badge/-Linux-ea5520.svg?logo=linux&style=flat">
  <img src="https://img.shields.io/badge/Subject_Version-8.02-3E72BC.svg">
  <img src="https://github.com/cuajarsaki/minishell/actions/workflows/test.yml/badge.svg" alt="minishell">
</p>
<p align="center">
  <img src="/home/pchung/work/minishell/docs/minishell_icon.png" alt="Minishell 42 project badge" style="width:10%;"/>
</p>
Minishellは、Bashの基本機能を模倣したCで書かれたミニシェルプロジェクトです。
プロセス管理、ファイルディスクリプタ、シグナル処理について学びながら、独自のコマンドラインインターフェースを構築しました。
終了ステータス値とエラーメッセージに関してbashの動作を正確に再現することにも特に注意を払い、bashに慣れたユーザーにとって一貫性のある親しみやすい方法でエラーに対応するシェルを確保しました。

## 機能
- **インタラクティブプロンプト:** ユーザー入力用のプロンプトを表示します。
- **コマンド実行:** `PATH`変数または相対/絶対パスを使用してコマンドを実行します。
- **リダイレクトとパイプ:** 入出力リダイレクト（`<`、`>`、`<<`、`>>`）とパイプ（`|`）をサポートします。
- **環境変数:** 変数を展開します（例：`$VAR`および終了ステータス用の`$?`）。
- **シグナル処理:** Ctrl-C、Ctrl-D、Ctrl-\に適切に対応します。

## ビルトインコマンド
| コマンド   | 説明                                                                  |
|-----------|---------------------------------------------------------------------|
| `echo`    | テキストを端末に出力します（改行を省略する `-n` オプションをサポート）。   |
| `cd`      | カレントディレクトリを変更します。                                     |
| `pwd`     | 現在の作業ディレクトリを表示します。                                   |
| `export`  | 環境変数を設定します。                                               |
| `unset`   | 環境変数を削除します。                                               |
| `env`     | すべての環境変数を一覧表示します。                                     |
| `exit`    | シェルを終了します。                                                 |

## Norminetteへの準拠
このプロジェクトは、42 School Norminetteルール（v3.3.55）に厳密に準拠しています。Norminetteは、42のすべてのCプロジェクトで一貫したコーディングスタイルを強制するものであり、以下のようなルールがあります：

- 関数あたり最大25行
- 1行あたり最大80文字
- ファイルあたり最大5関数
- 特定の変数宣言と命名規則
- 標準ライブラリ関数の使用制限
- 構造化された関数ヘッダーフォーマット
- forループ、switch文、do-whileループの禁止
- インデントにタブの使用が必須

このリポジトリのすべてのコードは、公式の42 Norminette検証ツールでエラーなく合格しています。

><img src="/home/pchung/work/minishell/docs/norminette.png" alt="grade" style="width:40%;"/>

## 最終評価
<img src="https://github.com/user-attachments/assets/765ec3c2-7927-4a92-b718-ec39fc93c64d" alt="grade" style="width:20%;"/>

検証日：2025年3月9日

必須部分：100/100

ボーナス：0/15

## フローチャート（メイン）

```mermaid
  
flowchart LR
    A[mainを開始] --> B[env_listを初期化] --> C[ターミナルをセットアップ] --> D[minishellを起動] --> E[クリーンアップ] --> F[退出]
    
    D --> G[run_shell ループ]
    G --> H[シグナル処理] --> I[入力を受け取る] --> J{入力あり?}
    J -->|No| G
    J -->|Yes| K[入力を分析] --> L[入力を処理] --> M[メモリ解放] --> G

```

##  フローチャート（出力処理）
```mermaid

graph TD
    A[exec_ast] --> B[exec_command_group]
    B --> C{コマンドが存在しない？}
    C -->|Yes| D[return 0]
    C -->|No| E{単一コマンド？}
    E -->|Yes| F{ビルトインコマンドはこれらか？<br>exit/cd/unset/export}
    F -->|Yes| G[exec_cmd_builtin]
    F -->|No| H[fork]
    H -->|子プロセス| I[exec_child_process]
    H -->|親プロセス| J[exec_parent]
    E -->|No| K[process_command]
    K --> L[パイプ設定とfork]
    L --> M[fork]
    M -->|子プロセス| N[execute_child]
    M -->|親プロセス| O[親プロセスのパイプ設定]
    K --> P[exec_parent]
    B --> Q[command_group解放]
    A --> R[exit_statusを返す]
    I --> S[exec_cmd]
    N --> S
    S --> T[ファイル・ディレクトリ設定]
    T --> U[出力追記処理]
    T --> V[ヒアドキュメント処理]
    T --> W[出力上書き処理]
    T --> X[入力リダイレクト処理]
    S --> Y[ファイルディスクリプタ保存]
    S --> Z[リダイレクト適用]
    S --> AA[コマンド処理]
    AA --> AB{ビルトインコマンド？}
    AB -->|Yes| AC[ビルトイン処理]
    AB -->|No| AD[非ビルトイン実行]
    AC --> AE[ビルトインコマンド実行]
    AD --> AF[パス付きコマンド処理]
    AD --> AG[パスなしコマンド処理]
    AF --> AH[実行権限チェック]
    AG --> AI[PATHから実行ファイル検索]
    AI --> AJ[パス内で実行ファイル検索]
    AD --> AK[外部コマンド実行]
    AK --> AL[ft_execvp]
    AL --> AM[パス内でexecve試行]
    AL --> AN[execve]
    AK --> AO[perror]
    AE --> AP[shell_echo]
    AE --> AQ[shell_cd]
    AE --> AR[shell_exit]
    AE --> AS[shell_pwd]
    AE --> AT[shell_export]
    AE --> AU[shell_unset]
    AE --> AV[shell_env]
    V --> AW[ヒアドキュメント実行]
    AW --> AX[ヒアドキュメントループ]
    U --> AY[open]
    W --> AZ[open]
    X --> BA[open]
    Z --> BB[dup2]
    Y --> BC[dup]
    BB --> BD[close]
    BC --> BE[close]
    J --> BF[waitpid]
    O --> BG[close]
    L --> BH[pipe]
    BH --> BI[perror]
    BI --> BJ[exit]
    
    subgraph "初期化と実行"
        A[exec_ast]
        B[exec_command_group]
        C{コマンドが存在しない？}
        D[return 0]
        E{単一コマンド？}
        F{ビルトインコマンドはこれらか？<br>exit/cd/unset/export}
        G[exec_cmd_builtin]
        H[fork]
        I[exec_child_process]
        J[exec_parent]
        Q[command_group解放]
        R[exit_statusを返す]
    end
    
    subgraph "コマンド実行"
        K[process_command]
        L[パイプ設定とfork]
        M[fork]
        N[execute_child]
        O[親プロセスのパイプ設定]
        P[exec_parent]
        S[exec_cmd]
        T[ファイル・ディレクトリ設定]
        U[出力追記処理]
        V[ヒアドキュメント処理]
        W[出力上書き処理]
        X[入力リダイレクト処理]
        Y[ファイルディスクリプタ保存]
        Z[リダイレクト適用]
        AA[コマンド処理]
        AB{ビルトインコマンド？}
        AC[ビルトイン処理]
        AD[非ビルトイン実行]
        AE[ビルトインコマンド実行]
        AF[パス付きコマンド処理]
        AG[パスなしコマンド処理]
        AH[実行権限チェック]
        AI[PATHから実行ファイル検索]
        AJ[パス内で実行ファイル検索]
        AK[外部コマンド実行]
        AL[ft_execvp]
        AM[パス内でexecve試行]
        AN[execve]
        AO[perror]
        AP[shell_echo]
        AQ[shell_cd]
        AR[shell_exit]
        AS[shell_pwd]
        AT[shell_export]
        AU[shell_unset]
        AV[shell_env]
        AW[ヒアドキュメント実行]
        AX[ヒアドキュメントループ]
        AY[open]
        AZ[open]
        BA[open]
        BB[dup2]
        BC[dup]
        BD[close]
        BE[close]
        BF[waitpid]
        BG[close]
        BH[pipe]
        BI[perror]
        BJ[exit]
    end

```

## インストール方法
1. **リポジトリをクローンする:**
   ```bash
   git clone https://github.com/yourusername/minishell.git
   ```
2. **プロジェクトディレクトリに移動:**
   ```bash
   cd minishell
   ```
3. プロジェクトをコンパイル:
   ```bash
    make
   ```

## 使用方法
- シェルを起動:
  ```bash
  ./minishell
  ```
- プロンプトでコマンドを入力。
- 終了するには「exit」と入力するか、Ctrl-Dを押します。
