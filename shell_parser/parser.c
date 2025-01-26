#include "../shell.h"

void process_command(char *input) {
    LexerBuffer lexerbuf = {0};
    lexer_build(input, &lexerbuf); // Tokenize the input
    if (lexerbuf.count > 0) {
        ASTreeNode *exectree = parse_tokens(&lexerbuf); // Build the syntax tree

        // Try executing the command and handle errors gracefully
        if (exectree) {
            execute_syntax_tree(exectree);
        } else {
            write(STDERR_FILENO, "Error: Failed to parse command\n", 31);
        }
        free(exectree); // Free the syntax tree
    }

    lexer_free(&lexerbuf); // Clean up lexer buffer
}


void lexer_build(char *input, LexerBuffer *lexerbuf)
{
    size_t capacity = 10; // Use size_t for consistency with lexerbuf->count
    lexerbuf->tokens = malloc(capacity * sizeof(char *));
    lexerbuf->count = 0;

    char *token = ft_strtok(input, " \t\n"); // Tokenize the input string
    while (token != NULL) {
        if (lexerbuf->count >= capacity) {
            // Expand the token array dynamically if needed
            capacity *= 2;
            lexerbuf->tokens = ft_realloc(lexerbuf->tokens, lexerbuf->count, capacity);
        }
        // Duplicate the token and store it
        lexerbuf->tokens[lexerbuf->count++] = ft_strdup(token);

        token = ft_strtok(NULL, " \t\n"); // Get the next token
    }
}


void lexer_free(LexerBuffer *lexerbuf)
{
    if (lexerbuf->tokens) {
        for (size_t i = 0; i < lexerbuf->count; i++) { // Use size_t for the loop counter
            free(lexerbuf->tokens[i]); // Free each token string
        }
        free(lexerbuf->tokens); // Free the token array
        lexerbuf->tokens = NULL;
        lexerbuf->count = 0;
    }
}


ASTreeNode *parse_tokens(LexerBuffer *lexerbuf)
{
    // Allocate memory for the AST node
    ASTreeNode *node = malloc(sizeof(ASTreeNode));

    // The first token is the command
    node->command = lexerbuf->tokens[0];

    // The remaining tokens are arguments
    node->args = &lexerbuf->tokens[1];

    // No left or right nodes for this implementation
    node->left = NULL;
    node->right = NULL;

    return node;
}
