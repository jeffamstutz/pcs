// ========================================================================== //
// The MIT License (MIT)                                                      //
//                                                                            //
// Copyright (c) Jefferson Amstutz                                            //
//                                                                            //
// Permission is hereby granted, free of charge, to any person obtaining a    //
// copy of this software and associated documentation files (the "Software"), //
// to deal in the Software without restriction, including without limitation  //
// the rights to use, copy, modify, merge, publish, distribute, sublicense,   //
// and/or sell copies of the Software, and to permit persons to whom the      //
// Software is furnished to do so, subject to the following conditions:       //
//                                                                            //
// The above copyright notice and this permission notice shall be included in //
// in all copies or substantial portions of the Software.                     //
//                                                                            //
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR //
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,   //
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL    //
// THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER //
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING    //
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER        //
// DEALINGS IN THE SOFTWARE.                                                  //
// ========================================================================== //

#include "scanner.h"
#include "utility.h"

#include <cctype>

namespace pcs {

  // Helper types /////////////////////////////////////////////////////////////

  enum class Token
  {
    // commands
    CMD_FUNC,
    CMD_EXTERN,
    // primary
    IDENTIFIER,
    NUMBER,
    STRING,
    // control
    CTRL_IF,
    CTRL_THEN,
    CTRL_ELSE,
    CTRL_FOR,
    CTRL_USING,
    // operators
    OP_BINARY,
    OP_UNARY,
    // extra
    EoF,
    UNDEFINED
  };

  static inline std::string getTokName(Token tok)
  {
    switch (tok) {
    case Token::EoF:
      return "EoF";
    case Token::CMD_FUNC:
      return "func";
    case Token::CMD_EXTERN:
      return "extern";
    case Token::IDENTIFIER:
      return "identifier";
    case Token::NUMBER:
      return "number";
    case Token::STRING:
      return "string";
    case Token::CTRL_IF:
      return "if";
    case Token::CTRL_THEN:
      return "then";
    case Token::CTRL_ELSE:
      return "else";
    case Token::CTRL_FOR:
      return "for";
    case Token::CTRL_USING:
      return "using";
    case Token::OP_BINARY:
      return "binop";
    case Token::OP_UNARY:
      return "unop";
    case Token::UNDEFINED:
    default:
      return "undefined";
    }
  }

  /////////////////////////////////////////////////////////////////////////////

  void scanFile(const std::string &file)
  {
    // TODO?
  }

}  // namespace pcs