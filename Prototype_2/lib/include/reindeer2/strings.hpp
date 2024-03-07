#ifndef STRINGS_INCLUDED
#define STRINGS_INCLUDED

#include <string>
#include <iostream>
#include <vector>
#include <sstream>
#include <reindeer2/utils.hpp>

/** @file strings.hpp
 *  @defgroup strings
 *  @author Florian Ingels
 * */


/**
 * @brief Converts an ACTG string into a binary string, following the correspondance A=00, C=01, G=10, T=11
 * @ingroup strings
 * @param[in] str A string containing only the letters A, C, T or G
*/
std::string stringToBinaryString(std::string const& str);


/**
 * @brief Returns the complement of a character A<->T or C<->G
 * @ingroup strings
 * @param[in] in A character within the alphabet A,T,C,G
 * @remark Borrowed from https://stackoverflow.com/a/15151947/11253795
*/
char complement(const char in);


/*
Compute the reverse complement of a k-mer
*/
std::string reverseComplement(std::string const& kmer);

/*
Returns the m-mer with minimal hash value extracted from the k-mer
kmer is assumed to be canonical
*/
std::string findMinimizer(std::string const& kmer, int m);

/*
Returns the smallest lexicographic word between a k-mer and its reverse complement
*/
std::string canonical(std::string const& kmer);


/**
 * @brief Split a string according to the chosen delimiter
 * @ingroup strings
 * @remark Borrowed from https://stackoverflow.com/a/46931770/11253795
*/
std::vector<std::string> split(const std::string &s, char delim);

#endif // STRINGS_INCLUDED
