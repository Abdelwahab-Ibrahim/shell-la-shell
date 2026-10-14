#ifndef JOBS_HPP
#define JOBS_HPP

#include <string>
#include <sys/types.h>

int add_background_job(const std::string &command, pid_t pid);
void run_jobs();

#endif