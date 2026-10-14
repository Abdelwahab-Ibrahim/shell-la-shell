#include "utils.hpp"
#include <algorithm>
#include <cstdlib>
#include <sstream>
#include <unistd.h>
using namespace std;

bool hasPipe( vector<Token> &tokens)
{
    return find_if(tokens.begin(), tokens.end(),
                   [](const Token &token)
                   {
                       return token.type == TokenType::PIPE;
                   }) != tokens.end();
}

// Check if external command exists in PATH
string checkCMD(const string &cmd)
{
    const char *path_env = getenv("PATH");

    if (!path_env)
        return "";

    stringstream ss_path(path_env);
    string path;

    while (getline(ss_path, path, ':'))
    {
        string full_path = path + '/' + cmd;

        if (access(full_path.c_str(), X_OK) == 0)
            return full_path;
    }

    return "";
}