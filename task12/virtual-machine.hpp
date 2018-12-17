#pragma once

#include "types.hpp"
#include <array>
#include <cstdint>
#include <functional>
#include <map>
#include <vector>

class VirtualMachine final {
public:
    explicit VirtualMachine(std::uint16_t ramSize = 8192, bool isDebugMode = false);

    void run();
    void save(const std::string& filePath) const;
    void load(const std::string& filePath);
    void reset();

    void writeMovToReg(Register destination, std::uint16_t value);
    void writeMovToRam(Address destination, Register source);
    void writeMovFromRam(Register destinationRegister, Address source);
    void writeDumpreg();
    void writeAdd(Register destination, Register source);
    void writeSub(Register destination, Register source);
    void writeInc(Register destination);
    void writeDec(Register destination);
    void writeRet();

private:
    void runMovToReg();
    void runMovToRam();
    void runMovFromRam();
    void runDumpreg();
    void runAdd();
    void runSub();
    void runInc();
    void runDec();
    void runRet();

    void updateIPAndCommandsCounter();

    bool m_isDebugMode;
    std::uint16_t m_ramSize;
    std::vector<std::uint8_t> m_ram;

    std::uint64_t m_lineNumber;
    bool m_isRunning;

    std::uint16_t m_a;
    std::uint16_t m_b;
    std::uint16_t m_ip;

    std::map<Register, std::uint16_t*> m_registerPointer;
    std::map<Register, std::string> m_registerName;
    std::map<Command, std::function<void()>> m_commandCallback;
};
