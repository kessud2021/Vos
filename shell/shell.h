/**
 * VSS-CO Shell header
 *
 * GNU Bash-compatible shell implementation
 */

#ifndef __SHELL_H__
#define __SHELL_H__

#include <stdint.h>
#include <stddef.h>

/* Token types */
typedef enum {
    TOKEN_EOF,
    TOKEN_WORD,
    TOKEN_NEWLINE,
    TOKEN_AND,          /* && */
    TOKEN_OR,           /* || */
    TOKEN_PIPE,         /* | */
    TOKEN_SEMICOLON,    /* ; */
    TOKEN_REDIRECT_IN,  /* < */
    TOKEN_REDIRECT_OUT, /* > */
    TOKEN_REDIRECT_APPEND, /* >> */
    TOKEN_LPAREN,       /* ( */
    TOKEN_RPAREN,       /* ) */
    TOKEN_IF,
    TOKEN_THEN,
    TOKEN_ELSE,
    TOKEN_FI,
    TOKEN_FOR,
    TOKEN_IN,
    TOKEN_DO,
    TOKEN_DONE,
    TOKEN_WHILE,
    TOKEN_CASE,
    TOKEN_ESAC,
} token_type_t;

typedef struct {
    token_type_t type;
    char *value;
    int line;
    int column;
} token_t;

/* AST Node types */
typedef enum {
    NODE_PROGRAM,
    NODE_PIPELINE,
    NODE_COMMAND,
    NODE_REDIRECT,
    NODE_IF,
    NODE_FOR,
    NODE_WHILE,
    NODE_CASE,
} node_type_t;

typedef struct ast_node {
    node_type_t type;
    union {
        struct {
            struct ast_node **children;
            int num_children;
        } program;
        struct {
            struct ast_node **commands;
            int num_commands;
        } pipeline;
        struct {
            char **args;
            int argc;
            struct redirect_t {
                int type;   /* REDIRECT_IN, REDIRECT_OUT, etc. */
                char *file;
            } *redirects;
            int num_redirects;
        } command;
        struct {
            struct ast_node *condition;
            struct ast_node *then_branch;
            struct ast_node *else_branch;
        } if_stmt;
        struct {
            char *var;
            char **values;
            int num_values;
            struct ast_node *body;
        } for_stmt;
    } data;
} ast_node_t;

/* Lexer */
typedef struct {
    const char *input;
    size_t pos;
    int line;
    int column;
} lexer_t;

lexer_t *lexer_create(const char *input);
void lexer_free(lexer_t *lexer);
token_t *lexer_next_token(lexer_t *lexer);

/* Parser */
ast_node_t *parser_parse(const char *input);
void parser_free(ast_node_t *node);

/* Executor */
int executor_execute(ast_node_t *node);

/* Built-ins */
int builtin_cd(int argc, char **argv);
int builtin_echo(int argc, char **argv);
int builtin_export(int argc, char **argv);
int builtin_exit(int argc, char **argv);
int builtin_history(int argc, char **argv);
int builtin_alias(int argc, char **argv);
int builtin_unalias(int argc, char **argv);

/* Environment */
char *env_get(const char *name);
void env_set(const char *name, const char *value);
void env_unset(const char *name);
char **env_get_all(int *count);

/* Jobs */
typedef struct {
    int job_id;
    pid_t pgid;
    char *command;
    int state;  /* Running, stopped, done */
} job_t;

int jobs_add(pid_t pgid, const char *command);
void jobs_remove(int job_id);
job_t *jobs_get(int job_id);
void jobs_list(void);

/* History */
void history_add(const char *line);
char *history_get(int index);
int history_size(void);
void history_save(const char *filename);
void history_load(const char *filename);

/* Line editing */
char *lineread(const char *prompt);
void lineread_init(void);
void lineread_free(void);

/* Completion */
char **completion_get(const char *word, int *count);
void completion_free(char **completions, int count);

/* Shell state */
typedef struct {
    int exit_code;
    int interactive;
    int debug;
    const char *histfile;
} shell_state_t;

extern shell_state_t g_shell_state;

#endif /* __SHELL_H__ */
