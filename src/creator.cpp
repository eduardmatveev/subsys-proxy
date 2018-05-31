#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cstring>

void write_uint16(std::vector<uint8_t> &buffer, uint16_t value)
{
    buffer.resize(buffer.size() + sizeof(value));
    std::memcpy(buffer.data() + buffer.size() - sizeof(value), &value, sizeof(value));
}

void write_string(std::vector<uint8_t> &buffer, const std::string &value)
{
    write_uint16(buffer, value.length());
    buffer.resize(buffer.size() + value.length());
    std::memcpy(buffer.data() + buffer.size() - value.length(), value.data(), value.length());
}

int main(int argc, char *argv[])
{
    auto quiet = false;
    if (argc < 4)
    {
        std::cout << "Usage" << std::endl;
        std::cout << std::endl;
        std::cout << "creator src dst cmd [options]" << std::endl;
        std::cout << "-f <replace-from-host> <replacement-from-host>" << std::endl;
        std::cout << "-t <replace-to-host> <replacement-to-host>" << std::endl;
        std::cout << "-l <log>" << std::endl;
        std::cout << "-i - input" << std::endl;
        std::cout << "-q - quiet" << std::endl;
        return 0;
    }
    std::ifstream src(argv[1], std::ios::binary);
    std::ofstream dst(argv[2], std::ios::binary);
    dst << src.rdbuf();
    src.close();
    std::string cmd = argv[3];

    std::vector<uint8_t> buffer;
    write_string(buffer, cmd);

    std::string log;
    std::vector<std::pair<std::string, std::string>> tohost;
    std::vector<std::pair<std::string, std::string>> fromhost;
    uint16_t input = 0;
    for (int i = 4; i < argc; ++i)
    {
        std::string cmd = argv[i];
        if (cmd == "-f")
        {
            fromhost.push_back(std::make_pair(argv[i + 1], argv[i + 2]));
            i += 2;
            continue;
        }
        if (cmd == "-t")
        {
            tohost.push_back(std::make_pair(argv[i + 1], argv[i + 2]));
            i += 2;
            continue;
        }
        if (cmd == "-l")
        {
            log = argv[i + 1];
            i += 1;
            continue;
        }
        if (cmd == "-i")
        {
            input = 1;
            continue;
        }
        if (cmd == "-q")
        {
            quiet = true;
            continue;
        }
    }
    if(!quiet) std::cout << "CMD:" << cmd << std::endl;
    if(!quiet) std::cout << "FROMHOST:" << std::endl;
    write_uint16(buffer, fromhost.size());
    for (const auto &pair : fromhost)
    {
        write_string(buffer, pair.first);
        write_string(buffer, pair.second);
        if(!quiet) std::cout << pair.first << ' ' << pair.second << std::endl;
    }

    if(!quiet) std::cout << "TOHOST:" << std::endl;
    write_uint16(buffer, tohost.size());
    for (const auto &pair : tohost)
    {
        write_string(buffer, pair.first);
        write_string(buffer, pair.second);
        if(!quiet) std::cout << pair.first << ' ' << pair.second << std::endl;
    }
    if (!quiet && log.size() > 0)
    {
        std::cout << "LOG:" << std::endl;
        std::cout << log << std::endl;
    }
    write_string(buffer, log);
    write_uint16(buffer, input);
    write_uint16(buffer, buffer.size() + 2);
    dst.seekp(0, std::ios_base::end);
    dst.write(reinterpret_cast<const char *>(buffer.data()), buffer.size());

    if(!quiet) std::cout << "File '" << argv[2] << "' created" << std::endl;
    return 0;
}
