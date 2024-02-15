#ifndef FILES_HANDLING_INCLUDED
#define FILES_HANDLING_INCLUDED

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <unordered_map>

#include "strings.hpp"


typedef std::tuple<int,int> couple;

struct key_hash : public std::unary_function<couple, std::size_t>
{
   std::size_t operator()(const couple& k) const
   {
      return std::get<0>(k) ^ std::get<1>(k);
   }
};

struct key_equal : public std::binary_function<couple, couple, bool>
{
   bool operator()(const couple& v0, const couple& v1) const
   {
      return (
               std::get<0>(v0) == std::get<0>(v1) &&
               std::get<1>(v0) == std::get<1>(v1) 
             );
   }
};


typedef std::unordered_map<couple,int,key_hash,key_equal> couple_map;

typedef std::unordered_map<std::string,double> kmer_count_map;
typedef std::unordered_map<std::string,kmer_count_map> minimizer_map;
typedef std::unordered_map<std::string, int> kmer_class_map;

/*
Read FASTA file `fileName`, and extract the first `cap` unitigs and their counting value, respectively stored in tables `unitigs` and `counts`.
*/
void readFasta(const std::string fileName, const double cap,std::vector<std::string> &unitigs, std::vector<double> &counts);

/*
Provided a list of `unitigs` and their respective `counts` :
1. Split each unitig in k-mers
2. Compute for each k-mer its minimizer (of size m)
3. Store in a nested unordered map : minimizer -> k-mer -> count

Note that it is not assumed that k-mers appear only once in the unitigs: the same k-mer can be found in different unitigs and the count value will be updated accordingly
*/
void unitigsToKmersWithCounts(const int k, const int m,std::vector<std::string> &unitigs, std::vector<double> &counts, minimizer_map &minimizerTable);

/*
Provided an unordered map k-mers -> count, for each k-mer:
1. If the associated count has never been seen:
   a. create a new class of count 
   b. map in `countToClasses` : count -> class
   c. map in `classesToCount` : class -> count
2. Add 1 to the abundance of the class associated to the count in `classesKmerAbundance`
*/
void assignEquivalenceClasses(const kmer_count_map &kmerMap,std::unordered_map<double,int> &countToClasses,std::unordered_map<int,int> &classesKmerAbundance, std::unordered_map<int,double> &classesToCounts);

/*
Create a file `fileName` , where each line has format `k-mer;class of count`
*/
void saveKmerCountMap(const kmer_count_map &kmerMap, const std::string fileName, std::unordered_map<double,int> &countToClasses);


/*
Create a file `fileName`, where each line (but the first) has format `class ID \t number of k-mers with this class ID \t vector of abundance associated to the class`.
The first line gives the number of classes in the file.
*/
void saveGlobalClasses(const std::string fileName, std::unordered_map<int,int> &globalClassesKmerAbundance, std::unordered_map<int,std::vector<double>> &globalClassesCountVectors);

/*
Read a file saved with `saveKmerCountMap` or `saveKmerClassMap` and create a unordered map k-mer -> class of count
*/
void readMinimizerFile(const std::string fileName, kmer_class_map &kmerClassMap);

/*
Create a file `fileName`, where each line has format `k-mer;class of count`
*/
void saveKmerClassMap(const kmer_class_map &kmerClassMap, std::string fileName);

#endif // FILES_HANDLING_INCLUDED
