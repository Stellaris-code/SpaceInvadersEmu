/*  include/opcodedefinition.hpp OpcodeDefinition - Yann BOUCHER (yann) 26/01/2016
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
#ifndef OPCODEDEFINITION_HPP
#define OPCODEDEFINITION_HPP

#include <algorithm>

#include "common.hpp"
#include "cpustate.hpp"
#include "utility.hpp"

namespace i8080
{

class OpcodeDefinition
{
        friend bool operator==(const OpcodeDefinition& a, const OpcodeDefinition& b);
        friend bool operator!=(const OpcodeDefinition& a, const OpcodeDefinition& b);

    public:
        OpcodeDefinition(const std::string& opcodePattern, const opcodeCallback& callback);

    public:
        bool isMatching(byte opcode) const;
        unsigned int execute(byte opcode, State& context) const;
        unsigned int wildcardAmount() const
        { return m_wildcardAmount; }

    private:
        void createBaseAndMask(const std::string& opcodePattern);
        void checkPattern(const std::string& opcodePattern) const;

    private:
        unsigned int m_wildcardAmount { 0 };
        opcodeCallback m_callback;
        byte m_base = 0x00;
        byte m_mask = 0xFF;
};

} // namespace i8080

#endif // OPCODEDEFINITION_HPP
