#include "../shell.h"

void process_command(char *input)
{
	LexerBuffer lexerbuf = {0};
	lexer_build(input, &lexerbuf);
	if (lexerbuf.count > 0) {
		ASTreeNode *exectree = parse_tokens(&lexerbuf);
		execute_syntax_tree(exectree);
		free(exectree);
	}
	lexer_free(&lexerbuf);
}

void lexer_build(char *input, LexerBuffer *lexerbuf)
{
	glob_t globbuf;
	memset(&globbuf, 0, sizeof(globbuf));

	char *token = strtok(input, " \t\n");
	int first_call = 1; // Track whether it's the first call to glob

	while (token != NULL) {
		int flags = first_call ? GLOB_DOOFFS | GLOB_NOCHECK : GLOB_DOOFFS | GLOB_NOCHECK | GLOB_APPEND;
		if (glob(token, flags, NULL, &globbuf) != 0) {
			// Handle glob error if needed
		}
		first_call = 0; // After the first call, always use GLOB_APPEND
		token = strtok(NULL, " \t\n");
	}

	lexerbuf->tokens = globbuf.gl_pathv;
	lexerbuf->count = globbuf.gl_pathc;
}


void lexer_free(LexerBuffer *lexerbuf) {
	if (lexerbuf->tokens) {
		globfree(&(glob_t){.gl_pathv = lexerbuf->tokens, .gl_pathc = lexerbuf->count});
		lexerbuf->tokens = NULL;
		lexerbuf->count = 0;
	}
}


ASTreeNode *parse_tokens(LexerBuffer *lexerbuf)
{
	ASTreeNode *node = malloc(sizeof(ASTreeNode));
	node->command = lexerbuf->tokens[0];
	node->args = &lexerbuf->tokens[1];
	node->left = NULL;
	node->right = NULL;
	return node;
}
