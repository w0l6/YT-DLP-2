#include <iostream>
#include <cstdlib>
#include <string>
#include <cstdio>
#include <iostream>


std::string executeCommand(const std::string& command) {
    std::string result;
    FILE* pipe = popen(command.c_str(), "r");
    if (pipe) {
        char buffer[128];
        while (!feof(pipe)) {
            if (fgets(buffer, 128, pipe) != nullptr)
                result += buffer;
        }
        pclose(pipe);
    }
    return result;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cout << "No link provided." << std::endl;
        return 1;
    }

    std::string link = argv[1];

    std::string downloadCommand = "yt-dlp -x --audio-format mp3 " + link;
    std::cout << "Downloading video..." << std::endl;
    std::string downloadOutput = executeCommand(downloadCommand);
    std::cout << "Download complete!" << std::endl;

    size_t start = downloadOutput.find("[ffmpeg] Destination: ");
    size_t end = downloadOutput.find(".mp3", start);
    if (start != std::string::npos && end != std::string::npos) {
        std::string filename = downloadOutput.substr(start + 22, end - start - 18);

        std::string convertCommand = "ffmpeg -i " + filename + ".mp4 " + filename + ".mp3";
        std::cout << "Converting to MP3..." << std::endl;
        executeCommand(convertCommand);
        std::cout << "Conversion complete!" << std::endl;
    }
    else {
        std::cout << "Unable to find downloaded file." << std::endl;
    }

    return 0;
}