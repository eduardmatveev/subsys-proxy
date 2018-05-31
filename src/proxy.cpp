#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <string>
#include <sstream>
#include <iostream>
#include <thread>
#include <fstream>
#include <vector>
#include <fcntl.h>
#include <termios.h>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

#define BUFFER_SIZE 1024 * 1024

uint16_t read_uint16(char *buffer, uint16_t &offset)
{
    uint16_t res;
    std::memcpy(&res, buffer + offset, sizeof(res));
    offset += sizeof(res);
    return res;
}

std::string read_string(char *buffer, uint16_t &offset)
{
    std::string result;
    auto length = read_uint16(buffer, offset);
    if (length > 0)
    {
        result.reserve(length);
        result.append(reinterpret_cast<const char *>(&buffer[offset]), length);
        offset += length;
    }
    return result;
}

void replace_all(std::string &str, const std::string &from, const std::string &to)
{
    size_t start_pos = 0;
    while ((start_pos = str.find(from, start_pos)) != std::string::npos)
    {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length();
    }
}

std::string log_file;

void log(const std::string &title, const std::string &text)
{
    if (log_file.size() > 0)
    {
        std::ofstream logging;
        logging.open(log_file, std::ios_base::app);
        logging << title << ':' << text << std::endl;
        logging.close();
    }
}

void log(const std::string &title, int text)
{
    if (log_file.size() > 0)
    {
        std::ofstream logging;
        logging.open(log_file, std::ios_base::app);
        logging << title << ':' << text << std::endl;
        logging.close();
    }
}

fd_set readset;
char buffer[BUFFER_SIZE];

void replace_fd(const std::string &name, int from, int to, const std::vector<std::pair<std::string, std::string>> &map, struct timeval *tv)
{
    FD_ZERO(&readset);
    FD_SET(from, &readset);
    select(from + 1, &readset, NULL, NULL, tv);
    if (FD_ISSET(from, &readset))
    {
        do
        {
            auto size = read(from, buffer, BUFFER_SIZE);
            if (size > 0)
            {
                std::string out(buffer, size);
                std::string orig = out;
                for (const auto &pair : map)
                    replace_all(out, pair.first, pair.second);
                write(to, out.c_str(), out.size());
                if (orig != out)
                {
                    log(name, orig);
                    log(name, out);
                }
                else
                {
                    log(name, out);
                }
            }
            else
            {
                break;
            }
        }
        while (tv == NULL);
    }
}

//void signal_sigint(int signum)
//{
//    std::cout << "Signal " << signum << std::endl;
//    exit(signum);
//}

