//
// Created by 宋博文 on 2024/6/26.
//

#include <fcntl.h>
#include "../basic.h"

/**
 * FD_CLOEXEC 是一个文件描述符（file descriptor）标志，它的作用是当一个新的程序通过 exec 系列函数（如 execve、execl、execvp 等）执行时，自动关闭文件描述符。这是一个常见的防止文件描述符泄漏的机制，尤其是在多进程环境中。
 */
TEST(fcntl_test, 测试FD_CLOEXEC标志符) {
    int r;
    int flags;
    const char *filename = "example.txt";
    int fd = open(filename, O_CREAT | O_RDWR | O_TRUNC);
    if (fd == -1) {
        perror("open");
        return;
    }
    do {
        r = fcntl(fd, F_GETFD);
    } while (r == -1 && errno == EINTR);

    flags = r | FD_CLOEXEC;

    do {
        r = fcntl(fd, F_SETFD, flags);
    } while (r == -1 && errno == EINTR);

    char buf[] = "hello world!\n";
    if (write(fd, buf, sizeof(buf)) == -1) {
        close(fd);
        exit(EXIT_FAILURE);
    }
    // 将文件指针重新设置到文件开头，防止读不到
    if (lseek(fd, 0, SEEK_SET) == -1) {
        perror("lseek");
        close(fd);
        exit(EXIT_FAILURE);
    }

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        close(fd);
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // 子进程
        printf("Child process (PID: %d)\n", getpid());

        // 在子进程中尝试访问文件描述符
        char buffer[100];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            printf("child read error, %s\n", strerror(errno));
        } else {
            buffer[bytesRead] = '\0';
            printf("Read from file in child process: %s\n", buffer);
        }
        // 在子进程中调用 exec 函数执行另一个程序
        execlp("ls", "ls", NULL);        // 如果 execl 函数返回，说明执行失败，正常执行不会往下走
        perror("execl");
        exit(EXIT_FAILURE);
    } else {
        // 父进程
        printf("Parent process (PID: %d)\n", getpid());

        // 等待子进程结束
        int status;
        waitpid(pid, &status, 0);
        if (WIFEXITED(status)) {
            printf("Child exited with status %d\n", WEXITSTATUS(status));
        }
        // 将文件指针重新设置到文件开头，防止读不到
        if (lseek(fd, 0, SEEK_SET) == -1) {
            printf("parent lseek error, %s\n", strerror(errno));
            close(fd);
            unlink(filename);
            exit(EXIT_FAILURE);
        }
        // 父进程继续尝试访问文件描述符
        char buffer[100];
        ssize_t bytesRead = read(fd, buffer, sizeof(buffer));
        if (bytesRead == -1) {
            printf("parent read error, %s\n", strerror(errno));
        } else {
            buffer[bytesRead] = '\0';
            printf("Read from file in parent process: %s\n", buffer);
        }
    }

    close(fd);
    if (unlink(filename) == 0) {
        printf("File deleted successfully\n");
    } else {
        perror("Error deleting file");
    }

}