#include "jobs.hpp"

#include <iomanip>
#include <iostream>
#include <sys/wait.h>
#include <vector>

using namespace std;
struct BackgroundJob
{
    int job_number;
    pid_t pid;
    string status;
    string command;
};

vector<BackgroundJob> background_jobs;

int add_background_job(const string &command, pid_t pid)
{
    int highest_job_number = 0;
    for (const BackgroundJob &job : background_jobs)
        highest_job_number = max(highest_job_number, job.job_number);

    const int job_number = highest_job_number + 1;
    background_jobs.push_back({job_number, pid, "Running", command});
    return job_number;
}

void run_jobs()
{
    for (auto job = background_jobs.begin(); job != background_jobs.end(); job++)
    {
        if (waitpid(job->pid, nullptr, WNOHANG) == job->pid)
        {
            job->status = "Done";
        }
    }

    if (background_jobs.empty())
        return;

    const int most_recent = background_jobs.back().job_number;
    const int previous = background_jobs.size() > 1
                             ? background_jobs[background_jobs.size() - 2].job_number
                             : -1;

    for (const BackgroundJob &job : background_jobs)
    {
        cout << '[' << job.job_number << ']';
        if (job.job_number == most_recent)
            cout << '+';
        else if (job.job_number == previous)
            cout << '-';
        else
            cout << ' ';

        cout << "  " << left << setw(24) << job.status
             << job.command << right << endl;
    }

    for (auto job = background_jobs.begin(); job != background_jobs.end();)
    {
        if (job->status == "Done")
        {
            background_jobs.erase(job);
        }
        else
        {
            job++;
        }
    }
}