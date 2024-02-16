#ifndef STRINGS_INCLUDED
#define STRINGS_INCLUDED

#include <string>
#include <iostream>
#include <reindeer2/utils.hpp>

/** @file strings.hpp
 *  @defgroup strings
 *  @author Florian Ingels
 * */


/*
Convert a ACTG string into a binary string (A = 00 / C =01 / G = 10 / T = 11)
*/
std::string stringToBinaryString(std::string const& str);


/*
Reproduced from https://stackoverflow.com/a/15151947/11253795.
For a character A,T,C,G, returns its complement (A<->T and C<->G).
*/
char complement(const char in);


/*
Compute the reverse complement of a k-mer
*/
std::string reverseComplement(std::string const& kmer,int n=NULL);

/*
Returns the m-mer with minimal hash value extracted from the k-mer
*/
std::string findMinimizer(std::string const& kmer, int m);

/*
Returns the smallest lexicographic word between a k-mer and its reverse complement
*/
std::string canonical(std::string const& kmer);

#endif // STRINGS_INCLUDED
