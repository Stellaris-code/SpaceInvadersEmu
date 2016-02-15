/*  opcodes %{Cpp:License:ClassName} - Yann BOUCHER (yann) 07/02/2016
**
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**                    Version 2, December 2004
**
** Copyright (C) 2004 Sam Hocevar <sam@hocevar.net>
**
** Everyone is permitted to copy and distribute verbatim or modified
** copies of this license document, and changing it is allowed as long
** as the name is changed.
**
**            DO WHAT THE FUCK YOU WANT TO PUBLIC LICENSE
**   TERMS AND CONDITIONS FOR COPYING, DISTRIBUTION AND MODIFICATION
**
**  0. You just DO WHAT THE FUCK YOU WANT TO.
*/
#ifndef OPCODES_HPP
#define OPCODES_HPP

#include "common.hpp"
#include "cpu.hpp"
#include "cpustate.hpp"
#include "opcodedefinition.hpp"
#include "utility.hpp"

namespace i8080
{
inline void registerOpcodes(CPU& cpu)
{

    // Carry bit instructions

    OpcodeDefinition cmc("00111111", [](byte, State& state) -> unsigned int {
        setFlagC(state.F, !getFlagC(state.F));
        return 4;
    });
    cpu.registerOpcode(cmc);

    OpcodeDefinition stc("00110111", [](byte, State& state) -> unsigned int {
        setFlagC(state.F, 1);
        return 4;
    });
    cpu.registerOpcode(stc);

    // Single register instructions

    OpcodeDefinition inr("00***100", [](byte opcode, State& state) -> unsigned int {
        unsigned int result = Opcode::readReg(Opcode::getDdd(opcode), state)+1;
        checkACarry(state.F, result, state);
        checkSign(state.F, result);
        checkZero(state.F, result);
        checkParity(state.F, result);
        Opcode::writeReg(Opcode::getDdd(opcode), result & 0xFF, state);
        return Opcode::getDdd(opcode) == 0x06 ? 10 : 5;
    });
    cpu.registerOpcode(inr);

    OpcodeDefinition dcr("00***101", [](byte opcode, State& state) -> unsigned int {
        unsigned int result = Opcode::readReg(Opcode::getDdd(opcode), state)-1;
        checkACarry(state.F, result, state);
        checkSign(state.F, result);
        checkZero(state.F, result);
        checkParity(state.F, result);
        Opcode::writeReg(Opcode::getDdd(opcode), result & 0xFF, state);
        return Opcode::getDdd(opcode) == 0x06 ? 10 : 5;
    });
    cpu.registerOpcode(dcr);


    OpcodeDefinition cma("00101111", [](byte, State& state) -> unsigned int {
        state.A ^= 0xFF;
        return 4;
    });
    cpu.registerOpcode(cma);

    OpcodeDefinition daa("00100111", [](byte, State& state) -> unsigned int {
        byte top4 = (state.A >> 4) & 0xF;
        byte bot4 = state.A & 0xF;

        if (bot4 > 9 || getFlagAC(state.F))
        {
            checkACarry(state.F, state.A + 6, state);
            state.A += 6;
            top4 = (state.A >> 4) & 0xF;
            bot4 = state.A & 0xF;
        }

        if (top4 > 9 || getFlagC(state.F))
        {
            setFlagC(state.F, top4 + 6 >= 16);
            top4 += 6;
            state.A = ((top4 & 0xF) << 4) | bot4;
        }

        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);

        return 4;
    });
    cpu.registerOpcode(daa);

    // NOP instructions

    OpcodeDefinition nop("00***000", [](byte, State&) -> unsigned int {
        return 4;
    });
    cpu.registerOpcode(nop);

    // Data transfer instructions

    OpcodeDefinition mov("01******", [](byte opcode, State& state) -> unsigned int {
        Opcode::writeReg(Opcode::getDdd(opcode), Opcode::readReg(Opcode::getSss(opcode), state), state);
        return Opcode::getDdd(opcode) == 0x06 ? 7 : 5;
    });
    cpu.registerOpcode(mov);

