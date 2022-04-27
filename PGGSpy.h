//
// Created by dawid on 20.01.2022.
// WARNING: Linux only NO support for windows using linux system commands

#pragma once

#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <atomic>
#include <thread>
#include <filesystem>
#include <python3.8/Python.h>

#include <libxml/xmlmemory.h>
#include <libxml/HTMLparser.h>
#include <libxml/xpath.h>

#include "SDL2/SDL_mixer.h"

#include "Module.h"
#include "Audio.h"

class PGGSpy : public Module {
    const std::string file_name = "page.html";
    const std::string page_name = "https://sklep.pgg.pl";

    std::atomic<bool> running = true;
    std::atomic<int> num_available = 0;
    std::atomic<int> coal_types = 5;

    //  AUDIO STUFF
    Mix_Chunk *alert;
    Audio audio;

public:
    PGGSpy() {
        alert = audio.loadSound("alert.mp3");
        if (!std::filesystem::exists("available_pages"))
            std::filesystem::create_directory("available_pages");
        if (!std::filesystem::exists("invalid_pages"))
            std::filesystem::create_directory("invalid_pages");
    };

    ~PGGSpy() override {
        Mix_FreeChunk(alert);
    };

    std::string name() override {
        return "pggspy";
    }

    void run() override {
        while (running) {
            update();
            if (num_available != 0) {
                sound_signal();
                std::cerr << date() << ": number of available coal types: " << num_available << '\n';
                copy_file("available_pages/" + date() + ".html");
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }

    void stop() override {
        running = false;
    }

private:
    void update() {
        download_page();

        std::ifstream file(file_name);
        if (!file.is_open()) {
            std::cerr << "PGGSpy::update -> check get.js file, must be in same file as binary\n";
            exit(1);
        }

        if (is_empty(file)) {
            num_available = 0;
            std::cout << "empty html\n";
        } else if (is_valid(file))
            find_text_count(file, "Chwilowy brak towaru");
        else {
//            sound_signal();
//            std::cerr << date() << ": invalid html\n";
            copy_file("invalid_pages/"+ date() + ".html");
        }

        file.close();
    }

    /**
     * @brief searches how many times is given fraze mentioned in given file
     * @param file
     * @param fraze
     * @return number of times mentioned
     */
    void find_text_count(std::ifstream &file, const std::string &fraze) {
        int result = 0;

        std::string line;
        while (getline(file, line))
            if (line.find(fraze) != std::string::npos)
                result++;

        num_available = coal_types - result;
    }

    static std::string date() {
        // current date/time based on current system
        time_t now = time(nullptr);

        // convert now to string form
        std::string dt = ctime(&now);

        dt.pop_back();
        return dt;
    }

    void sound_signal() {
        audio.playSound(alert);
    }

    static bool is_empty(std::ifstream &file) {
        return file.peek() == std::ifstream::traits_type::eof();
    }

    static bool is_valid(std::ifstream &file) {
        std::string line;
        std::string fraze = "<title>Ekogroszek workowany | Strona główna - Sklep PGG</title>";

        if (file.tellg() > 0) {
            std::cout << file.tellg() << ", file.tellg() > 0\n";
            file.seekg(0);
        }

        for (int i = 0; i < 10 && !file.eof(); ++i) {
            getline(file, line);
            if (line.find(fraze) != std::string::npos)
                return true;
        }

        return false;
    }

    static void copy_file(const std::string &copy_name) {
        std::filesystem::copy_file("page.html", copy_name, std::filesystem::copy_options::overwrite_existing);
    }

    void download_page() {
        //  TODO: change to get.py
        std::string command = "phantomjs get.js \"" + page_name + "\" > " + file_name + " 2>/dev/null";
        system(command.c_str());

        //Initialize the python instance
        /*Py_Initialize();

        //Run a simple file
        FILE* PScriptFile = fopen("get.py", "r");
        if(PScriptFile){
            PyRun_SimpleFile(PScriptFile, "get.py");
            fclose(PScriptFile);
        }

        //Close the python instance
        Py_Finalize();*/
    }

    static std::string read_file(std::ifstream &file) {
        std::string content;
        file >> content;
        return content;
    }
};


