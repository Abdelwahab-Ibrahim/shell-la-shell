#include <iostream>
#include <string>

#include "builtins.hpp"
#include "utils.hpp"
#include "style.hpp"
#include "executor.hpp"
#include "lexer.hpp"
#include "declare.hpp"
#include "jobs.hpp"
#include "great.hpp"

using namespace std;
int main()
{
  // Flush after every std::cout / std:cerr
  std::cout << std::unitbuf;
  std::cerr << std::unitbuf;
  great();
  bool running = true;
  while (running)
  {
    std::cout << "$ ";
    string command;
    std::getline(std::cin, command);
    vector<Token> commands = tokenize(command);
    for (auto token = commands.begin(); token != commands.end();)
    {
      string token_value;

      if (token->type == TokenType::SUBSTITUTION)
      {
        token_value = replaceVariable(token->value);
      }
      else if (token->type == TokenType::SUBSTITUTIONNE)
      {
        token_value = replaceVariabolle(token->value);
      }
      else
      {
        ++token;
        continue;
      }

      if (token_value.empty())
      {
        token = commands.erase(token);
      }
      else
      {
        token->value = token_value;
        token->type = TokenType::WORD;
        ++token;
      }
    }

    if (commands.empty() || commands[0].type != TokenType::WORD)
    {
      run_jobs();
      continue;
    }

    if (hasPipe(commands))
    {
      vector<vector<Token>> commandGroups;
      vector<Token> currentCommand;

      for (const Token &token : commands)
      {
        if (token.type == TokenType::PIPE)
        {
          if (!currentCommand.empty())
          {
            commandGroups.push_back(currentCommand);
            currentCommand.clear();
          }
        }
        else
        {
          currentCommand.push_back(token);
        }
      }

      if (!currentCommand.empty())
      {
        commandGroups.push_back(currentCommand);
      }

      runPipeline(commandGroups);
    }
    else
    {
      execute_cmd(commands, running);
    }
  }
}
