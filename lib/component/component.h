#pragma once

class Component {
public:
    virtual void tick(int deltaMs) = 0;
    virtual void initialize() = 0;
    virtual ~Component() {}
};
