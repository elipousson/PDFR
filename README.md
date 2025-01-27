
<!-- README.md is generated from README.Rmd. Please edit that file -->

# PDFR

<!-- badges: start -->

[![Lifecycle:
experimental](https://img.shields.io/badge/lifecycle-experimental-orange.svg)](https://lifecycle.r-lib.org/articles/stages.html#experimental)
[![License:
MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
<!-- badges: end -->

The goal of PDFR is to aid data scientists who need the ability to
extract data from files in pdf format. PDFR is a new C++ based R library
to extract usable text from portable document format (pdf) files.

The majority of the code base is written in C++ with a view to being
ported to other languages, but at present it is constructed to be built
as an R package.

## Installation

You can install the development version of PDFR from
[GitHub](https://github.com/) with:

``` r
# install.packages("pak")
pak::pkg_install("AllanCameron/PDFR")
```

## Usage

The main function used to extract all data from a pdf page to an R data
frame is `pdfpage()`. This accepts either the path to a pdf or a raw
data vector representing a pdf. For example, this is how you extract all
text from page 1 of the barcodes PDF from `pdfr_paths`:

``` r
library(PDFR)

pdfpage(pdfr_paths$barcodes, 1)
#>                               text  left right bottom   top    font size
#> 1                             None  53.5  74.4  774.2 782.2 Courier    8
#> 2                   Acute medicine 187.4 255.9  774.2 782.2 Courier    8
#> 3                                / 258.8 264.8  774.2 782.2 Courier    8
#> 4                             ward 267.8 288.6  774.2 782.2 Courier    8
#> 5                               11 291.6 303.5  774.2 782.2 Courier    8
#> 6 jean.cairney@ggc.scot.nhs.uk0141 318.3 470.1  774.2 782.2 Courier    8
#> 7                              211 473.0 490.9  774.2 782.2 Courier    8
#> 8                             5719 493.9 514.7  774.2 782.2 Courier    8
```

## Background

The current version is at an early stage of development. It will work
with most pdfs, but there are some unsupported features which may lead
to some pdfs producing runtime errors.

Documents encrypted using the standard method and which can be opened
without a password are supported. Password-based encryption is currently
unsupported.

If there are any suggestions for development please submit a feature
request, or let me know about pdfs that break the package.

## Motivation

Extracting useful data from pdf is difficult for two reasons. Firstly,
the pdf format primarily consists of binary data, which is laid out in
such a way as to provide quick random access to pdf *objects* as
required by a pdf reader. The text elements as seen on the page are
usually encoded in a binary stream within the document. Even when the
binary stream is decoded, the text items exist as individual elements
within a page description program, which has to be parsed before the
text can be extracted. It is therefore not a trivial matter to extract
the “raw text” from a pdf file into a format in which it can be read by
R, though there exist some excellent tools that can do this quickly. In
particular,
[pdftools](https://ropensci.org/blog/2016/03/01/pdftools-and-jeroen/)
provides an R interface to some of Poppler’s pdf tools, and can quickly
and reliably extract text wholesale from pdf.

The second problem is that, unlike some other common file types used to
exchange information on the internet (e.g. html, xml, csv, JSON), the
raw text extracted from a pdf does not have a fixed structure to provide
semantic information about the data to allow it to be processed easily
by a data scientist.

The mismatch between the fact that humans can read data from pdfs so
easily yet the format is so difficult to convert into machine-readable
data is explained by the fact that humans use the structure of the page
layout to provide the semantic context to the data. When the structure
is lost (as it often is with copy and pasting from PDF), it becomes very
difficult for a human reader to interpret. The computer does not know
how to interpret the characters’ positions, so it cannot classify the
characters by semantics as a human reader (usually) can.

The idea behind PDFR is to try to extract raw text then use the
positioning and formatting data from the extracted text to reconstruct
some of the semantic content that would otherwise be lost. For example,
identifying and grouping letters into words, words into paragraphs or
into tables.

Ultimately, to extract useful data, the user will need the option to
control how and to what extent text elements are grouped. For example,
they may need the fine control of having every letter’s position on the
page (e.g. to accurately reconstruct a part of the document on a plot),
or may wish to extract a corpus of plain text from a book as a set of
paragraphs or even whole pages.

PDFR is written in C++ 11 and has no external dependencies, but makes
extensive use of the C++ standard libraries. Rather than being based on
an existing library such as [xpdf](https://www.xpdfreader.com/) or
[Poppler](https://poppler.freedesktop.org/), it was written from scratch
with the specific goal of making text extraction easier for R users.
Most of the design is new, an attempt to implement the text extraction
elements of the pdf standard [ISO
32000](https://www.iso.org/standard/51502.html), though it borrows some
concepts from existing open-source libraries such as Poppler and
[pdfjs](https://mozilla.github.io/pdf.js/).

Clearly, the package would not exist without the excellent
[Rcpp](http://www.rcpp.org/) package. Much of the pdf parsing would take
too long to do in R, but having the facility to write C++ extensions
makes pdf parsing feasible, and even pretty quick in some cases.

## Related projects

- [pdftools](https://github.com/ropensci/pdftools): Text Extraction,
  Rendering and Converting of PDF Documents.
- [qpdf](https://github.com/ropensci/qpdf): Content-preserving
  transformations transformations of PDF files such as split, combine,
  and compress. This package interfaces directly to the ‘qpdf’ C++ API
  and does not require any command line utilities.
- [tabulizer](https://github.com/ropensci/tabulizer): Bindings for
  Tabula PDF Table Extractor Library
- [PDE](https://github.com/erikstricker/PDE): The PDE (Pdf Data
  Extractor) allows the extraction of information and tables optionally
  based on search words from PDF (Portable Document Format) files and
  enables the visualization of the results, both by providing a
  convenient user-interface.
- [xmpdf](https://github.com/trevorld/r-xmpdf): Edit XMP metadata and
  PDF bookmarks/documentation info.