    OpcodeDefinition stax("000*0010", [](byte opcode, State& state) -> unsigned int {
        state.mem.write(getBit(opcode, 4) ? state.DE : state.BC, state.A);
        return 7;
    });
    cpu.registerOpcode(stax);

    OpcodeDefinition ldax("000*1010", [](byte opcode, State& state) -> unsigned int {
        state.A = state.mem.read(getBit(opcode, 4) ? state.DE : state.BC);
        return 7;
    });
    cpu.registerOpcode(ldax);

    // Register or memory to accumulator instructions

    OpcodeDefinition add("10000***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        checkACarry(state.F, state.A + val, state);
        checkCarry(state.F, state.A + val);
        checkSign(state.F, state.A + val);
        checkZero(state.F, state.A + val);
        checkParity(state.F, state.A + val);
        state.A += val;
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(add);

    OpcodeDefinition adc("10001***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state) + getFlagC(state.F);
        checkACarry(state.F, state.A + val, state);
        checkCarry(state.F, state.A + val);
        checkSign(state.F, state.A + val);
        checkZero(state.F, state.A + val);
        checkParity(state.F, state.A + val);
        state.A += val;
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(adc);

    OpcodeDefinition sub("10010***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        checkACarry(state.F, state.A - val, state);
        checkCarry(state.F, state.A - val);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);
        state.A -= val;
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(sub);

    OpcodeDefinition sbb("10011***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state) + getFlagC(state.F);
        checkACarry(state.F, state.A - val, state);
        checkCarry(state.F, state.A - val);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);
        state.A -= val;
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(sbb);

    OpcodeDefinition ana("10100***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        state.A &= val;
        checkACarry(state.F, state.A, state);
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);
        setFlagC(state.F, 0);
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(ana);

    OpcodeDefinition xra("10101***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        state.A ^= val;
        checkACarry(state.F, state.A, state);
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);
        setFlagC(state.F, 0);
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(xra);

    OpcodeDefinition ora("10110***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        state.A |= val;
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);
        checkACarry(state.F, state.A, state);
        setFlagC(state.F, 0);
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(ora);

    OpcodeDefinition cmp("10111***", [](byte opcode, State& state) -> unsigned int {
        byte val = Opcode::readReg(Opcode::getSss(opcode), state);
        checkACarry(state.F, state.A - val, state);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);
        checkCarry(state.F, state.A - val);
        return Opcode::getSss(opcode) == 0x06 ? 7 : 4;
    });
    cpu.registerOpcode(cmp);

    // Rotate accumulator instructions

    OpcodeDefinition rlc("00000111", [](byte, State& state) -> unsigned int {
        setFlagC(state.F, getBit(state.A, 7));
        state.A = rotl(state.A, 1);
        return 4;
    });
    cpu.registerOpcode(rlc);


    OpcodeDefinition rrc("00001111", [](byte, State& state) -> unsigned int {
        setFlagC(state.F, getBit(state.A, 0));
        state.A = rotr(state.A, 1);
        return 4;
    });
    cpu.registerOpcode(rrc);

    OpcodeDefinition ral("00010111", [](byte, State& state) -> unsigned int {
        bool oldC = getFlagC(state.F);
        setFlagC(state.F, getBit(state.A, 7));
        state.A = rotl(state.A, 1);
        setBit(state.A, 0, oldC);
        return 4;
    });
    cpu.registerOpcode(ral);

    OpcodeDefinition rar("00011111", [](byte, State& state) -> unsigned int {
        bool oldC = getFlagC(state.F);
        setFlagC(state.F, getBit(state.A, 0));
        state.A = rotr(state.A, 1);
        setBit(state.A, 7, oldC);
        return 4;
    });
    cpu.registerOpcode(rar);

    // Register pair instructions

    OpcodeDefinition push("11**0101", [](byte opcode, State& state) -> unsigned int {
        word value = 0;
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                value = state.BC;
                break;
            case 1:
                value = state.DE;
                break;
            case 2:
                value = state.HL;
                break;
            case 3:
                value = state.AF;
                break;
        }
        state.mem.write(--state.sp, (value >> 8) & 0xFF);
        state.mem.write(--state.sp, value & 0xFF);
        return 11;
    });
    cpu.registerOpcode(push);

