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
Lit le fichier FASTA `fileName`, et extrait les `cap` premiers unitigs et leur valeur de comptage, stockées dans les tableaux `unitigs` et `counts`, respectivement.
*/
void readFasta(const std::string fileName, const double cap,std::vector<std::string> &unitigs, std::vector<double> &counts);

/*
TODO
*/
void unitigsToKmersWithCounts(const int k, const int m,std::vector<std::string> &unitigs, std::vector<double> &counts, minimizer_map &minimizerTable);

/*
TODO
*/
void assignEquivalenceClasses(const kmer_count_map &kmerMap,std::unordered_map<double,int> &countToClasses,std::unordered_map<int,int> &classesKmerAbundance, std::unordered_map<int,double> &classesToCounts);

/*
TODO
*/
void saveKmerCountMap(const kmer_count_map &kmerMap, const std::string fileName, std::unordered_map<double,int> &countToClasses);


/*
TODO
*/
void saveGlobalClasses(const std::string fileName, std::unordered_map<int,int> &globalClassesKmerAbundance, std::unordered_map<int,std::vector<double>> &globalClassesCountVectors);

/*
TODO
*/
void readMinimizerFile(const std::string fileName, kmer_class_map &kmerClassMap);

/*
TODO
*/
void saveKmerClassMap(const kmer_class_map &kmerClassMap, std::string fileName);

#endif // FILES_HANDLING_INCLUDED
