
#include "files.hpp"


void readFasta(const std::string fileName, const double cap, std::vector<std::string> &unitigs, std::vector<double> &counts)
{
    std::ifstream myFile(fileName);  //Ouverture d'un fichier en lecture

    if(myFile)
    {
        //Tout est prêt pour la lecture.

        int i=0;
        std::string line;
        while (i<cap and getline(myFile,line))
        {
            // C'est la ligne qui contient le header
            int flagCount = line.find("km:f:");
            int endCount = line.substr(flagCount).find(" ");
            double count = std::stod(line.substr(flagCount+5,endCount-5));

            counts.push_back(count);

            // On passe à la ligne qui contient l'unitig
            if (not getline(myFile,line))
            {
                std::cout << "ERROR: FASTA files should have even number of lines" << std::endl;
                break;
            }
            int flagEOL = line.find("\n");

            unitigs.push_back(line.substr(0,flagEOL));
            i++;
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to open the file." << std::endl;
    }
}

void unitigsToKmersWithCounts( const int k, const int m,std::vector<std::string> &unitigs, std::vector<double> &counts, minimizer_map &minimizerTable)
{
    
    for (int i=0; i< unitigs.size(); i++)
    {
        std::string& unitig = unitigs[i];
        double& count = counts[i];

        for (int j=0; j<unitig.size()-k+1;j++)
        {
            std::string kmer = canonical(unitig.substr(j,k));
            
            // on calcule le minimiseur
            std::string minimizer = findMinimizer(kmer,m);

            // on range (minimizer, kmer) dans une table

            if (minimizerTable.count(minimizer) ==0)
            {
                // Le minimizer n'a jamais été vu
                minimizerTable[minimizer][kmer] = count;
            }
            else
            {
                if (minimizerTable[minimizer].count(kmer) ==0)
                {
                    // Le kmer n'a jamais été vu
                    minimizerTable[minimizer][kmer] = count;
                }
                else
                {
                    minimizerTable[minimizer][kmer]+= count;
                }
            }
        }  
    }

    // for (auto x: minimizerTable)
    // {
    //     cout << "Minimizer: " << x.first << endl;
    //     for (auto y: minimizerTable[x.first])
    //     {
    //         cout << " \t" << y.first << "|" << y.second << endl;
    //     }
    // }
}

void assignEquivalenceClasses(const kmer_count_map &kmerMap, std::unordered_map<double, int> &countToClasses, std::unordered_map<int, int> &classesKmerAbundance,std::unordered_map<int,double> &classesToCounts)
{
    int classMax=0;
    if (countToClasses.size()>0)
    {
        for (auto x:countToClasses)
        {
            if (x.second > classMax)
            {
                classMax = x.second;
            }
        }
        classMax++;
    }

    for (auto x:kmerMap)
    {
        if (countToClasses.count(x.second)==0)
        {
            // on a pas encore rencontré ce comptage
            countToClasses[x.second] = classMax;
            classesKmerAbundance[classMax] = 1;
            classesToCounts[classMax] = x.second;
            classMax++;
        }
        else
        {
            classesKmerAbundance[countToClasses[x.second]]++;
        }
    }
}

void saveKmerCountMap(const kmer_count_map &kmerMap, const std::string fileName, std::unordered_map<double,int> &countToClasses)
{
    std::ofstream myFile(fileName);

    if (myFile)
    {
        for (auto x:kmerMap)
        {
            myFile << x.first << ";" << countToClasses[x.second] << std::endl;
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to create the file." << std::endl;
    }

}

void saveGlobalClasses(const std::string fileName, std::unordered_map<int, int> &globalClassesKmerAbundance, std::unordered_map<int, std::vector<double>> &globalClassesCountVectors)
{
    std::ofstream myFile(fileName, std::ofstream::out | std::ofstream::trunc);
    if (myFile)
    {
        myFile << "# Number of classes\t" << globalClassesCountVectors.size() << std::endl;

        for (auto x:globalClassesCountVectors)
        {
            myFile << x.first << "\t" << globalClassesKmerAbundance[x.first] << "\t";

            int vecSize = x.second.size();

            for (int i=0; i<vecSize; i++)
            {
                myFile << x.second[i] << ";";
            }
            myFile << std::endl;
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to create the file." << std::endl;
    }

}

void readMinimizerFile(const std::string fileName, kmer_class_map &kmerClassMap)
{
    std::ifstream myFile(fileName);  //Ouverture d'un fichier en lecture

    if(myFile)
    {
        //Tout est prêt pour la lecture.
        std::string line;
        while (getline(myFile,line))
        {
            if (line!="\n")
            {
                int flag = line.find(";");
                kmerClassMap[line.substr(0,flag)] = std::stoi(line.substr(flag+1));
            }
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to open the file." << std::endl;
    }

}

void saveKmerClassMap(const kmer_class_map &kmerClassMap, std::string fileName)
{
    std::ofstream myFile(fileName,std::ofstream::out | std::ofstream::trunc);

    if (myFile)
    {

        for (auto x:kmerClassMap)
        {
            myFile << x.first << ";" << x.second << std::endl;
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to create the file." << std::endl;
    }

}
