//---------------------------------------------------------------------------//
//                                                                           //
//                          PDFR main C++ file                               //
//                        (C) 2018 Allan Cameron                             //
//                                                                           //
//          This file provides the library's definitions for the             //
//        functions that will be exported to be made available in R.         //
//                                                                           //
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//
//  Include Rcpp plugin for c++11:                                           //
// [[Rcpp::plugins(cpp11)]]                                                  //
//---------------------------------------------------------------------------//
//---------------------------------------------------------------------------//

#include<Rcpp.h>
#include "pdfr.h"
#include "fileio.h"
#include "debugtools.h"
#include "dictionary.h"
#include "stringfunctions.h"
#include "document.h"
#include "miniz.h"
#include "crypto.h"
#include "streams.h"

//---------------------------------------------------------------------------//

Rcpp::List PDFpage(document mypdf, page pg)
{
  std::vector<EncMap> fontenc;
  for(auto i : pg.fontnames) fontenc.push_back(pg.fontmap[i].EncodingMap);
  return Rcpp::List::create(
    Rcpp::Named("Resources")  =  pg.resources.R_out(),
    Rcpp::Named("Fonts")      =  pg.fonts.R_out(),
    Rcpp::Named("Encodings")  =  fontenc,
    Rcpp::Named("Box")        =  pg.minbox,
    Rcpp::Named("Rotate")     =  pg.rotate,
    Rcpp::Named("PageString") =  pg.contentstring,
    Rcpp::Named("Elements")   =  GraphicsState(pg).db
  );
}

/*---------------------------------------------------------------------------*/

Rcpp::DataFrame get_xref(const std::string& filename)
{
  document mydoc = document(filename);
  std::vector<int> ob, startb, stopb, inob;

  if(mydoc.Xref.getObjects().size() > 0)
    {
      for(int j : mydoc.Xref.getObjects())
        {
          ob.push_back(j);
          startb.push_back(mydoc.Xref.getStart(j));
          stopb.push_back(mydoc.Xref.getEnd(j));
          inob.push_back(mydoc.Xref.inObject(j));
        }
    }
  return Rcpp::DataFrame::create(Rcpp::Named("Object") = ob,
                                 Rcpp::Named("StartByte") = startb,
                                 Rcpp::Named("StopByte") = stopb,
                                 Rcpp::Named("InObject") = inob);
}

//---------------------------------------------------------------------------//

Rcpp::List get_object(const std::string& filename, int o)
{
  document mydoc = document(filename);
  return Rcpp::List::create(
    Rcpp::Named("header") = mydoc.getobject(o).getDict().R_out(),
    Rcpp::Named("stream") = mydoc.getobject(o).getStream());
}

//---------------------------------------------------------------------------//

std::string get_obj_stream(const std::string& filename, int o)
{
  return document(filename).getobject(o).getStream();
}

//---------------------------------------------------------------------------//

Rcpp::List pdfdoc(const std::string & filepath)
{
  document myfile = document(filepath);
  Rcpp::CharacterVector filename = Rcpp::wrap(myfile.file);
  return Rcpp::List::create(Rcpp::Named("file") = filename);
}

//---------------------------------------------------------------------------//

Rcpp::List pdfpage(const std::string& filename, int pagenum)
{
  document myfile = document(filename);
  return PDFpage(myfile, myfile.getPage(pagenum - 1));
}

