# minishell

Minishell Project Plan Summary (for a Two-Person Team)
The Minishell project involves creating a simple shell, similar to Bash, and requires extensive knowledge of processes and file descriptors. It must be written in C, adhere to specific coding norms, and ensure proper memory management.

Here's a breakdown of how to approach it:

## Phase 1: Initial Understanding & Setup (Everyone)

1. Thorough Review: Both team members should read and understand the entire minishell.pdf document, paying close attention to "Common Instructions" and "Mandatory Part" requirements.

2. Shell Fundamentals: Grasp core concepts like processes, file descriptors, redirections, pipes, environment variables, and signals.

3. Allowed Functions: Review the list of external functions permitted for use.

4. Coding Standards: Agree on a consistent coding style from the start, as norm errors will result in a 0 score.

## Phase 2: Project Setup & Makefile (Collaborative / One Lead)

1. Git Repository: Set up the shared Git repository.

2. Basic File Structure: Create initial .c and .h files, and a libft/ folder if using your libft.

3. Makefile: Create a basic Makefile with $(NAME), all, clean, fclean, and re rules. Ensure it compiles with -Wall, -Wextra, -Werror flags using cc , and properly compiles libft if included.

## Phase 3: Work Division (Individual Focus)

### Person 1: Focus on Parser and Executor Core

1. Input & History:

- Implement reading commands using readline and managing history with add_history.

- Handle ctrl-C to display a new prompt on a new line, ctrl-D to exit the shell, and ctrl-\ to do nothing in interactive mode.

2. Lexer/Tokenizer (Basic): Develop functions to break input strings into basic tokens (commands, arguments).

3. Parser (Basic): Create data structures (e.g., structs, linked lists) to represent parsed commands and organize tokens for execution.

4. Process Management & Execution (Basic):

- Implement basic execution of external commands (e.g., ls, cat) without complex arguments or redirections.

- Use fork, execve, and waitpid (or wait, wait3, wait4) for process creation and waiting.

- Search and launch executables based on the PATH variable or relative/absolute paths.

### Person 2: Focus on Built-in Commands, Redirections, and Signals

1. Built-in Commands:

- Implement the mandated built-in commands: 

echo (with -n option), cd (relative/absolute path only), pwd (no options), export (no options), unset (no options), env (no options/arguments), exit (no options). These must run in the shell's process.

2. I/O Redirections:

- Implement input redirection (<), output redirection (>), append output redirection (>>), and heredoc (<<<). Note that heredoc does not need to update history.

- Utilize functions like open, close, dup, dup2 for handling file descriptors.

3. Pipes:

- Implement pipes (|) to connect the output of one command to the input of the next. Use the pipe function.

4. Signal Handling:

- Set up signal handlers for ctrl-C, ctrl-D, and ctrl-\.

- Use at most one global variable to indicate a received signal, storing only the signal number, without accessing main data structures.


## Phase 4: Collaboration & Testing (Ongoing)

1. Interface Definition: Agree on the data structures and interfaces for passing information between the parser and executor.

2. Frequent Merges: Merge code branches regularly to minimize complex conflicts.

3. Communication: Hold daily stand-ups to discuss progress, blockers, and daily plans.

4. Independent Testing:

- Person 1 Tests: Test prompt display, history functionality, basic tokenization, simple external command execution (e.g., ls, cat), and proper process waiting.

- Person 2 Tests: Test basic built-in commands (echo, pwd, exit), signal handling (ctrl-C, ctrl-\ behavior), and simple output redirection (echo hello > file.txt).

5. Memory Management: Ensure all heap-allocated memory is freed properly to avoid leaks (except for readline() which is exempt ).

6. Bonus Part: Focus on the mandatory part first, as the bonus part will only be evaluated if the mandatory part is perfectly implemented and functions without issues.

By following this approach, you can work independently on your assigned components, ensuring they are functional before integrating them, which simplifies the overall development process.
