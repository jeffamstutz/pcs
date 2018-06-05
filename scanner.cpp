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

#include <iostream>
#include <fstream>
#include <vector>

namespace pcs {

  // Helper types /////////////////////////////////////////////////////////////

  enum class TokenKind
  {
    IDENTIFIER,
    OPERATOR,
    BRACE,
    SEMICOLON,
    STRING,
    NUMBER,
    WHITESPACE,
    UNDEFINED
  };

  // Tokenization helper functions ////////////////////////////////////////////

  static bool isBrace(char c)
  {
    return c == '{' || c == '}' || c == '(' || c == ')' || c == '[' || c == ']';
  }

  static bool isQuote(char c)
  {
    return c == '"';
  }

  static bool isSemicolon(char c)
  {
    return c == ';';
  }

  static bool isWhitespace(char c)
  {
    return c == ' ' || c == '\t' || c == '\n';
  }

  static bool isOperator(char c)
  {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '<' ||
           c == '>' || c == '=' || c == '!' || c == '|' || c == '&';
  }

  static bool isNumber(char c)
  {
    return c == '1' || c == '2' || c == '3' || c == '4' || c == '5' ||
           c == '6' || c == '7' || c == '8' || c == '9' || c == '0';
  }

  template <TokenKind CURRENT_STATE>
  TokenKind advance(char nextCharacter, std::string &currentToken)
  {
    throw std::runtime_error("unkown version of advance<>() function called!");
  }

  template <>
  TokenKind advance<TokenKind::WHITESPACE>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    const auto &c = nextCharacter;

    if (isWhitespace(c)) return TokenKind::WHITESPACE;

    currentToken.push_back(c);

    if (isQuote(c)) return TokenKind::STRING;
    if (isSemicolon(c)) return TokenKind::SEMICOLON;
    if (isOperator(c)) return TokenKind::OPERATOR;
    if (isNumber(c)) return TokenKind::NUMBER;
    if (isBrace(c)) return TokenKind::BRACE;

    return TokenKind::IDENTIFIER;
  }

  template <>
  TokenKind advance<TokenKind::STRING>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    const auto &c = nextCharacter;
    currentToken.push_back(c);
    if (isQuote(c)) return TokenKind::WHITESPACE;
    return TokenKind::STRING;
  }

  template <>
  TokenKind advance<TokenKind::IDENTIFIER>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    const auto &c = nextCharacter;

    if (isWhitespace(c)) return TokenKind::WHITESPACE;

    currentToken.push_back(c);

    if (isQuote(c)) return TokenKind::STRING;
    if (isOperator(c)) return TokenKind::OPERATOR;
    if (isSemicolon(c)) return TokenKind::SEMICOLON;
    if (isBrace(c)) return TokenKind::BRACE;

    return TokenKind::IDENTIFIER;
  }

  template <>
  TokenKind advance<TokenKind::SEMICOLON>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    return advance<TokenKind::WHITESPACE>(nextCharacter, currentToken);
  }

  template <>
  TokenKind advance<TokenKind::OPERATOR>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    return advance<TokenKind::IDENTIFIER>(nextCharacter, currentToken);
  }

  template <>
  TokenKind advance<TokenKind::BRACE>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    return advance<TokenKind::IDENTIFIER>(nextCharacter, currentToken);
  }

  template <>
  TokenKind advance<TokenKind::NUMBER>(
    char nextCharacter,
    std::string &currentToken
  )
  {
    const auto &c = nextCharacter;

    if (isWhitespace(c)) return TokenKind::WHITESPACE;

    currentToken.push_back(c);

    if (isQuote(c)) return TokenKind::STRING;
    if (isOperator(c)) return TokenKind::OPERATOR;
    if (isSemicolon(c)) return TokenKind::SEMICOLON;
    if (isBrace(c)) return TokenKind::BRACE;
    if (isNumber(c)) return TokenKind::NUMBER;

    return TokenKind::IDENTIFIER;
  }

  // Misc. helper functions ///////////////////////////////////////////////////

  static std::vector<std::string> getLinesFromFile(const std::string &file)
  {
    std::vector<std::string> lines;

    std::string line;
    std::ifstream fileStream(file);
    if (fileStream) {
      while (std::getline(fileStream, line))
        lines.push_back(line);

      fileStream.close();
    } else {
      std::cout << "could not open file '" << file << "'" << std::endl;
    }

    return lines;
  }

  static void printLines(const std::vector<std::string> &lines)
  {
    int lineNum = 0;

    for (const auto &line : lines) {
      std::cout << lineNum << ": " << line << std::endl;
      lineNum++;
    }
  }

  static std::vector<std::string> tokenize(
    const std::vector<std::string> &lines
  )
  {
    std::vector<std::string> tokens;

    std::string token;
    TokenKind currentKind = TokenKind::WHITESPACE;

    for (const auto &l : lines) {
      for (const auto &c : l) {
        auto previousKind = currentKind;
        switch(currentKind) {
        case TokenKind::WHITESPACE:
          currentKind = advance<TokenKind::WHITESPACE>(c, token);
          break;
        case TokenKind::IDENTIFIER:
          currentKind = advance<TokenKind::IDENTIFIER>(c, token);
          break;
        case TokenKind::OPERATOR:
          currentKind = advance<TokenKind::OPERATOR>(c, token);
          break;
        case TokenKind::BRACE:
          currentKind = advance<TokenKind::BRACE>(c, token);
          break;
        case TokenKind::SEMICOLON:
          currentKind = advance<TokenKind::SEMICOLON>(c, token);
          break;
        case TokenKind::STRING:
          currentKind = advance<TokenKind::STRING>(c, token);
          break;
        case TokenKind::NUMBER:
          currentKind = advance<TokenKind::NUMBER>(c, token);
          break;
        default:
          currentKind = advance<TokenKind::UNDEFINED>(c, token);
          break;
        }

        if (previousKind != currentKind &&
            previousKind != TokenKind::WHITESPACE &&
            currentKind != TokenKind::STRING &&
            !token.empty()) {
          tokens.push_back(token);
          token.clear();
        }
      }
    }

    return tokens;
  }

  static void printTokens(const std::vector<std::string> &tokens)
  {
    if (tokens.empty())
      return;

    for (const auto &token : tokens)
      std::cout << "token: " << token << std::endl;
  }

  /////////////////////////////////////////////////////////////////////////////

  void scanFile(const std::string &file)
  {
    auto lines = getLinesFromFile(file);
    printLines(lines);

    std::cout << std::endl;

    auto tokens = tokenize(lines);
    printTokens(tokens);
  }

} // namespace pcs