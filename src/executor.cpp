#include "executor.hpp"
#include "builtins.hpp"
#include "lexer.hpp"
#include "utils.hpp"
#include "redirection.hpp"
#include "style.hpp"
#include "declare.hpp"
#include "jobs.hpp"

#include <filesystem>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <csignal>
#include <cstdlib>
#include <sys/wait.h>
#include <algorithm>
// hello world
bool wait_job = true;
bool command_is_background_job()
{
    return !wait_job;
}

void run_external(vector<Token> &tokens)
{
    auto dont_wait = std::find_if(tokens.begin(), tokens.end(),
                                  [](Token &token)
                                  {
                                      return token.value == "&";
                                  });

    const bool requested_background = dont_wait != tokens.end();
    if (requested_background)
    {
        tokens.erase(dont_wait);
    }

    std::vector<std::string> arguments;
    arguments.reserve(tokens.size());
    for (const Token &token : tokens)
    {
        if (token.type == TokenType::WORD)
            arguments.push_back(token.value);
    }

    if (arguments.empty())
        return;

    std::vector<char *> argv;
    argv.reserve(arguments.size() + 1);
    for (std::string &argument : arguments)
        argv.push_back(const_cast<char *>(argument.c_str()));
    argv.push_back(nullptr);

    const string path = checkCMD(arguments[0]);
    const bool background_job = requested_background && !path.empty();
    wait_job = !background_job;

    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork");
        return;
    }

    if (pid == 0)
    {
        // Child process
        signal(SIGINT, SIG_DFL);
        int saved_stdout;
        int saved_stderr;
        if (!applyRedirections(tokens, saved_stdout, saved_stderr))
            _exit(1);
        if (path.empty())
        {
            cout << argv[0] << ERROR << ": command not found" << RESET << endl;
            _exit(EXIT_FAILURE);
        }

        execv(path.c_str(), argv.data());

        perror("execv");
        exit(EXIT_FAILURE);
    }

    // Parent process
    if (!background_job)
    {
        waitpid(pid, nullptr, 0);
        run_jobs();

        wait_job = true;
    }
    else
    {
        const int job_number = add_background_job(get_command(), pid);
        cout << "[" << GREEN << job_number << RESET << "] " << pid << endl;
        wait_job = true;
    }
}
