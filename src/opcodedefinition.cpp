/*  src/opcodedefinition.cpp OpcodeDefinition - Yann BOUCHER (yann) 26/01/2016
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
#include "opcodedefinition.hpp"

namespace i8080
{

OpcodeDefinition::OpcodeDefinition(const std::string& opcodePattern, const opcodeCallback& callback)
    : m_callback(callback)
{
    checkPattern(toUpper(opcodePattern));
    if (!m_callback)
    {
        error("OpcodeDefinition : Invalid Callback ! (" + opcodePattern + ")");
    }
    createBaseAndMask(opcodePattern);
    m_wildcardAmount = std::count(opcodePattern.cbegin(), opcodePattern.cend(), '*');
}

bool OpcodeDefinition::isMatching(byte opcode) const
{
    return (opcode & m_mask) == m_base;
}

unsigned int OpcodeDefinition::execute(byte opcode, State& context) const
{
    return m_callback(opcode, context);
}

void OpcodeDefinition::checkPattern(const std::string& opcodePattern) const
{
    if (opcodePattern.size() != 8)
    {
        error("Opcode pattern size invalid ! (" + opcodePattern + ")");
    }

    if(opcodePattern.find_first_not_of("01*") != std::string::npos)
    {
        error("Opcode pattern format invalid ! (" + opcodePattern + ")");
    }

    if(opcodePattern == "********")
    {
        error("Opcode pattern cannot be only wildcards ! ");
    }
}

void OpcodeDefinition::createBaseAndMask(const std::string& opcodePattern)
{
    for (uint8_t i { 0 }; i < 8; ++i)
    {
        if (opcodePattern[i] == '*')
        {
            unsetBit(m_mask, std::abs(i - 7));
        }
        else
        {
            setBit(m_base, std::abs(i - 7), std::stoul(std::string(1, opcodePattern[i]), nullptr));
        }
    }
}


bool operator==(const OpcodeDefinition& a, const OpcodeDefinition& b)
{
    return a.m_mask == b.m_mask && a.m_base == b.m_base;
}

bool operator!=(const OpcodeDefinition& a, const OpcodeDefinition& b)
{
    return !(a == b);
}

} // namespace i8080
