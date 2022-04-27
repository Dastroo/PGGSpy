//
// Created by dawid on 11.10.2021.
//

#pragma once


class Module {
public:
    Module() = default;

    virtual ~Module() = default;

    virtual std::string name() = 0;

    virtual void run() = 0;

    virtual void stop() = 0;
};


