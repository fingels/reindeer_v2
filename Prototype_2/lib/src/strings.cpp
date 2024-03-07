#include <reindeer2/strings.hpp>

std::string stringToBinaryString(std::string const &str)
{
    std::string cible="";

    for(int i=0;i<str.size();i++){
        switch (str[i])
        {
        case 'A':
            cible+= "00";
            break;
        
        case 'C':
            cible+="01";
            break;
        
        case 'G':
            cible+="10";
            break;

        case 'T':
            cible+="11";
            break;
        
        default:
            std::cout << "String should contain only A,T,C,G characters" << std::endl;
            break;
        }
    }

    return cible;
}

char complement(const char in)
{ return ((in & 2) ? '\x8a' - in : '\x95' - in); }

std::string reverseComplement(std::string const &kmer)
{
    std::string reverseComp;

    int n = kmer.size();

    for (int j=0; j<n; j++)
    {
        reverseComp+= complement(kmer[n-j-1]);
    }
    return reverseComp;
}

std::string findMinimizer(std::string const &kmer, int m)
{
    
    int n=kmer.size();

    if (n<m)
    {
        std::cout << "ERROR: string is too short" << std::endl;
        return "";
    }
    else
    {
        std::string minimizer = canonical(kmer.substr(0,m));
        uint64_t valMin = xorshift(std::stoull(stringToBinaryString(minimizer)));

        for (int i=1;i<n-m+1;i++)
        {
            std::string candidate = canonical(kmer.substr(i,m));
            uint64_t valCandidate = xorshift(std::stoull(stringToBinaryString(candidate)));
            
            if (valCandidate<valMin){
                minimizer=candidate;
            }
        }
        return minimizer;
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
            return reverseComplement(kmer);
        }
    }
    // ce cas de figure ne peut se produire que si kmer==reverseComplement(kmer), ce qui nécessite k pair
    return kmer;
}

std::vector<std::string> split (const std::string &s, char delim) {
    std::vector<std::string> result;
    std::stringstream ss (s);
    std::string item;

    while (getline (ss, item, delim)) {
        result.push_back (item);
    }

    return result;
}
