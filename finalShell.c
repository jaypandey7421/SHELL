#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>



// Array holding buitin commands name.
const char *commands[] = {
    "exit", "echo", "type", "pwd", "cd",
};

typedef struct {
  char name[100];
  char *args[100];
  int num_args;
} cmd_line;

typedef struct {
  char *paths[100];
  int num_paths;
} all_path;

void print_all(cmd_line cmd) {
  printf("Name: %s\n", cmd.name);
  for (int i = 0; i < cmd.num_args; i++) {
    printf("Arg[%d]: %s\n", i, cmd.args[i]);
  }
  printf("\n");
}

// Parse command name and its arguments
void parse_command(char *input, cmd_line *cmd) {
  // Get command name
  for (int i = 0; i < strlen(input); i++) {
    if (input[i] == ' ' || input[i] == '\n' || input[i] == '\0') {
      cmd->name[i] = '\0';
      break;
    }
    cmd->name[i] = input[i];
  }

  // Get command arguments
  char *args_str = input + strlen(cmd->name) + 1;
  cmd->num_args = 0;
  for (int i = 0; i < strlen(args_str); i++) {
    if (args_str[i] == ' ' || args_str[i] == '\n' || args_str[i] == '\0') {
      cmd->args[cmd->num_args] = (char *)malloc(i + 1);
      strncpy(cmd->args[cmd->num_args], args_str, i);
      cmd->args[cmd->num_args][i] = '\0';
      cmd->num_args++;
      args_str += i + 1;
      i = 0;
    }
  }
}

// Prints all of the args(used in ECHO)
void print_args(cmd_line cmd, int num_args) {
  for (int i = 0; i < num_args; i++) {
    printf("%s", cmd.args[i]);
    if (i != num_args - 1)
      printf(" ");
  }
  printf("\n");
}

// Extract OS PATHS, count number of paths, and store it to STRUCTURE variable
void get_paths(char *path, all_path *paths) {
  paths->num_paths = 0;
  for (int i = 0; i < strlen(path); i++) {
    if (path[i] == ':') {
      paths->paths[paths->num_paths] = (char *)malloc(i + 1);
      strncpy(paths->paths[paths->num_paths], path, i);
      paths->paths[paths->num_paths][i] = '\0';
      paths->num_paths++;
      path += i + 1;
      i = 0;
    }
  }
  paths->paths[paths->num_paths] = (char *)malloc(strlen(path) + 1);
  strcpy(paths->paths[paths->num_paths], path);
  paths->num_paths++;
}

void print_paths(all_path paths) {
  for (int i = 0; i < paths.num_paths; i++) {
    if (paths.paths[i] == NULL)
      break;
    printf("path %d: %s\n", i, paths.paths[i]);
  }
}

// Checks for executable PATHs and return the PATH of that Executable
char *check_path(char *program, all_path paths) {
  for (int i = 0; i < paths.num_paths; i++) {
    char *cmd_path =
        (char *)malloc(strlen(paths.paths[i]) + strlen(program) + 2);
    strcpy(cmd_path, paths.paths[i]);
    strcat(cmd_path, "/");
    strcat(cmd_path, program);
    if (access(cmd_path, F_OK) != -1)
      return cmd_path;
  }
  return NULL;
}

//Checks weather the coomand is builtin, executable or not exits and return 1,2 and 0 respectively.
int check_cmd(char *cmd, all_path paths, char *msg, char *path) {
  // Checks for builtin commands and return 1 if found
  for (int i = 0; i < sizeof(commands) / sizeof(commands[0]); i++) {
    if (strcmp(cmd, commands[i]) == 0) {
      sprintf(msg, "%s is a shell builtin", cmd);
      return 1;
    }
  }

  // Store Executable PATH to msg and retrn 2
  char *cmd_path = check_path(cmd, paths);
  if (cmd_path != NULL) {
    sprintf(msg, "%s is %s", cmd, cmd_path);
    strcpy(path, cmd_path);
    return 2;
  }

  // If command is neither in builtins nor in Executable, returns 0.
  sprintf(msg, "%s: not found", cmd);
  return 0;
}

// Executes the command using system call
void execute_command(cmd_line cmd, char *path) {
  char run[300];
  strcpy(run, path);
  strcat(run, " ");

  for (int i = 0; i < cmd.num_args; i++) {
    strcat(run, cmd.args[i]);
    if (i != cmd.num_args - 1)
      strcat(run, " ");
  }
  
  if(system(run) < 0){
    printf("Command execution failed.\n");
  };
}

int main() {
  char *commands[] = {"exit", "echo", "type"};
  char *path = getenv("PATH");
  char *home_dir = getenv("HOME");
  all_path paths;

  get_paths(path, &paths);

  while (1) {
    char cwd[1024];

    if(getcwd(cwd, sizeof(cwd)) == NULL) printf("Error: PWD erro!\n");
    
    printf("\033[32m~%s\033[0m$ ",cwd+strlen(home_dir));
    fflush(stdout);

    // Wait for user input
    char input[100];
    if(fgets(input, 100, stdin) == NULL) continue;;
    cmd_line cmd;
    parse_command(input, &cmd);

    char msg[100];
    char path_c[100];

    if (strcmp(cmd.name, "exit") == 0) {
      return 0;
    } else if (strcmp(cmd.name, "echo") == 0) {
      print_args(cmd, cmd.num_args);
    } else if (strcmp(cmd.name, "type") == 0) {
      check_cmd(cmd.args[0], paths, msg, path_c);
      printf("%s\n", msg);
    } else if (strcmp(cmd.name, "pwd") == 0) {
      // char cwd[1024];
      // getcwd(cwd, sizeof(cwd));
      printf("%s\n", cwd);
    }else if (strncmp(input, "~", 1) == 0) {
      char *home = getenv("HOME");
      if (chdir(home))
      printf("error home directory\n");
    }else if (strcmp(cmd.name, "cd") == 0) {
      if (chdir(cmd.args[0]) != 0)
        printf("%s: No such file or directory\n", cmd.args[0]);
    } else if (check_cmd(cmd.name, paths, msg, path_c) == 2) {
      execute_command(cmd, path_c);
    } else
      printf("%s: command not found\n", cmd.name);
  }

  return 0;
}