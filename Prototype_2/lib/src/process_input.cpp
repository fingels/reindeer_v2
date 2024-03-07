#include <reindeer2/process_input.hpp>

void read_kmtricks_csv(const std::string fileName)
{
    std::ifstream myFile(fileName);

    if(myFile)
    {
        //Tout est prêt pour la lecture.

        std::string line;
        int i=0;

        while (i<10 and getline(myFile,line))
        {

            std::vector<std::string> line_split = split(line,' ');

            std::vector<int> abundance_vector;

            int m = line_split.size();
            std::string kmer = line_split[0]

            for (int j=1;j<m;j++)
            {
                abundance_vector.push_back(stoi(line_split[j]));
            }

            i++;
        }
    }
    else
    {
        std::cout << "ERR0R: Failed to open the file." << std::endl;
    }
}