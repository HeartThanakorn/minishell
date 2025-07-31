#include "minishell.h"

int main(void)
{
    char *test_pwd[] = {"pwd", NULL};
    char *test_echo[] = {"echo", "-n", "hello", "world", NULL};
    char *test_echo2[] = {"echo", "hello", "world", NULL};
    char *test_env[] = {"env", NULL};
    char *test_cd[] = {"cd", "/tmp", NULL};
    char *test_cd_home[] = {"cd", NULL};
    char *test_export[] = {"export", "TEST_VAR=hello", NULL};
    char *test_unset[] = {"unset", "TEST_VAR", NULL};
    char *test_exit[] = {"exit", NULL};

    printf("Testing pwd:\n");
    execute_builtin(test_pwd);

    printf("\nTesting echo -n:\n");
    execute_builtin(test_echo);

    printf("\nTesting echo:\n");
    execute_builtin(test_echo2);

    printf("\nTesting cd /tmp:\n");
    execute_builtin(test_cd);
    execute_builtin(test_pwd);

    printf("\nTesting cd (home):\n");
    execute_builtin(test_cd_home);
    execute_builtin(test_pwd);

    printf("\nTesting export:\n");
    execute_builtin(test_export);

    printf("\nTesting env (should show TEST_VAR):\n");
    execute_builtin(test_env);

    printf("\nTesting unset:\n");
    execute_builtin(test_unset);

    printf("\nTesting env (TEST_VAR should be gone):\n");
    execute_builtin(test_env);

    printf("\nTesting exit:\n");
    execute_builtin(test_exit);

    return (0);
}