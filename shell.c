#include <unistd.h>
#include <string.h>
#include <sys/syscall.h>
#include <sys/wait.h>

#define MAX_INPUT 256

// Syscall wrappers for minimalism
long sys_write(int fd, const char *buf, size_t count) {
    return syscall(SYS_write, fd, buf, count);
}

long sys_read(int fd, char *buf, size_t count) {
    return syscall(SYS_read, fd, buf, count);
}

long sys_fork() {
    return syscall(SYS_fork);
}

long sys_execv(const char *path, char *const argv[]) {
    return syscall(SYS_execve, path, argv, NULL);
}

long sys_waitid(int idtype, int id, void *infop, int options) {
    return syscall(SYS_waitid, idtype, id, infop, options);
}

long sys_exit(int status) {
    return syscall(SYS_exit, status);
}

// ASCII garden feature
void plant_tree() {
    const char *tree = "   ^\n  /|\\\n / | \\\n/  |  \\\n  ===\n";
    sys_write(1, tree, strlen(tree));
}

void plant_flower() {
    const char *flower = "  @\n /|\\\n/ | \\\n ===\n";
    sys_write(1, flower, strlen(flower));
}

void grow_plant(int stage) {
    const char *stages[] = {
        "  .\n  .\n ===\n",  // Seed
        "  *\n / \\\n ===\n", // Sprout
        "  ^\n /|\\\n/ | \\\n ===\n" // Full tree
    };
    if (stage < 0 || stage > 2) stage = 0;
    sys_write(1, stages[stage], strlen(stages[stage]));
}

void prompt() {
    const char *msg = "> ";
    sys_write(1, msg, strlen(msg));
}

int main() {
    char buf[MAX_INPUT];
    int stage = 0; // For grow command

    while (1) {
        prompt();
        long n = sys_read(0, buf, MAX_INPUT - 1);
        if (n <= 0) continue;
        buf[n - 1] = '\0'; // Remove newline

        if (strcmp(buf, "exit") == 0) {
            sys_exit(0);
        } else if (strcmp(buf, "plant tree") == 0) {
            plant_tree();
        } else if (strcmp(buf, "plant flower") == 0) {
            plant_flower();
        } else if (strcmp(buf, "grow") == 0) {
            grow_plant(stage);
            stage = (stage + 1) % 3; // Cycle through stages
        } else {
            // Attempt to execute command
            long pid = sys_fork();
            if (pid == 0) {
                char *args[] = {buf, NULL};
                sys_execv(buf, args);
                const char *err = "Command not found\n";
                sys_write(1, err, strlen(err));
                sys_exit(1);
            } else if (pid > 0) {
                sys_waitid(P_PID, pid, NULL, WEXITED);
            }
        }
    }
    return 0;
}
