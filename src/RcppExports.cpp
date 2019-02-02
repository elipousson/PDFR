// Generated by using Rcpp::compileAttributes() -> do not edit by hand
// Generator token: 10BE3573-1514-4C36-9D1C-5A225CD40393

#include <Rcpp.h>

using namespace Rcpp;

// get_xref
Rcpp::DataFrame get_xref(const std::string& filename);
RcppExport SEXP _PDFR_get_xref(SEXP filenameSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type filename(filenameSEXP);
    rcpp_result_gen = Rcpp::wrap(get_xref(filename));
    return rcpp_result_gen;
END_RCPP
}
// get_xrefraw
Rcpp::DataFrame get_xrefraw(const std::vector<uint8_t>& rawfile);
RcppExport SEXP _PDFR_get_xrefraw(SEXP rawfileSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<uint8_t>& >::type rawfile(rawfileSEXP);
    rcpp_result_gen = Rcpp::wrap(get_xrefraw(rawfile));
    return rcpp_result_gen;
END_RCPP
}
// get_object
Rcpp::List get_object(const std::string& filename, int o);
RcppExport SEXP _PDFR_get_object(SEXP filenameSEXP, SEXP oSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< int >::type o(oSEXP);
    rcpp_result_gen = Rcpp::wrap(get_object(filename, o));
    return rcpp_result_gen;
END_RCPP
}
// get_objectraw
Rcpp::List get_objectraw(const std::vector<uint8_t>& rawfile, int o);
RcppExport SEXP _PDFR_get_objectraw(SEXP rawfileSEXP, SEXP oSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<uint8_t>& >::type rawfile(rawfileSEXP);
    Rcpp::traits::input_parameter< int >::type o(oSEXP);
    rcpp_result_gen = Rcpp::wrap(get_objectraw(rawfile, o));
    return rcpp_result_gen;
END_RCPP
}
// pdfpage
Rcpp::List pdfpage(const std::string& filename, int pagenum);
RcppExport SEXP _PDFR_pdfpage(SEXP filenameSEXP, SEXP pagenumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type filename(filenameSEXP);
    Rcpp::traits::input_parameter< int >::type pagenum(pagenumSEXP);
    rcpp_result_gen = Rcpp::wrap(pdfpage(filename, pagenum));
    return rcpp_result_gen;
END_RCPP
}
// pdfpageraw
Rcpp::List pdfpageraw(const std::vector<uint8_t>& rawfile, int pagenum);
RcppExport SEXP _PDFR_pdfpageraw(SEXP rawfileSEXP, SEXP pagenumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<uint8_t>& >::type rawfile(rawfileSEXP);
    Rcpp::traits::input_parameter< int >::type pagenum(pagenumSEXP);
    rcpp_result_gen = Rcpp::wrap(pdfpageraw(rawfile, pagenum));
    return rcpp_result_gen;
END_RCPP
}
// getglyphmap
Rcpp::DataFrame getglyphmap(const std::string& s, int pagenum);
RcppExport SEXP _PDFR_getglyphmap(SEXP sSEXP, SEXP pagenumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type s(sSEXP);
    Rcpp::traits::input_parameter< int >::type pagenum(pagenumSEXP);
    rcpp_result_gen = Rcpp::wrap(getglyphmap(s, pagenum));
    return rcpp_result_gen;
END_RCPP
}
// pagestring
std::string pagestring(const std::string& s, int pagenum);
RcppExport SEXP _PDFR_pagestring(SEXP sSEXP, SEXP pagenumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type s(sSEXP);
    Rcpp::traits::input_parameter< int >::type pagenum(pagenumSEXP);
    rcpp_result_gen = Rcpp::wrap(pagestring(s, pagenum));
    return rcpp_result_gen;
END_RCPP
}
// pagestringraw
std::string pagestringraw(const std::vector<uint8_t>& rawfile, int pagenum);
RcppExport SEXP _PDFR_pagestringraw(SEXP rawfileSEXP, SEXP pagenumSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<uint8_t>& >::type rawfile(rawfileSEXP);
    Rcpp::traits::input_parameter< int >::type pagenum(pagenumSEXP);
    rcpp_result_gen = Rcpp::wrap(pagestringraw(rawfile, pagenum));
    return rcpp_result_gen;
END_RCPP
}
// pdfdoc
Rcpp::DataFrame pdfdoc(const std::string& s);
RcppExport SEXP _PDFR_pdfdoc(SEXP sSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::string& >::type s(sSEXP);
    rcpp_result_gen = Rcpp::wrap(pdfdoc(s));
    return rcpp_result_gen;
END_RCPP
}
// pdfdocraw
Rcpp::DataFrame pdfdocraw(const std::vector<uint8_t>& s);
RcppExport SEXP _PDFR_pdfdocraw(SEXP sSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< const std::vector<uint8_t>& >::type s(sSEXP);
    rcpp_result_gen = Rcpp::wrap(pdfdocraw(s));
    return rcpp_result_gen;
END_RCPP
}
// utf
std::string utf(std::vector<uint16_t> u);
RcppExport SEXP _PDFR_utf(SEXP uSEXP) {
BEGIN_RCPP
    Rcpp::RObject rcpp_result_gen;
    Rcpp::RNGScope rcpp_rngScope_gen;
    Rcpp::traits::input_parameter< std::vector<uint16_t> >::type u(uSEXP);
    rcpp_result_gen = Rcpp::wrap(utf(u));
    return rcpp_result_gen;
END_RCPP
}

static const R_CallMethodDef CallEntries[] = {
    {"_PDFR_get_xref", (DL_FUNC) &_PDFR_get_xref, 1},
    {"_PDFR_get_xrefraw", (DL_FUNC) &_PDFR_get_xrefraw, 1},
    {"_PDFR_get_object", (DL_FUNC) &_PDFR_get_object, 2},
    {"_PDFR_get_objectraw", (DL_FUNC) &_PDFR_get_objectraw, 2},
    {"_PDFR_pdfpage", (DL_FUNC) &_PDFR_pdfpage, 2},
    {"_PDFR_pdfpageraw", (DL_FUNC) &_PDFR_pdfpageraw, 2},
    {"_PDFR_getglyphmap", (DL_FUNC) &_PDFR_getglyphmap, 2},
    {"_PDFR_pagestring", (DL_FUNC) &_PDFR_pagestring, 2},
    {"_PDFR_pagestringraw", (DL_FUNC) &_PDFR_pagestringraw, 2},
    {"_PDFR_pdfdoc", (DL_FUNC) &_PDFR_pdfdoc, 1},
    {"_PDFR_pdfdocraw", (DL_FUNC) &_PDFR_pdfdocraw, 1},
    {"_PDFR_utf", (DL_FUNC) &_PDFR_utf, 1},
    {NULL, NULL, 0}
};

RcppExport void R_init_PDFR(DllInfo *dll) {
    R_registerRoutines(dll, NULL, CallEntries, NULL, NULL);
    R_useDynamicSymbols(dll, FALSE);
}
