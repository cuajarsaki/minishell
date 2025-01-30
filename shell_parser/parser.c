#include "../shell.h"

void process_command(char *input, t_env *env_list)
{
    LexerBuffer lexerbuf = {0};
    lexer_build(input, &lexerbuf); // Tokenize the input

    if (lexerbuf.count > 0) {
        ASTreeNode *exectree = parse_tokens(&lexerbuf); // Build the syntax tree

        // Try executing the command and handle errors gracefully
        if (exectree) {
            execute_syntax_tree(exectree, env_list); // Pass env_list here
        } else {
            write(STDERR_FILENO, "Error: Failed to parse command\n", 31);
        }
        free(exectree); // Free the syntax tree
    }

    lexer_free(&lexerbuf); // Clean up lexer buffer
}

void lexer_build(char *input, LexerBuffer *lexerbuf)
{
    size_t capacity = 10;
    lexerbuf->tokens = malloc(capacity * sizeof(char *));
    lexerbuf->count = 0;

    char *ptr = input;
    while (*ptr)
    {
        while (*ptr == ' ' || *ptr == '\t' || *ptr == '\n') // Skip whitespace
            ptr++;

        if (*ptr == '\0') break;

        char token[1024] = {0}; // Temporary buffer for token
        int i = 0;
        int is_assignment = 0; // Flag to track assignment operation

        while (*ptr && *ptr != ' ' && *ptr != '\t' && *ptr != '\n')
        {
            if (*ptr == '=') 
                is_assignment = 1; // If an '=' is found, treat the whole thing as one token

            if ((*ptr == '"' || *ptr == '\'') && is_assignment) // Handle quoted values
            {
                char quote_type = *ptr++; // Store quote type and move forward
                token[i++] = quote_type; // Keep the opening quote

                // printf("[DEBUG] Found opening quote: %c\n", quote_type);

                while (*ptr && *ptr != quote_type) // Capture everything inside quotes
                    token[i++] = *ptr++;

                if (*ptr == quote_type) // Ensure closing quote is found
                {
                    token[i++] = *ptr; // Keep the closing quote
                    ptr++; // Move past closing quote
                    // printf("[DEBUG] Found closing quote: %c\n", quote_type);
                }
                else
                {
                    printf("[ERROR] Missing closing quote for: %c\n", quote_type);
                }
            }
            else // Normal characters
            {
                token[i++] = *ptr++;
            }
        }

        token[i] = '\0'; // Null-terminate the token

        // Store the whole assignment as one token
        if (lexerbuf->count >= capacity)
        {
            capacity *= 2;
            lexerbuf->tokens = ft_realloc(lexerbuf->tokens, lexerbuf->count, capacity);
        }

        lexerbuf->tokens[lexerbuf->count++] = ft_strdup(token);

        // printf("[DEBUG] Stored Token: %s\n", token);
    }

    // printf("[DEBUG] Final Token Count: %zu\n", lexerbuf->count);
    // for (size_t j = 0; j < lexerbuf->count; j++)
        // printf("[DEBUG] lexerbuf->tokens[%zu]: %s\n", j, lexerbuf->tokens[j]);
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
