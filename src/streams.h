//---------------------------------------------------------------------------//
//                                                                           //
//  PDFR Streams implementation file                                         //
//                                                                           //
//  Copyright (C) 2018 - 2019 by Allan Cameron                               //
//                                                                           //
//  Licensed under the MIT license - see https://mit-license.org             //
//  or the LICENSE file in the project root directory                        //
//                                                                           //
//---------------------------------------------------------------------------//

#ifndef PDFR_STREAMS

//---------------------------------------------------------------------------//

#define PDFR_STREAMS

/* Streams in pdf files are usually made up of a sequence of non-ascii bytes
 * intended to represent raw data. When they occur they are always part of a pdf
 * object, which will always start with a <<dictionary>>. At the end of the
 * dictionary, after the closing brackets, comes the keyword 'stream', usually
 * (?always) followed by two whitespace bytes: \r and \n. The data then begins.
 * The end of the stream is declared by the sequence (\r\nendstream).
 *
 * The data can represent many different things including pictures, fonts,
 * annotations and postscript-type page descriptions. For the purposes of text
 * extraction, it is mainly the latter we are interested in.
 *
 * The raw data in the stream is almost always compressed, so needs to be
 * decompressed before being processed. That is the purpose of the stream class.
 *
 * At present, only the flatedecode decompression algorithm is implemented.
 * I have yet to find a pdf file that uses anything else for page description
 * to allow testing.
 *
 * The possible stream types are:
 *
 *  Ascii85Stream,
 *  AsciiHexStream,
 *  DecodeStream,
 *  FlateStream,
 *  NullStream,
 *  PredictorStream,
 *  RunLengthStream,
 *  StreamsSequenceStream,
 *  StringStream,
 *  LZWStream
 *
 * This header is required by the xref class, as it needs to be able to deflate
 * xrefstreams.
 */

#include<string>
#include<vector>
#include<iostream>
#include<stdexcept>
#include<map>

//---------------------------------------------------------------------------//
// Stand-alone function to inflate a deflate stream

void FlateDecode(std::string* deflated_string);

//---------------------------------------------------------------------------//
// The Stream class is the base class for the different streams used in pdfs.
// It provides a unified interface, with an input string, an output string,
// and an iterator for each. It allows for consumption of individual bytes or
// even for bits within bytes, while keeping track of its reading position and
// signalling when the end of a stream has been reached without throwing.

class Stream
{
// The constructors are protected to make this an abstract class.
protected:
  Stream(const std::string*);           // string version
  Stream(const std::vector<uint8_t>*);  // bytes version (converts to string)

public:
  inline std::string Output(){return output_;} // Getter for output
  uint32_t GetByte();                          // Consumes next byte
  uint32_t PeekByte();                         // Looks but doesn't consume
  void Reset();                                // Returns stream to start
  uint32_t GetBits(uint32_t n);                // Get next n bits
  uint32_t BitFlip(uint32_t value, uint32_t);  // Reverses bit order

  // Appends byte to output and advances iterator
  inline void WriteOutput(uint8_t byte){output_.append(1, (char) byte);
                                        output_position_++;}

  // Writes a repeat sequence from earlier in the ouput to the end of the
  // output. Used in Deflate and LZW.
  inline void AppendPrevious(uint32_t distance_t, uint32_t length_t)
  {
    // Do this rather than use ranges, since repeats of length n can start
    // from m bytes before the end of the output stream, even if n > m;
    for (uint32_t i = 0; i < length_t; ++i)
    {
      WriteOutput(output_[output_position_ - distance_t]);
    }
  }

  // Gets a byte from a specific location in the output stream
  inline char GetOutput(){return output_[output_position_++];}

  private:
  const std::string* input_;        // The input string
  std::string output_;              // The output string
  size_t input_position_;           // Input iterator
  size_t output_position_;          // Output iterator
  uint8_t unconsumed_bits_;         // Bit iterator
  uint32_t unconsumed_bit_value_;   // Keeps track of unused bits

};

//---------------------------------------------------------------------------//
// This class reinvents the wheel in an attempt to free the library from
// dependencies. It is a full implementation of Deflate decompression. It uses
// std::map for storing and looking up Huffman trees

class Deflate : public Stream
{
public:
  // String and byte-vector constructors. The latter converts to a string.
  Deflate(const std::string*);
  Deflate(const std::vector<uint8_t>*);

private:
  bool is_last_block_;    // Flag so decompressor knows when to stop
  void CheckHeader();     // Read first two bytes to ensure valid Deflate

  // The fixed literal and distance maps are used if compression used a
  // fixed dictionary. Usually this only happens with short messages.
  static const std::map<uint32_t, uint32_t> fixed_literal_map_;
  static const std::map<uint32_t, uint32_t> fixed_distance_map_;

  // If we come across a length code or a distance code, we need to know
  // how many extra bytes to read. This is looked up in these tables.
  static const std::vector<uint32_t> length_table_;
  static const std::vector<uint32_t> distance_table_;

  // Whether its fixed or dynamic compression, we want to end up with a literal
  // and distance map that we can look up.
  std::map<uint32_t, uint32_t> literal_map_;
  std::map<uint32_t, uint32_t> distance_map_;

  void ReadBlock();               // Co-ordinates reading of a single block
  void BuildDynamicCodeTable();   // Builds lookup tables for each block
  void ReadCodes();               // Actual reading of compressed data
  void HandlePointer(uint32_t);   // Deals with length & distance pointers

  // Finds the next code in the input stream using given lookup table
  uint32_t ReadCode(std::map<uint32_t, uint32_t>&);

  // Creates a Huffman tree from a vector of bit lengths.
  std::map<uint32_t, uint32_t> Huffmanize(const std::vector<uint32_t>&);
};

#endif

