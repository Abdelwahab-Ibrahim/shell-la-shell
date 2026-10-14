#ifndef EXECUTOR_HPP
#define EXECUTOR_HPP

#include "lexer.hpp"

#include <string>
#include <vector>

void run_external( vector<Token> &tokens);

void runPipeline( vector<vector<Token>> &commands);

bool command_is_background_job();

#endif