    OpcodeDefinition pop("11**0001", [](byte opcode, State& state) -> unsigned int {
        word value = 0;
        value = state.mem.read(state.sp++);
        value |= state.mem.read(state.sp++) << 8;
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                state.BC = value;
                break;
            case 1:
                state.DE = value;
                break;
            case 2:
                state.HL = value;
                break;
            case 3:
                state.AF = value;
                break;
        }
        return 10;
    });
    cpu.registerOpcode(pop);

    OpcodeDefinition dad("00**1001", [](byte opcode, State& state) -> unsigned int {
        word value = 0;
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                value = state.BC;
                break;
            case 1:
                value = state.DE;
                break;
            case 2:
                value = state.HL;
                break;
            case 3:
                value = state.sp;
                break;
        }

        setFlagC(state.F, unsigned(value + state.HL) > 0xFFFF);
        state.HL += value;
        return 10;
    });
    cpu.registerOpcode(dad);

    OpcodeDefinition inx("00**0011", [](byte opcode, State& state) -> unsigned int {
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                state.BC++;
                break;
            case 1:
                state.DE++;
                break;
            case 2:
                state.HL++;
                break;
            case 3:
                state.sp++;
                break;
        }
        return 5;
    });
    cpu.registerOpcode(inx);

    OpcodeDefinition dcx("00**1011", [](byte opcode, State& state) -> unsigned int {
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                state.BC--;
                break;
            case 1:
                state.DE--;
                break;
            case 2:
                state.HL--;
                break;
            case 3:
                state.sp--;
                break;
        }
        return 5;
    });
    cpu.registerOpcode(dcx);

    OpcodeDefinition xchg("11101011", [](byte, State& state) -> unsigned int {
        std::swap(state.DE, state.HL);
        return 4;
    });
    cpu.registerOpcode(xchg);

    OpcodeDefinition xthl("11100011", [](byte, State& state) -> unsigned int {
        byte tempL = state.mem.read(state.sp);
        byte tempH = state.mem.read(state.sp + 1);
        state.mem.write(state.sp, state.L);
        state.mem.write(state.sp + 1, state.H);
        state.H = tempH;
        state.L = tempL;
        return 18;
    });
    cpu.registerOpcode(xthl);

    OpcodeDefinition sphl("11111001", [](byte, State& state) -> unsigned int {
        state.sp = state.HL;
        return 5;
    });
    cpu.registerOpcode(sphl);

    // Immediate instructions

    OpcodeDefinition lxi("00**0001", [](byte opcode, State& state) -> unsigned int {
        byte pair = (opcode >> 4) & 0x3;
        switch (pair)
        {
            case 0:
                state.BC = Opcode::fetchWord(state);
                break;
            case 1:
                state.DE = Opcode::fetchWord(state);
                break;
            case 2:
                state.HL = Opcode::fetchWord(state);
                break;
            case 3:
                state.sp = Opcode::fetchWord(state);
                break;
            default:
                error("lxi : Invalid register pair !");
        }
        return 10;
    });
    cpu.registerOpcode(lxi);

    OpcodeDefinition mvi("00***110", [](byte opcode, State& state) -> unsigned int {
        byte dest = Opcode::getDdd(opcode);
        Opcode::writeReg(dest, Opcode::fetchByte(state), state);
        return Opcode::getDdd(opcode) == 0x06 ? 7 : 5;
    });
    cpu.registerOpcode(mvi);

    OpcodeDefinition adi("11000110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        checkACarry(state.F, state.A + val, state);
        checkCarry(state.F, state.A + val);
        checkSign(state.F, state.A + val);
        checkZero(state.F, state.A + val);
        checkParity(state.F, state.A + val);
        state.A += val;

        return 7;
    });
    cpu.registerOpcode(adi);

    OpcodeDefinition aci("11001110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state) + getFlagC(state.F);
        checkACarry(state.F, state.A + val, state);
        checkCarry(state.F, state.A + val);
        checkSign(state.F, state.A + val);
        checkZero(state.F, state.A + val);
        checkParity(state.F, state.A + val);
        state.A += val;

        return 7;
    });
    cpu.registerOpcode(aci);

    OpcodeDefinition sui("11010110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        checkACarry(state.F, state.A - val, state);
        checkCarry(state.F, state.A - val);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);
        state.A -= val;

        return 7;
    });
    cpu.registerOpcode(sui);

    OpcodeDefinition sbi("11011110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state) + getFlagC(state.F);
        checkACarry(state.F, state.A - val, state);
        checkCarry(state.F, state.A - val);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);
        state.A -= val;

        return 7;
    });
    cpu.registerOpcode(sbi);

    OpcodeDefinition ani("11100110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        state.A &= val;
        checkACarry(state.F, state.A, state);
        checkCarry(state.F, state.A);
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);

        return 7;
    });
    cpu.registerOpcode(ani);

    OpcodeDefinition xri("11101110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        state.A ^= val;
        checkACarry(state.F, state.A, state);
        checkCarry(state.F, state.A);
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);

        return 7;
    });
    cpu.registerOpcode(xri);

    OpcodeDefinition ori("11110110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        state.A |= val;
        checkACarry(state.F, state.A, state);
        checkCarry(state.F, state.A);
        checkSign(state.F, state.A);
        checkZero(state.F, state.A);
        checkParity(state.F, state.A);

        return 7;
    });
    cpu.registerOpcode(ori);

    OpcodeDefinition cpi("11111110", [](byte, State& state) -> unsigned int {
        byte val = Opcode::fetchByte(state);
        checkACarry(state.F, state.A - val, state);
        checkCarry(state.F, state.A - val);
        checkSign(state.F, state.A - val);
        checkZero(state.F, state.A - val);
        checkParity(state.F, state.A - val);

        return 7;
    });
    cpu.registerOpcode(cpi);

    // Direct addressing instructions

    OpcodeDefinition sta("00110010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        state.mem.write(addr, state.A);
        return 13;
    });
    cpu.registerOpcode(sta);

    OpcodeDefinition lda("00111010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        state.A = state.mem.read(addr);
        return 13;
    });
    cpu.registerOpcode(lda);

    OpcodeDefinition shld("00100010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        state.mem.write(addr, state.L);
        state.mem.write(addr + 1, state.H);
        return 16;
    });
    cpu.registerOpcode(shld);

    OpcodeDefinition lhld("00101010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        state.L = state.mem.read(addr);
        state.H = state.mem.read(addr + 1);
        return 16;
    });
    cpu.registerOpcode(lhld);

    // Jump instructions

    OpcodeDefinition pchl("11101001", [](byte, State& state) -> unsigned int {
        state.pc = state.HL;
        return 5;
    });
    cpu.registerOpcode(pchl);

    OpcodeDefinition jmp("11000011", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        state.pc = addr;
        return 10;
    });
    cpu.registerOpcode(jmp);

    OpcodeDefinition jc("11011010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagC(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jc);

    OpcodeDefinition jnc("11010010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagC(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jnc);

    OpcodeDefinition jz("11001010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagZ(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jz);

    OpcodeDefinition jnz("11000010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagZ(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jnz);

    OpcodeDefinition jm("11111010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagS(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jm);

    OpcodeDefinition jp("11110010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagS(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jp);

    OpcodeDefinition jpe("11101010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagP(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jpe);

    OpcodeDefinition jpo("11100010", [](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagP(state.F))
        {
            state.pc = addr;
        }
        return 10;
    });
    cpu.registerOpcode(jpo);

    // Call subroutine instructions

    auto commonCall = [](word addr, State& state)
    {
        state.mem.write(--state.sp, (state.pc >> 8) & 0xFF);
        state.mem.write(--state.sp, state.pc & 0xFF);
        state.pc = addr;
    };

    OpcodeDefinition call("11001101", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);

        commonCall(addr, state);

        return 17;
    });
    cpu.registerOpcode(call);

    OpcodeDefinition cc("11011100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagC(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cc);

    OpcodeDefinition cnc("11010100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagC(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cnc);

    OpcodeDefinition cz("11001100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagZ(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cz);

    OpcodeDefinition cnz("11000100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagZ(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cnz);

    OpcodeDefinition cm("11111100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagS(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cm);

    OpcodeDefinition cp("11110100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagS(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cp);

    OpcodeDefinition cpe("11101100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (getFlagP(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cpe);

    OpcodeDefinition cpo("11100100", [commonCall](byte, State& state) -> unsigned int {
        word addr = Opcode::fetchWord(state);
        if (!getFlagP(state.F))
        {
            commonCall(addr, state);
            return 17;
        }
        return 11;
    });
    cpu.registerOpcode(cpo);

    // Return from subroutine instructions

    auto commonRet = [](State& state)
    {
        byte l = (state.mem.read(state.sp++));
        word h = state.mem.read(state.sp++) << 8;
        state.pc = h | l;
    };

    OpcodeDefinition ret("11001001", [commonRet](byte, State& state) -> unsigned int {
        commonRet(state);
        return 10;
    });
    cpu.registerOpcode(ret);

    OpcodeDefinition rc("11011000", [commonRet](byte, State& state) -> unsigned int {
        if (getFlagC(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rc);

    OpcodeDefinition rnc("11010000", [commonRet](byte, State& state) -> unsigned int {
        if (!getFlagC(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rnc);

    OpcodeDefinition rz("11001000", [commonRet](byte, State& state) -> unsigned int {
        if (getFlagZ(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rz);

    OpcodeDefinition rnz("11000000", [commonRet](byte, State& state) -> unsigned int {
        if (!getFlagZ(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rnz);

    OpcodeDefinition rm("11111000", [commonRet](byte, State& state) -> unsigned int {
        if (getFlagS(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rm);

    OpcodeDefinition rp("11110000", [commonRet](byte, State& state) -> unsigned int {
        if (!getFlagS(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rp);

    OpcodeDefinition rpe("11101000", [commonRet](byte, State& state) -> unsigned int {
        if (getFlagP(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rpe);

    OpcodeDefinition rpo("11100000", [commonRet](byte, State& state) -> unsigned int {
        if (!getFlagP(state.F))
        {
            commonRet(state);
            return 11;
        }
        return 5;
    });
    cpu.registerOpcode(rpo);

    // RST instruction

    OpcodeDefinition rst("11***111", [](byte opcode, State& state) -> unsigned int {
        byte index = Opcode::getAaa(opcode);
        state.mem.write(--state.sp, (state.pc >> 8) & 0xFF);
        state.mem.write(--state.sp, state.pc & 0xFF);
        state.pc = index * 8;
        return 11;
    });
    cpu.registerOpcode(rst);

    // Interrupt flip-flop instructions

    OpcodeDefinition ei("11111011", [](byte, State& state) -> unsigned int {
        state.int_enabled = true;
        return 4;
    });
    cpu.registerOpcode(ei);

    OpcodeDefinition di("11110011", [](byte, State& state) -> unsigned int {
        state.int_enabled = false;
        return 4;
    });
    cpu.registerOpcode(di);

    // Input-output instructions

    OpcodeDefinition in("11011011", [](byte, State& state) -> unsigned int {
        byte port = Opcode::fetchByte(state);
        state.A = state.io.inb(port);
        return 10;
    });
    cpu.registerOpcode(in);

    OpcodeDefinition out("11010011", [](byte, State& state) -> unsigned int {
        byte port = Opcode::fetchByte(state);
        state.io.outb(port, state.A);
        return 10;
    });
    cpu.registerOpcode(out);

    // Halt instruction

    OpcodeDefinition hlt("01110110", [](byte, State& state) -> unsigned int {
        state.halt_flag = true;
        return 7;
    });
    cpu.registerOpcode(hlt);
}
} // namespace i8080

#endif // OPCODES_HPP
