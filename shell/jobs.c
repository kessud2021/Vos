/**
 * Job control
 */

#include "shell.h"

int jobs_add(pid_t pgid, const char *command) { return 0; }
void jobs_remove(int job_id) {}
job_t *jobs_get(int job_id) { return NULL; }
void jobs_list(void) {}