int main(int argc, char *argv[])
{
//    for (auto signum = SIGHUP; signum <= SIGUSR2; ++signum)
//    {
//        if (signum != 20)
//            signal(signum, signal_sigint);
//    }
    std::string cmd;
    std::string args;
    std::vector<std::pair<std::string, std::string>> fromhost;
    std::vector<std::pair<std::string, std::string>> tohost;
    auto input = false;
    {
        std::ifstream stream(argv[0], std::ios::binary);
        stream.seekg(std::ios_base::end);
        char size_buffer[2];
        stream.seekg(-2, std::ios_base::end);
        stream.read(size_buffer, 2);
        uint16_t offset = 0;
        uint16_t size = read_uint16(size_buffer, offset);
        char buffer[size];
        stream.seekg(-size - 2, std::ios_base::end);
        stream.read(buffer, size);

        cmd = read_string(buffer, offset);

        {
            uint16_t n = read_uint16(buffer, offset);
            fromhost.reserve(n);
//            std::cout << "FROMHOST:" << std::endl;
            for (uint16_t i = 0; i < n; ++i)
            {
                auto first = read_string(buffer, offset);
                auto second = read_string(buffer, offset);
                fromhost.push_back(std::make_pair(first, second));
//                std::cout << first << ' ' << second << std::endl;
            }
        }
        {
            uint16_t n = read_uint16(buffer, offset);
            tohost.reserve(n);
//            std::cout << "TOHOST:" << std::endl;
            for (uint16_t i = 0; i < n; ++i)
            {
                auto first = read_string(buffer, offset);
                auto second = read_string(buffer, offset);
                tohost.push_back(std::make_pair(first, second));
//                std::cout << first << ' ' << second << std::endl;
            }
        }
        log_file = read_string(buffer, offset);
        input = read_uint16(buffer, offset) != 0;
    }

    {
        char buffer[FILENAME_MAX];
        getcwd(buffer, FILENAME_MAX);
        std::string pwd(buffer);
        for (int i = 1; i < argc; ++i)
        {
            if (i != 1)
                args += " ";
            args += "'";
            args += argv[i];
            args += "'";
        }
        for (const auto &pair : fromhost)
        {
            replace_all(pwd, pair.first, pair.second);
            replace_all(args, pair.first, pair.second);
        }
        replace_all(cmd, "#PWD#", pwd);
        replace_all(cmd, "#ARGS#", args);

        log("PWD", pwd);
        log("CMD", cmd);
    }
    if (input)
    {

        int fdoutput[2];
        pipe(fdoutput);
        int fdinput[2];
        pipe(fdinput);
        int fderror[2];
        pipe(fderror);
        pid_t pid;

        if ((pid = fork()) == -1)
        {
            log("ERROR", "fork");
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            close(fdoutput[0]);
            dup2(fdoutput[1], 1);
            close(fdinput[1]);
            dup2(fdinput[0], 0);
            close(fderror[0]);
            dup2(fderror[1], 2);

            if (execl("/bin/sh", "cmd", "-c", cmd.c_str(), NULL) == -1)
            {
                log("ERROR", "execl");
                perror("execl");
            }
            return 0;
        }

        auto fpoutput = fdopen(fdoutput[0], "r");
        close(fdoutput[1]);

        auto fpinput = fdopen(fdinput[1], "w");
        close(fdinput[0]);

        auto fperror = fdopen(fderror[0], "r");
        close(fderror[1]);

        auto fdoutput_from = fileno(fpoutput);
        auto fderror_from = fileno(fperror);
        auto fdinput_to = fileno(fpinput);

        struct timeval tv { 0, 100 };
        int stat;
        while (true)
        {
            replace_fd("OUTPUT", fdoutput_from, STDOUT_FILENO, tohost, &tv);
            replace_fd("ERROR", fderror_from, STDERR_FILENO, tohost, &tv);
            replace_fd("INPUT", STDIN_FILENO, fdinput_to, fromhost, &tv);

            if (waitpid(pid, &stat, WNOHANG | WUNTRACED) == -1 && errno != EINTR)
            {
                break;
            }
        }
        replace_fd("OUTPUT", fdoutput_from, STDOUT_FILENO, tohost, NULL);
        replace_fd("ERROR", fderror_from, STDERR_FILENO, tohost, NULL);
        log("EXIT", stat);
        return stat;
    }
    else
    {
        int fdoutput[2];
        pipe(fdoutput);
        int fderror[2];
        pipe(fderror);
        pid_t pid;

        if ((pid = fork()) == -1)
        {
            log("ERROR", "fork");
            perror("fork");
            return 1;
        }

        if (pid == 0)
        {
            close(fdoutput[0]);
            dup2(fdoutput[1], 1);
            close(fderror[0]);
            dup2(fderror[1], 2);

            if (execl("/bin/sh", "cmd", "-c", cmd.c_str(), NULL) == -1)
            {
                log("ERROR", "execl");
                perror("execl");
            }
            return 0;
        }

        auto fpoutput = fdopen(fdoutput[0], "r");
        close(fdoutput[1]);

        auto fperror = fdopen(fderror[0], "r");
        close(fderror[1]);

        std::thread output([&]
        {
            auto fdoutput_from = fileno(fpoutput);
            char result[BUFFER_SIZE];
            while (true)
            {
                auto size = read(fdoutput_from, result, BUFFER_SIZE);
                if(size > 0)
                {
                    std::string out(result, size);
                    for (const auto& pair : tohost)
                    replace_all(out, pair.first, pair.second);
                    write(STDOUT_FILENO, out.c_str(), out.size());
                    log("OUTPUT", out);
                }
            }
        });
        output.detach();

        std::thread error([&]
        {
            auto fderror_from = fileno(fperror);
            char result[BUFFER_SIZE];
            while (true)
            {
                auto size = read(fderror_from, result, BUFFER_SIZE);
                if(size > 0)
                {
                    std::string out(result, size);
                    for (const auto& pair : tohost)
                    replace_all(out, pair.first, pair.second);
                    write(STDERR_FILENO, out.c_str(), out.size());
                    log("ERROR", out);
                }
            }
        });
        error.detach();

        int stat;
        while (waitpid(pid, &stat, 0) == -1)
        {
            if (errno != EINTR)
            {
                break;
            }
        }
        log("EXIT", stat);
        return stat;
    }
    return 0;
}
