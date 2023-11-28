#ifndef STRINGS_HANDLING_INCLUDED
#define STRINGS_HANDLING_INCLUDED

#include <string>
#include <iostream>

/*
TODO
*/
uint64_t xorshift ( uint64_t x );

/*
Convertit une chaîne de caractère en chaîne de caractère binaire
*/
std::string stringToBinaryString(std::string const& str);

/*
Renvoie le complément A<->T et C<->G, reproduit de https://stackoverflow.com/a/15151947/11253795
WARNING: marche uniquement avec A,T,C,G
*/
char complement(const char in);


/*
Calcule le reverse complement d'un kmer
*/
std::string reverseComplement(std::string const& kmer,int n=NULL);

/*
Retourne l'index de départ de la plus petite (au sens lexicographique) sous-chaîne de taille m
*/
std::string findMinimizer(std::string const& kmer, int m);

/*
Renvoie le plus petit mot (au sens lexicographique) entre un kmer et son reverse complement
*/
std::string canonical(std::string const& kmer);

#endif // STRINGS_HANDLING_INCLUDED
