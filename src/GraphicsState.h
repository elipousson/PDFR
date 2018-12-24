//---------------------------------------------------------------------------//
//                                                                           //
//  PDFR GraphicsState header file                                           //
//                                                                           //
//  Copyright (C) 2018 by Allan Cameron                                      //
//                                                                           //
//  Permission is hereby granted, free of charge, to any person obtaining    //
//  a copy of this software and associated documentation files               //
//  (the "Software"), to deal in the Software without restriction, including //
//  without limitation the rights to use, copy, modify, merge, publish,      //
//  distribute, sublicense, and/or sell copies of the Software, and to       //
//  permit persons to whom the Software is furnished to do so, subject to    //
//  the following conditions:                                                //
//                                                                           //
//  The above copyright notice and this permission notice shall be included  //
//  in all copies or substantial portions of the Software.                   //
//                                                                           //
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS  //
//  OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF               //
//  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.   //
//  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY     //
//  CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,     //
//  TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE        //
//  SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.                   //
//                                                                           //
//---------------------------------------------------------------------------//


#ifndef PDFR_GS
#define PDFR_GS

#include "pdfr.h"


class GraphicsState
{
public:
  Rcpp::DataFrame db;
  GraphicsState(page& pag);
  page p;

private:
  font wfont;
  std::vector<std::vector<float>> gs, statehx;
  std::vector<float> xvals, yvals, fontsize, widths, Tmstate, Tdstate, R,
  left, right, bottom, size, width, fontsizestack, initstate;
  std::vector<std::string> fontname, stringres, text, fonts, fontstack;
  std::vector<int> leftmatch, rightmatch;
  int PRstate;
  float Tl, Tw, Th, Tc, currfontsize;
  std::string currentfont;
  Instructionset Instructions;
  void InstructionReader(Instructionset I, const std::string& subloop);
  void Q(std::vector<std::string>& Operands);
  void q(std::vector<std::string>& Operands);
  void TH(std::vector<std::string>& Operands);
  void TW(std::vector<std::string>& Operands);
  void TC(std::vector<std::string>& Operands);
  void TL(std::vector<std::string>& Operands);
  void Tstar(std::vector<std::string>& Operands);
  void Tm(std::vector<std::string>& Operands);
  void cm(std::vector<std::string>& Operands);
  void Td(std::vector<std::string>& Operands);
  void TD(std::vector<std::string>& Operands);
  void BT(std::vector<std::string>& Operands);
  void ET(std::vector<std::string>& Operands);
  void Tf(std::vector<std::string>& Operands);
  void TJ(std::vector<std::vector<std::string>>& i);
  void MakeGS();
  void clump();
  void Do(std::string& xo);

};

#endif
