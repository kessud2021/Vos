/**
 * VSS-CO Shell
 *
 * GNU Bash-compatible shell implementation
 */

#include "shell.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pwd.h>

shell_state_t g_shell_state = {
    .exit_code = 0,
    .interactive = 0,
    .debug = 0,
    .histfile = NULL,
};

/**
 * main
 *
 * Shell entry point. Parses arguments and starts REPL or script.
 */
int main(int argc, char **argv) {
    const char *script = NULL;
    int opt;

    /* Parse arguments */
    while ((opt = getopt(argc, argv, "x:c:h")) != -1) {
        switch (opt) {
        case 'x':   /* Execute file */
            script = optarg;
            break;
        case 'c':   /* Execute command string */
            g_shell_state.exit_code = shell_execute_string(optarg);
            return g_shell_state.exit_code;
        case 'h':   /* Help */
            printf("VSS-CO Shell v1.0\n");
            printf("Usage: vss-shell [options] [file]\n");
            printf("  -c <cmd>      Execute command\n");
            printf("  -x <file>     Execute script file\n");
            return 0;
        default:
            fprintf(stderr, "Unknown option: %c\n", opt);
            return 1;
        }
    }

    /* Determine if interactive */
    if (isatty(STDIN_FILENO) && !script) {
        g_shell_state.interactive = 1;
    }

    /* Initialize */
    lineread_init();
    
    char *histfile = getenv("HISTFILE");
    if (histfile) {
        g_shell_state.histfile = strdup(histfile);
        history_load(histfile);
    } else {
        /* Default: ~/.bash_history */
        struct passwd *pw = getpwuid(getuid());
        if (pw) {
            asprintf(&g_shell_state.histfile, "%s/.bash_history", pw->pw_dir);
            history_load(g_shell_state.histfile);
        }
    }

    /* Run REPL or script */
    if (g_shell_state.interactive) {
        shell_repl();
    } else if (script) {
        g_shell_state.exit_code = shell_execute_file(script);
    } else if (argc > optind) {
        g_shell_state.exit_code = shell_execute_file(argv[optind]);
    } else {
        shell_repl();
    }

    /* Cleanup */
    if (g_shell_state.histfile) {
        history_save(g_shell_state.histfile);
        free(g_shell_state.histfile);
    }
    lineread_free();

    return g_shell_state.exit_code;
}

/**
 * shell_repl
 *
 * Read-eval-print loop for interactive shell
 */
void shell_repl(void) {
    char *line;
    char prompt[256];

    while (1) {
        /* Build prompt */
        const char *user = getenv("USER");
        const char *host = getenv("HOSTNAME");
        const char *pwd = getenv("PWD");
        
        if (!user) user = "user";
        if (!host) host = "localhost";
        if (!pwd) pwd = "/";

        snprintf(prompt, sizeof(prompt), "%s@%s:%s$ ", user, host, pwd);

        /* Read line */
        line = lineread(prompt);
        if (!line) {
            /* EOF (Ctrl-D) */
            printf("\nexit\n");
            break;
        }

        if (strlen(line) > 0) {
            history_add(line);
            g_shell_state.exit_code = shell_execute_string(line);
        }

        free(line);
    }
}

/**
 * shell_execute_string
 *
 * Parse and execute a single command string
 */
int shell_execute_string(const char *line) {
    ast_node_t *ast = parser_parse(line);
    if (!ast) {
        return 1;  /* Parse error */
    }

    int ret = executor_execute(ast);
    parser_free(ast);
    return ret;
}

/**
 * shell_execute_file
 *
 * Execute commands from a file (script)
 */
int shell_execute_file(const char *filename) {
    FILE *f = fopen(filename, "r");
    if (!f) {
        perror(filename);
        return 127;  /* File not found */
    }

    char line[4096];
    int ret = 0;

    while (fgets(line, sizeof(line), f)) {
        /* Skip empty lines and comments */
        if (line[0] == '\n' || line[0] == '#')
            continue;

        ret = shell_execute_string(line);
        if (ret != 0 && !getenv("SHELL_IGNORE_ERRORS")) {
            break;
        }
    }

    fclose(f);
    return ret;
}

/* Stub implementations (to be filled in) */
void shell_repl(void) {}
int shell_execute_string(const char *line) { return 0; }
int shell_execute_file(const char *filename) { return 0; }
int isatty(int fd) { return 0; }
char *lineread(const char *prompt) { return NULL; }
void lineread_init(void) {}
void lineread_free(void) {}
void history_load(const char *file) {}
void history_save(const char *file) {}
void history_add(const char *line) {}
ast_node_t *parser_parse(const char *input) { return NULL; }
void parser_free(ast_node_t *node) {}
int executor_execute(ast_node_t *node) { return 0; }
