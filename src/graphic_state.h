//---------------------------------------------------------------------------//
//                                                                           //
//  PDFR graphic_state header file                                           //
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

//---------------------------------------------------------------------------//

#define PDFR_GS

/* The graphic_state class represents the last of our dealings with the actual
 * pdf file. After this stage, we have a complete description of the text on
 * the page including the size and position of every correctly-encoded glyph.
 * The subsequent steps will use only this data to try to reconstruct useful
 * semantic information from the text position in an attempt to provide useable
 * data, and to output the result to a variety of formats.
 *
 * The job of the graphic_state class is to parse the pdf page description
 * language into a table of glyphs, positions, sizes and fontnames - one row
 * for each character on the page. The instructions from the page description
 * language have already been "compiled" by the lexer into an instruction set,
 * but we now need to interpret and enact those instructions.
 *
 * Conceptually, this is done using operators and operands. This is actually
 * made a bit easier by the grammar of the page description language, which
 * operates on a stack system - most tokens are operands, and are loaded onto
 * the stack until an operator is reached. When the operator is reached, it
 * performs an action on the operands then clears the stack.
 *
 * In order that graphic_state can interpret the operands, it needs to know
 * about the fonts on the page, the content string, and any xobjects that
 * are called to be inserted on the page. It therefore needs to use the page's
 * public interface to get these data, and in fact is created by giving the
 * constructor a pointer to a page. It takes the content string, sends it to
 * the tokenizer to compile the instructions, then uses an instruction reader
 * which loads the stack with operands and calls the appropriate function when
 * it reaches an operator, until the instructions are finished.
 *
 * The functions that the instruction reader calls take up most of the code
 * in the implementation. They do work on the operands to change the global
 * graphics state, set fonts, write characters, handle kerning etc. There are
 * a number of private data members which maintain state between loops of the
 * instruction reader, and some which record the entire history of the state.
 *
 * The final output of graphic_state is a collection of vectors, all of the same
 * length, comprising the Unicode symbol, width, font size, font name and x/y
 * position of every character on the page. This is output as a specific struct
 * to reduce the passing around of several parameters.
 */

#include "tokenizer.h"
#include "page.h"

//---------------------------------------------------------------------------//
// This struct is a container for the output of the graphic_state class. All
// of the vectors are the same length, so it can be thought of as a table with
// one row for each glyph on the page. This makes it straightforward to output
// to other formats if needed.

struct GSoutput
{
  std::vector<Unicode> text;      // vector of unicode code points
  std::vector<float> left;        // vector of glyphs' left edges
  std::vector<float> bottom;      // vector of glyphs' bottom edges
  std::vector<float> right;       // vector of glyphs' right edges
  std::vector<std::string> fonts; // vector of glyphs' font names
  std::vector<float> size;        // vector of glyphs' point size
  std::vector<float> width;       // vector of glyphs' widths in text space
};

//---------------------------------------------------------------------------//

class graphic_state
{
public:
  // constructor
  graphic_state(page*);

  // access results
  GSoutput output();

private:
  //private data members - used to maintain state between calls to parser

  page*                           p;              // pointer to creating page
  font*                           wfont;          // pointer to "working" font
  float                           currfontsize;   // Current font size
  std::vector<float>              initstate,      // Identity 3x3 matrix as vec9
                                  fontsizestack,  // stack of current font size
                                  fontsize,       // size of glyph in points
                                  widths,         // width of glyph in textspace
                                  Tmstate,        // Text matrix state
                                  Tdstate;        // Temp modification to Tm
  std::vector<std::vector<float>> gs,             // stack of graphics state
                                  statehx;        // history of graphics state
  std::string                     currentfont;    // Name of current font
  std::vector<std::string>        fontname,       // vector of font names
                                  fontstack;      // stack of font history
  std::vector<Unicode>            stringres;      // working Unicode vector
  int                             PRstate;        // current kerning state
  float                           Tl,             // Leading (line spacing)
                                  Tw,             // Word spacing
                                  Th,             // Horizontal scaling
                                  Tc;             // Character spacing


  // The instruction set returned by tokenizer and read by parser
  std::vector<std::pair<std::string, Token::TState>> Instructions;

  // The main output struct

  GSoutput db;

  // Private methods

  // The parser method takes the compiled instructions and writes operands
  // to a "stack", or calls an operator method depending on the label given
  // to each token in the instruction set. It loops through the entire
  // instruction set, after which the data just needs tidied and wrapped.
  void parser(std::vector<std::pair<std::string, Token::TState>>&, std::string);

  void Do(std::string&);                  //----------------------------------//
  void Q(std::vector<std::string>& );           //  OPERATOR METHODS
  void q(std::vector<std::string>& );           //
  void TH(std::vector<std::string>& );          //  These functions do the
  void TW(std::vector<std::string>& );          //  work of performing actions
  void TC(std::vector<std::string>& );          //  on the graphics state and
  void TL(std::vector<std::string>& );          //  writing the results. They
  void Tstar(std::vector<std::string>& );       //  are called by the parser
  void Tm(std::vector<std::string>& );          //  method according to the
  void cm(std::vector<std::string>& );          //  operator it encounters, and
  void Td(std::vector<std::string>& );          //  act on any operands sitting
  void TD(std::vector<std::string>& );          //  on the stack. Each is named
  void BT(std::vector<std::string>& );          //  for the operator it enacts.
  void ET(std::vector<std::string>& );          //  These functions use private
  void Tf(std::vector<std::string>& );          //  data members to maintain
  void TJ(std::string,                          //  state in between calls.
          std::vector<std::string>&,            //
          std::vector<Token::TState>&);   //---------------------------------//

  // This is a helper function for the TJ method which otherwise would become
  // a bit of a "hairball". It uses the font information and current graphics
  // state to identify the intended glyph, size and position from a character
  // in a pdf string object
  void processRawChar(std::vector<RawChar>&, float&,
                      std::vector<float>&,   float&);

  // This tidies and wraps the data
  void MakeGS();

  // Multiplies to 3x3 matrices represented as length-9 vector floats
  std::vector<float> matmul(std::vector<float>, std::vector<float>);

  // Converts pdfs' 6-token string representation of matrices to a 3x3 matrix
  std::vector<float> stringvectomat(std::vector<std::string>);
};

//---------------------------------------------------------------------------//

#endif