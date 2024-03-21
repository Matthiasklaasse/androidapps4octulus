#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cstdlib>

std::vector<std::string> split(const std::string &s, const std::string &delimiter)
{
    std::vector<std::string> tokens;
    std::size_t start = 0, end = 0;
    while ((end = s.find(delimiter, start)) != std::string::npos)
    {
        tokens.push_back(s.substr(start, end - start));
        start = end + delimiter.length();
    }
    tokens.push_back(s.substr(start));
    return tokens;
}

int main()
{
    system("platform-tools\\adb devices -l > Ctmp.txt");

    std::ifstream devicesFile("Ctmp.txt");
    std::string line;
    std::vector<std::string> devices;
    std::vector<std::string> unauthorizedDevices;

    bool firstLineSkipped = false;
    while (std::getline(devicesFile, line))
    {
        if (!firstLineSkipped)
        {
            firstLineSkipped = true;
            continue;
        }

        if (line.find("device") != std::string::npos)
        {
            devices.push_back(split(split(line, "model:")[1], " device:")[0]);
        }
        else if (line.find("unauthorized") != std::string::npos)
        {
            unauthorizedDevices.push_back(split(split(line, "model:")[1], " device:")[0]);
        }
    }
    devicesFile.close();

    if (devices.empty())
    {
        if (unauthorizedDevices.empty())
        {
            std::cout << "No devices found" << std::endl;
        }
        else
        {
            std::cout << "No devices found however we found unauthorized device(s):" << std::endl;
            for (const auto &device : unauthorizedDevices)
            {
                std::cout << device << std::endl;
            }
            std::cout << "Make sure you set up developer mode correctly and trust this PC" << std::endl;
        }
        return 0;
    }

    if (devices.size() == 1)
    {
        std::cout << "Found: " << devices[0] << std::endl;
        std::string response;
        std::cout << "Do you want to continue with this device? (y/n): ";
        std::cin >> response;
        if (response != "y")
        {
            return 0;
        }
    }
    else
    {
        std::cout << "Found multiple devices. Plug in only the target device with USB debugging enabled or disconnect your phone." << std::endl;
        return 0;
    }

    std::vector<std::string> apps;

    bool auroraInstalled = false;
    std::string pmOutput;
    std::string command = "platform-tools\\adb shell pm list packages -f > Ctmp.txt";
    system(command.c_str());
    std::ifstream pmOutputFile("Ctmp.txt");
    while (std::getline(pmOutputFile, pmOutput))
    {
        if (pmOutput.find("com.aurora.store") != std::string::npos)
        {
            std::cout << "Removing aurora..." << std::endl;
            std::string command = "platform-tools\\adb uninstall com.aurora.store";
            system(command.c_str());
            return 0;
        }
    }

    std::cout << "Aurora store is not installed" << std::endl;

    return 0;
}
