#pragma once

#include "types.hpp"
#include "virtual-machine.hpp"
#include <cstdint>
#include <map>
#include <optional>
#include <string>
#include <utility>
#include <vector>

class Interpreter final {
public:
    explicit Interpreter(bool isDebugMode = false);

    void start();

private:
    void interpretInput(const std::string&);

    void handleRun();
    void handleSave() const;
    void handleLoad();
    void handleReset();
    void handleExit();

    void handleMovToReg();
    void handleMovToRam();
    void handleMovFromRam();
    void handleDumpreg();
    void handleAdd();
    void handleSub();
    void handleInc();
    void handleDec();
    void handleRet();

    bool m_isEnabled;
    VirtualMachine m_virtualMachine;
    std::pair<std::string, std::string> m_arguments;
    std::map<std::string, std::function<void()>> m_commandCallback;
    std::map<std::string, Register> m_registerCode;
};
