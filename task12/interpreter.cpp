#include "interpreter.hpp"
#include "constants.hpp"
#include <boost/algorithm/string.hpp>
#include <boost/exception/all.hpp>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <regex>
#include <stdexcept>

Interpreter::Interpreter(bool isDebugMode)
    : m_virtualMachine { 8192, isDebugMode }
{
    m_commandCallback[CommandName::Run] = [this] { handleRun(); };
    m_commandCallback[CommandName::Save] = [this] { handleSave(); };
    m_commandCallback[CommandName::Load] = [this] { handleLoad(); };
    m_commandCallback[CommandName::Reset] = [this] { handleReset(); };
    m_commandCallback[CommandName::Exit] = [this] { handleExit(); };
    m_commandCallback[CommandName::MovToReg] = [this] { handleMovToReg(); };
    m_commandCallback[CommandName::MovToRam] = [this] { handleMovToRam(); };
    m_commandCallback[CommandName::MovFromRam] = [this] { handleMovFromRam(); };
    m_commandCallback[CommandName::Dumpreg] = [this] { handleDumpreg(); };
    m_commandCallback[CommandName::Add] = [this] { handleAdd(); };
    m_commandCallback[CommandName::Sub] = [this] { handleSub(); };
    m_commandCallback[CommandName::Inc] = [this] { handleInc(); };
    m_commandCallback[CommandName::Dec] = [this] { handleDec(); };
    m_commandCallback[CommandName::Ret] = [this] { handleRet(); };

    m_registerCode[RegisterName::A] = RegisterCode::A;
    m_registerCode[RegisterName::B] = RegisterCode::B;
}

void Interpreter::start()
{
    std::string message;
    std::cout << "Welcome!" << std::endl;

    try {
        while (m_isEnabled) {
            std::cout << '>';
            if (!std::getline(std::cin, message))
                break;
            interpretInput(message);
        }
    } catch (...) {
        std::cerr << "interpreter: somethin wrong!" << std::endl;
    }
}

void Interpreter::interpretInput(const std::string& message)
{
    try {
        std::smatch matches;
        std::regex formattingRegex {
            "([a-z]+)(?: ([a-z/]+|[0-9]+|\\[[0-9]+\\])(?:, ([a-z\\]+|[0-9]+|\\[[0-9]+\\]))?)?",
        };

        if (!std::regex_match(message, matches, formattingRegex))
            throw std::runtime_error("error: invalid input");

        auto commandName = matches[1].str();
        m_arguments = { matches[2].str(), matches[3].str() };

        if (commandName == "mov") {
            std::regex addressFormattingRegex { "\\[([0-9]+)\\]" };

            if (std::regex_match(m_arguments.first, matches, addressFormattingRegex)) {
                commandName = "mov_to_ram";
                m_arguments.first = matches[1].str();
            } else if (std::regex_match(m_arguments.first, matches, addressFormattingRegex)) {
                commandName = "mov_from_ram";
                m_arguments.second = matches[1].str();
            } else {
                commandName = "mov_to_reg";
            }
        }

        if (m_commandCallback.count(commandName))
            m_commandCallback[commandName]();
        else
            throw std::runtime_error("error <Interpreter>: unknown command");
    } catch (boost::bad_lexical_cast e) {
        std::cerr << e.what() << std::endl;
    } catch (std::out_of_range e) {
        std::cerr << e.what() << std::endl;
    } catch (std::ios_base::failure e) {
        std::cerr << e.what() << std::endl;
    }
}

void Interpreter::handleRun()
{
    m_virtualMachine.run();
}

void Interpreter::handleSave() const
{
    m_virtualMachine.save(m_arguments.first);
}

void Interpreter::handleLoad()
{
    m_virtualMachine.load(m_arguments.first);
}

void Interpreter::handleReset()
{
    m_virtualMachine.reset();
}

void Interpreter::handleExit()
{
    m_isEnabled = false;
}

void Interpreter::handleMovToReg()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    auto value = boost::lexical_cast<std::uint16_t>(m_arguments.second);
    m_virtualMachine.writeMovToReg(destinationRegister, value);
}

void Interpreter::handleMovToRam()
{
    auto destinationAddress = boost::lexical_cast<std::uint16_t>(m_arguments.first);
    auto sourceRegister = m_registerCode[m_arguments.second];
    m_virtualMachine.writeMovToRam(destinationAddress, sourceRegister);
}

void Interpreter::handleMovFromRam()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    auto sourceAddress = boost::lexical_cast<std::uint16_t>(m_arguments.second);
    m_virtualMachine.writeMovFromRam(destinationRegister, sourceAddress);
}

void Interpreter::handleDumpreg()
{
    m_virtualMachine.writeDumpreg();
}

void Interpreter::handleAdd()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    auto sourceRegister = m_registerCode[m_arguments.second];
    m_virtualMachine.writeAdd(destinationRegister, sourceRegister);
}

void Interpreter::handleSub()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    auto sourceRegister = m_registerCode[m_arguments.second];
    m_virtualMachine.writeSub(destinationRegister, sourceRegister);
}

void Interpreter::handleInc()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    m_virtualMachine.writeInc(destinationRegister);
}

void Interpreter::handleDec()
{
    auto destinationRegister = m_registerCode[m_arguments.first];
    m_virtualMachine.writeDec(destinationRegister);
}

void Interpreter::handleRet()
{
    m_virtualMachine.writeRet();
}
