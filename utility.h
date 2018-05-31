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

#pragma once

#include <string>
#include <vector>

namespace pcs {

  inline std::vector<std::string> split(const std::string &input,
                                        const std::string &delim)
  {
    std::vector<std::string> tokens;
    auto pos = size_t(0);
    while (true) {
      auto begin = input.find_first_not_of(delim, pos);
      if (begin == input.npos) return tokens;
      auto end = input.find_first_of(delim, begin);
      auto token = input.substr(begin,
                                (end == input.npos) ? input.npos : (end - begin));
      tokens.push_back(token);
      pos = end;
    }
  }

} // namespace pcs