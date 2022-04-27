//
// Created by dawid on 19.07.2021.
//  bdf_token: "fvsdmkfvh4232m42c4283ycm42938c4"

#pragma once

#include <iostream>
#include <vector>
#include <thread>
#include <algorithm>
#include "Module.h"


class Server {
    std::vector<Module *> modules;
    std::vector<std::thread> workers;

public:
    Server()=default;

    ~Server() {
        for (auto m: modules)
            delete m;
        modules.clear();
    };

    template<typename M, typename ...Args>
    M &add(Args &&... args) {
        modules.template emplace_back(new M(std::forward<Args>(args)...));
        return reinterpret_cast<M&>(*modules.at(modules.size()-1));
    }

    void launch() {
        for (auto &module: modules)
            workers.emplace_back(&Module::run, module);
    }

    void stop() {
        for (auto module: modules) {
            module->stop();
            std::cout << module->name() << " stopped\n";
        }

        for (auto &worker: workers) {
            worker.join();
            std::cout << "worker joined\n";
        }
    }

    friend Module;
};

