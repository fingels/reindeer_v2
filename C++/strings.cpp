#include "strings.hpp"


/*
std::stoull
*/
uint64_t xorshift ( uint64_t x ) { 
x = ( ( x >> 32 ) ^ x ) * 0xCFEE444D8B59A89B;
x = ( ( x >> 32 ) ^ x ) * 0xCFEE444D8B59A89B;
x = ( ( x >> 32 ) ^ x );
return x;
}



char complement(const char in)
{ return ((in & 2) ? '\x8a' - in : '\x95' - in); }

std::string reverseComplement(std::string const &kmer, int n)
{
    std::string reverseComp;

    if (n==NULL)
    {
        n = kmer.size();
    }

    for (int j=0; j<n; j++)
    {
        reverseComp+= complement(kmer[n-j-1]);
    }
    return reverseComp;
}

int findMinimizer(std::string const &kmer, int m)
{

    /*
    TODO: hasher le canonique de la fenêtre ?

    canoniser le k-mer
    canoniser les m-mer
    hasher
    prendre le plus petit

    */

    int n=kmer.size();
    if (n<m)
    {
        std::cout << "ERROR: string is too short" << std::endl;
        return -1;
    }
    else
    {
        int indexMin=0;
        for (int i=0;i<n-m+1;i++)
        {
            // on parcoure kmer[i:i+m] tant que les lettres coïncident avec avec kmer[indexMin:indexMin+m]
            for (int j=0;j<m;j++)
            {
                if (kmer[i+j]<kmer[indexMin+j])
                {
                    indexMin=i;
                    break;
                }
                else if (kmer[indexMin+j]<kmer[i+j])
                {
                    break;
                }
            }
        }
        return indexMin;
    }
}

std::string canonical(std::string const& kmer)
{

    int n = kmer.size();

    for (int i=0; i<n; i++)
    {
        if (kmer[i]<complement(kmer[n-i-1]))
        {
            // il faut retourner kmer
            return kmer;
        }
        else if (complement(kmer[n-i-1])<kmer[i])
        {
            // il faut retourner le reverse complement
            return reverseComplement(kmer,n);
        }
    }
    // ce cas de figure ne peut se produire que si kmer==reverseComplement(kmer), ce qui nécessite k pair
    return kmer;
}

