#include "virtual-machine.hpp"
#include "constants.hpp"
#include <cstring>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <stdexcept>

VirtualMachine::VirtualMachine(std::uint16_t ramSize, bool isDebugMode)
    : m_isDebugMode { isDebugMode }
    , m_ramSize { ramSize }
    , m_ram(m_ramSize, 0)
    , m_a { 0 }
    , m_b { 0 }
    , m_ip { 0 }
{
    m_registerPointer[RegisterCode::A] = &m_a;
    m_registerPointer[RegisterCode::B] = &m_b;

    m_registerName[RegisterCode::A] = "a";
    m_registerName[RegisterCode::B] = "b";

    m_commandCallback[CommandCode::MovToReg] = [this] { runMovToReg(); };
    m_commandCallback[CommandCode::MovToRam] = [this] { runMovToRam(); };
    m_commandCallback[CommandCode::MovFromRam] = [this] { runMovFromRam(); };
    m_commandCallback[CommandCode::Dumpreg] = [this] { runDumpreg(); };
    m_commandCallback[CommandCode::Add] = [this] { runAdd(); };
    m_commandCallback[CommandCode::Sub] = [this] { runSub(); };
    m_commandCallback[CommandCode::Inc] = [this] { runInc(); };
    m_commandCallback[CommandCode::Dec] = [this] { runDec(); };
    m_commandCallback[CommandCode::Ret] = [this] { runRet(); };
}

void VirtualMachine::run()
{
    m_ip = 0;
    m_lineNumber = 0;
    m_isRunning = true;
    while (m_isRunning) {
        if (m_commandCallback.count(m_ram[m_ip])) {
            auto callback = m_commandCallback[m_ram[m_ip]];
            updateIPAndCommandsCounter();
            callback();
        } else
            throw std::runtime_error("error <VirtualMachine>: unknown command");
    }
    m_ip = 0;
}

void VirtualMachine::save(const std::string& filePath) const
{
    std::ofstream fileStream { filePath };
    fileStream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    for (auto&& value : m_ram)
        fileStream << value;
}

void VirtualMachine::load(const std::string& filePath)
{
    reset();
    std::ifstream fileStream { filePath };
    fileStream.exceptions(std::ofstream::failbit | std::ofstream::badbit);
    for (auto&& value : m_ram)
        fileStream >> value;
}

void VirtualMachine::reset()
{
    m_ram.clear();
    m_ip = 0;
    m_a = 0;
    m_b = 0;
}

void VirtualMachine::writeMovToReg(Register destination, std::uint16_t value)
{
    m_ram[m_ip++] = CommandCode::MovToReg;
    m_ram[m_ip++] = destination;
    *reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip) = value;
    m_ip += 2;
}

void VirtualMachine::writeMovToRam(Address destination, Register source)
{
    m_ram[m_ip++] = CommandCode::MovToRam;
    *reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip) = destination;
    m_ip += 2;
    m_ram[m_ip++] = source;
}

void VirtualMachine::writeMovFromRam(Register destination, Address source)
{
    m_ram[m_ip++] = CommandCode::MovFromRam;
    m_ram[m_ip++] = destination;
    *reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip) = source;
    m_ip += 2;
}

void VirtualMachine::writeDumpreg()
{
    m_ram[m_ip++] = CommandCode::Dumpreg;
}

void VirtualMachine::writeAdd(Register destination, Register source)
{
    m_ram[m_ip++] = CommandCode::Add;
    m_ram[m_ip++] = (destination << 4) | source;
}

void VirtualMachine::writeSub(Register destination, Register source)
{
    m_ram[m_ip++] = CommandCode::Sub;
    m_ram[m_ip++] = (destination << 4) | source;
}

void VirtualMachine::writeInc(Register destination)
{
    m_ram[m_ip++] = CommandCode::Inc;
    m_ram[m_ip++] = destination;
}

void VirtualMachine::writeDec(Register destination)
{
    m_ram[m_ip++] = CommandCode::Dec;
    m_ram[m_ip++] = destination;
}

void VirtualMachine::writeRet()
{
    m_ram[m_ip++] = CommandCode::Ret;
}

void VirtualMachine::runMovToReg()
{
    auto destinationRegisterCode = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[destinationRegisterCode];
    std::uint16_t* value = reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip);
    m_ip += 2;
    *(destinationRegisterPointer) = *value;
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: mov_to_reg "
                  << m_registerName[destinationRegisterCode] << ", " << *value << std::endl;
    }
}

void VirtualMachine::runMovToRam()
{
    auto destinationAddress = reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip);
    m_ip += 2;
    auto sourceRegisterCode = m_ram[m_ip++];
    auto sourceRegisterPointer = m_registerPointer[sourceRegisterCode];
    *reinterpret_cast<std::uint16_t*>(m_ram.data() + *destinationAddress) = *sourceRegisterPointer;
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: mov_to_ram ["
                  << destinationAddress << "], " << m_registerName[sourceRegisterCode] << std::endl;
    }
}

void VirtualMachine::runMovFromRam()
{
    auto destinationRegisterCode = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[destinationRegisterCode];
    auto src_addr = reinterpret_cast<std::uint16_t*>(m_ram.data() + m_ip);
    m_ip += 2;
    *destinationRegisterPointer = *reinterpret_cast<std::uint16_t*>(m_ram.data() + *src_addr);
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: mov_from_ram "
                  << m_registerName[destinationRegisterCode] << ", [" << src_addr << "]" << std::endl;
    }
}

void VirtualMachine::runDumpreg()
{
    if (m_isDebugMode)
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: dumpreg" << std::endl;
    std::cout << "a: " << m_a << std::endl;
    std::cout << "b: " << m_b << std::endl;
    std::cout << "ip: " << m_ip << std::endl;
}

void VirtualMachine::runAdd()
{
    auto registersInformation = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[registersInformation >> 4];
    auto sourceRegisterPointer = m_registerPointer[registersInformation & 0x0F];
    *destinationRegisterPointer += *sourceRegisterPointer;
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: add "
                  << m_registerName[registersInformation >> 4] << ", "
                  << m_registerName[registersInformation & 0x0F] << std::endl;
    }
}

void VirtualMachine::runSub()
{
    auto registersInformation = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[registersInformation >> 4];
    auto sourceRegisterPointer = m_registerPointer[registersInformation & 0x0F];
    *destinationRegisterPointer -= *sourceRegisterPointer;
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: sub "
                  << m_registerName[registersInformation >> 4] << ", "
                  << m_registerName[registersInformation & 0x0F] << std::endl;
    }
}

void VirtualMachine::runInc()
{
    auto destinationRegisterCode = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[destinationRegisterCode];
    ++(*destinationRegisterPointer);
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: inc "
                  << m_registerName[destinationRegisterCode] << std::endl;
    }
}

void VirtualMachine::runDec()
{
    auto destinationRegisterCode = m_ram[m_ip++];
    auto destinationRegisterPointer = m_registerPointer[destinationRegisterCode];
    --(*destinationRegisterPointer);
    if (m_isDebugMode) {
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: dec "
                  << m_registerName[destinationRegisterCode] << std::endl;
    }
}

void VirtualMachine::runRet()
{
    m_isRunning = false;
    if (m_isDebugMode)
        std::cout << "debug [" << std::setw(2) << std::setfill('0') << m_lineNumber++ << "]: ret" << std::endl;
}

void VirtualMachine::updateIPAndCommandsCounter()
{
    ++m_ip;
}
