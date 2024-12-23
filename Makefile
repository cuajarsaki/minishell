# brew install readline


CMD_DIR = shell_commands
PAR_DIR = shell_parser
EXE_DIR = shell_executor


SRC = main.c $(CMD_DIR)/shell_cd.c $(CMD_DIR)/shell_ls.c $(CMD_DIR)/shell_commands.c $(PAR_DIR)/parser.c $(EXE_DIR)/executor.c\
	$(CMD_DIR)/shell_echo.c $(CMD_DIR)/shell_man.c shell_termcaps/termcaps.c





all: build

build:
	cc $(SRC) -o shell -lreadline -ltermcap

run:
	./shell 