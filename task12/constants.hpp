#pragma once

#include "types.hpp"

namespace CommandCode {
static constexpr Command MovToReg = 0x00;
static constexpr Command MovToRam = 0x01;
static constexpr Command MovFromRam = 0x02;
static constexpr Command Dumpreg = 0x03;
static constexpr Command Add = 0x04;
static constexpr Command Sub = 0x05;
static constexpr Command Inc = 0x06;
static constexpr Command Dec = 0x07;
static constexpr Command Ret = 0x08;
};

namespace CommandName {
static constexpr auto Run = "run";
static constexpr auto Reset = "reset";
static constexpr auto Save = "save";
static constexpr auto Load = "load";
static constexpr auto Exit = "exit";
static constexpr auto MovToReg = "mov_to_reg";
static constexpr auto MovToRam = "mov_to_ram";
static constexpr auto MovFromRam = "mov_from_ram";
static constexpr auto Dumpreg = "dumpreg";
static constexpr auto Add = "add";
static constexpr auto Sub = "sub";
static constexpr auto Inc = "inc";
static constexpr auto Dec = "dec";
static constexpr auto Ret = "ret";
};

namespace RegisterCode {
static constexpr Register A = 0x00;
static constexpr Register B = 0x01;
};

namespace RegisterName {
static constexpr auto A = "a";
static constexpr auto B = "b";
};
