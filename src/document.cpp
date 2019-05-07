//---------------------------------------------------------------------------//
//                                                                           //
//  PDFR Document implementation file                                        //
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

#include "document.h"
#include<list>

//---------------------------------------------------------------------------//
// See the document.h file for comments regarding the rationale and use of this
// class. The comments here are for descriptions of the functions themselves.
//---------------------------------------------------------------------------//

using namespace std;

/*---------------------------------------------------------------------------*/
// Constructor for Document class. This takes a file location as an argument
// then uses the GetFile() function from utilities.h to read in the filestring
// from which buildDoc() then creates the object

Document::Document(const string& filename) :
  file_path_(filename), file_string_(GetFile(file_path_))
{
  build_document(); // Call constructor helper to build Document
}

/*---------------------------------------------------------------------------*/
// Constructor for Document class. This takes raw data in the form of a vector
// of bytes and converts it to the filestring before calling the helper function
// to construct the Document object

Document::Document(const vector<uint8_t>& bytevector) :
  file_string_(std::string(bytevector.begin(), bytevector.end()))
{
  build_document(); // Call constructor helper to build Document
}

/*---------------------------------------------------------------------------*/
// buildDoc is just a helper function to create Document objects. It is the
// "final common pathway" of both non-default Document constructor functions
// and is seperated out to make this clear and avoid duplication of code

void Document::build_document()
{
  xref_ = make_shared<const XRef>(make_shared<string>(file_string_));
  read_catalog();             // Gets the catalog dictionary
  read_page_directory();             // Gets the /Pages dictionary
}

/*---------------------------------------------------------------------------*/
// One of two public interface functions after Document object creation. This
// returns an object of Object class with the requested object number. It first
// checks to see whether that object has been retrieved before. If so, it
// returns it from the 'objects' vector. If not, it creates the object then
// stores a copy in the 'objects' vector before returning the requested object.

shared_ptr<Object> Document::get_object(int n)
{
  // Check if object n is already stored
  if(objects_.find(n) == objects_.end())
  {
    // If it is not stored, check whether it is in an object stream
    size_t holder = xref_->GetHoldingNumberOf(n);

    // If object is in a stream, create it recursively from the stream object
    if(holder) objects_[n] = make_shared<Object>(get_object(holder), n);

    // Otherwise create & store it directly
    else objects_[n] = make_shared<Object>(xref_, n);
  }

  return objects_[n];
}

/*---------------------------------------------------------------------------*/
// The catalog (never spelled 'catalogue') dictionary contains information
// about the pdf, including which object contains the /Pages dictionary.
// This finds the catalog object from the trailer dictionary which is read as
// part of xref creation

void Document::read_catalog()
{
  // The pointer to the catalog is given under /Root in the trailer dictionary
  int root_number = xref_->GetTrailer().GetReference("/Root");

  // With errors handled, we can now just get the pointed-to object's dictionary
  catalog_ = get_object(root_number)->GetDictionary();
}

/*---------------------------------------------------------------------------*/
// The /Pages dictionary contains pointers to every page represented in the pdf
// file. There should be a pointer to it in the catalog dictionary. If its not
// there, the pdf structure is unspecified and we throw an error

void Document::read_page_directory()
{
  // Else get the object number of the /Pages dictionary
  int page_object_number = catalog_.GetReference("/Pages");

  // Now fetch that object and store it
  page_directory_ = get_object(page_object_number)->GetDictionary();

  // Ensure /Pages has /kids entry
  if (!page_directory_.ContainsReferences("/Kids"))
    throw runtime_error("No Kids entry in /Pages");

  // Create the page directory tree. Start with the pages object as root node
  auto root = make_shared<TreeNode<int>>(page_object_number);

  // Populate the tree
  expand_kids(page_directory_.GetReferences("/Kids"), root);

  // Get the leafs of the tree
  page_object_numbers_ = root->GetLeafs();
}

/*---------------------------------------------------------------------------*/
// The /Pages dictionary acts as a root node to point to the actual objects
// that contain page descriptors. These pointers are given in the /Kids entry
// of the /Pages dictionary. Unfortunately, sometimes with a large document,
// the pointers do not point directly to page descriptors, but to further
// /Pages dictionaries with /Kids entries that act as parent nodes for further
// /Pages dictionaries and so on. This is a tree structure, and for our purposes
// we only want the leaf nodes of the tree. This algorithm uses recursion to
// populate the nodes of the tree class defined in utilities.h.
//
// This function takes a lot of the time needed for document creation. It is
// not that the algorithm is particularly slow; rather, it has to create all the
// objects it comes across, and there are at least as many of these are there
// are pages. Options for speeding this up include getting only the objects
// needed for a particular page's creation, which would mean a major change to
// the way the program works depending on user input, and only getting object
// streams for an object when they are requested. This second way is more
// promising, but it is likely to be complex, and it is not clear that it would
// lead to a major speedup. Getting an object at present takes about 36us.

void Document::expand_kids(const vector<int>& object_numbers,
                          shared_ptr<TreeNode<int>> tree)
{
  // This function is only called from a single point that is already range
  // checked, so does not need error checked.

  // Create new children tree nodes with this one as parent
  tree->AddKids(object_numbers);

  // Get a vector of pointers to the new nodes
  auto kid_nodes = tree->GetKids();

  // For each new node get a vector of ints for its kid nodes
  for(auto& kid : kid_nodes)
  {
    auto refs =
      get_object(kid->Get())->GetDictionary().GetReferences("/Kids");

    // If it has children, use recursion to get them
    if (!refs.empty()) expand_kids(refs, kid);
  }
}

/*---------------------------------------------------------------------------*/
// Public function that gets a specific page header from the pageheader vector

Dictionary Document::get_page_header(int page_number)
{
  // Ensure the pagenumber is valid
  if((page_object_numbers_.size() < (size_t) page_number) || page_number < 0)
  {
    throw runtime_error("Invalid page number");
  }

  // All good - return the requested header
  return objects_[page_object_numbers_[page_number]]->GetDictionary();
}


