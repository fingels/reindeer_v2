#include <iostream>
#include <string>
#include <cmath>
#include <chrono>
#include <filesystem>
#include <vector>
#include <unordered_map>
#include <limits>

#include "utils.hpp"
#include "files.hpp"
#include "strings.hpp"
#include "CLI11.hpp"

using namespace std;

int main(int argc, char *argv[])
{
   Color::Modifier blue(Color::FG_BLUE);
   Color::Modifier red(Color::FG_RED);
   Color::Modifier def(Color::FG_DEFAULT);

   using chrono::duration_cast;
   using chrono::high_resolution_clock;
   using chrono::milliseconds;

   SpaceSeparator facet(1); // 1 - don't delete when done
   cout.imbue(std::locale(std::locale(), &facet));

   // Parameters

   CLI::App app{"Counting kmers from raw files"};
   // argv = app.ensure_utf8(argv);

   int k;
   app.add_option("-k",k,"Length of k-mers");

   int m;
   app.add_option("-m",m,"Length of minimizers");

   double cap = numeric_limits<double>::infinity();
   app.add_option("-c,--cap",cap,"Maximum number of lines to parse per file");

   string folder;
   app.add_option("-f,--folder",folder,"Folder where to find raw files");

   bool display = true;
   app.add_option("-d,--display",display,"Whether or not display executions comments");

   CLI11_PARSE(app, argc, argv);

   // End of parameters

   int nbFile = 0;
   unordered_map<int, int> globalClassesKmerAbundance;           // classes globales -> nb de kmers avec cette classe
   unordered_map<int, vector<double>> globalClassesCountVectors; // classes globables -> vecteurs de comptage
   int globalClassIndex = 0;

   if (display)
   {
      cout << "Parameters" << endl;
      cout << "\tk-mer size: " << blue << k << def << endl;
      cout << "\tminimizer size: " << blue << m << def << endl;
      cout << "\tMax number of strings read on each file: " << blue << cap << def << endl;
      cout << "**************" << endl;
   }

   auto t1 = high_resolution_clock::now(); // Start measuring time

   for (const auto &entry : filesystem::directory_iterator(folder))
   {
      nbFile++;

      if (display)
      {
         cout << "Processing file\t" << red << entry.path() << def << endl;
      }

      // ouvrir fichier

      vector<string> unitigs;
      vector<double> counts;
      minimizer_map minimizerTable;

      readFasta(entry.path(), cap, unitigs, counts);

      if (display)
      {
         cout << "\tSuccessfully read file." << endl;
      }

      // on récupère les kmer du fichier et leurs valeurs de comptage
      unitigsToKmersWithCounts(k, m, unitigs, counts, minimizerTable);

      int nbKmer = 0;

      if (display)
      {
         for (auto x : minimizerTable)
         {
            nbKmer += minimizerTable[x.first].size();
         }

         cout << "\tFound " << blue << minimizerTable.size() << def << " minimizers for a total of " << blue << nbKmer << def << " k-mers." << endl;
      }

      // on associe des classes d'équivalence locales aux comptages

      unordered_map<double, int> countToClasses;    // valeurs de comptage -> classes locales
      unordered_map<int, int> classesKmerAbundance; // classes locales -> nb de kmers avec cette classe
      unordered_map<int, double> classesToCounts;   // classes locales -> valeurs de comptage

      for (auto x : minimizerTable)
      {
         assignEquivalenceClasses(minimizerTable[x.first], countToClasses, classesKmerAbundance, classesToCounts);
      }

      if (display)
      {
         cout << "\tFound " << blue << classesToCounts.size() << def << " classes of counts." << endl;
      }

      if (nbFile == 1)
      {
         /* C'est le premier fichier qu'on traite */

         if (!filesystem::is_directory("../tmp") || !filesystem::exists("../tmp"))
         {                                          // Check if tmp folder exists
            filesystem::create_directory("../tmp"); // create tmp folder
         }

         // On crée un fichier par minimizer

         for (auto x : minimizerTable)
         {
            saveKmerCountMap(minimizerTable[x.first], "../tmp/" + x.first + ".txt", countToClasses);
         }

         // On crée les classes globales
         for (auto x : classesKmerAbundance)
         {
            globalClassesKmerAbundance[x.first] = x.second;
            globalClassesCountVectors[x.first].push_back(classesToCounts[x.first]);
            globalClassIndex++;
         }

         if (display)
         {
            cout << "Successfully created " << blue << minimizerTable.size() << def << " minimizers files and registered " << blue << nbKmer << def << " k-mers." << endl;
         }
      }
      else
      {
         // les classes existantes sont conservées avec une valeur de comptage de 0 dans le fichier courant
         for (auto x : globalClassesCountVectors)
         {
            globalClassesCountVectors[x.first].push_back(0);
         }

         int newFileCount = 0; // nombre de nouveaux fichiers de minimizers créés
         int newKmerCount = 0; // nombre de nouveaux kmers enregistrés

         unordered_map<int, int> localToGlobalClassesWith0; // classes de la forme (0,...,0,x)
         couple_map localToGlobalClassesWithNon0;           // classes de la forme (x1,...,xn,x)

         for (auto x : minimizerTable)
         {
            // on parcoure chaque minimizer local

            if (!filesystem::exists("../tmp/" + x.first + ".txt"))
            {
               // le minimizer n'a pas encore été vu

               newFileCount++;

               // on crée les nouvelles classes globales
               for (auto y : classesKmerAbundance)
               {
                  // y.first : classe locale

                  if (localToGlobalClassesWith0.count(y.first) == 0)
                  {
                     localToGlobalClassesWith0[y.first] = globalClassIndex;
                     globalClassesKmerAbundance[globalClassIndex] = classesKmerAbundance[y.first];

                     for (int j = 1; j < nbFile; j++)
                     {
                        globalClassesCountVectors[globalClassIndex].push_back(0);
                     }
                     globalClassesCountVectors[globalClassIndex].push_back(classesToCounts[y.first]);
                     globalClassIndex++;
                  }
               }

               kmer_class_map kmerClassMap;

               for (auto y : minimizerTable[x.first])
               {
                  // y.first : kmer

                  kmerClassMap[y.first] = localToGlobalClassesWith0[countToClasses[minimizerTable[x.first][y.first]]];
                  newKmerCount++;
               }

               saveKmerClassMap(kmerClassMap, "../tmp/" + x.first + ".txt");
            }
            else
            {
               // le minimizer a déjà été vu

               // on charge en mémoire le fichier du minimizer
               kmer_class_map kmerClassMap;
               readMinimizerFile("../tmp/" + x.first + ".txt", kmerClassMap);

               // on parcoure les kmer locaux
               for (auto y : minimizerTable[x.first])
               {
                  // y.first : kmer
                  int localClass = countToClasses[y.second];

                  if (kmerClassMap.count(y.first) == 0)
                  {
                     // le kmer n'a jamais été vu auparavant
                     newKmerCount++;

                     if (localToGlobalClassesWith0.count(localClass) == 0)
                     {
                        // on crée une nouvelle classe globale
                        localToGlobalClassesWith0[localClass] = globalClassIndex;
                        globalClassesKmerAbundance[globalClassIndex] = classesKmerAbundance[localClass];

                        for (int j = 1; j < nbFile; j++)
                        {
                           globalClassesCountVectors[globalClassIndex].push_back(0);
                        }
                        globalClassesCountVectors[globalClassIndex].push_back(classesToCounts[localClass]);
                        globalClassIndex++;
                     }

                     kmerClassMap[y.first] = localToGlobalClassesWith0[localClass];
                  }
                  else
                  {

                     int globalClass = kmerClassMap[y.first];

                     couple key = make_tuple(localClass, globalClass);

                     if (localToGlobalClassesWithNon0.count(key) == 0)
                     {
                        // on crée une nouvelle classe globale
                        globalClassesKmerAbundance[globalClassIndex] = 0;
                        for (int j = 0; j < globalClassesCountVectors[globalClass].size() - 1; j++)
                        {
                           globalClassesCountVectors[globalClassIndex].push_back(globalClassesCountVectors[globalClass][j]);
                        }
                        globalClassesCountVectors[globalClassIndex].push_back(classesToCounts[localClass]);

                        localToGlobalClassesWithNon0[key] = globalClassIndex;
                        globalClassIndex++;
                     }

                     // on déplace le compte du kmer de son ancienne vers sa nouvelle classe
                     kmerClassMap[y.first] = localToGlobalClassesWithNon0[key];

                     globalClassesKmerAbundance[globalClass]--;
                     globalClassesKmerAbundance[localToGlobalClassesWithNon0[key]]++;

                     // s'il n'y a plus aucun kmer dans l'ancienne classe, on peut la supprimer

                     if (globalClassesKmerAbundance[globalClass] == 0)
                     {
                        globalClassesKmerAbundance.erase(globalClass);
                        globalClassesCountVectors.erase(globalClass);
                     }
                  }
               }

               // on réenregistre le fichier du minimizer
               saveKmerClassMap(kmerClassMap, "../tmp/" + x.first + ".txt");
            }
         }

         // saveGlobalClasses("tmp/0_final_classes.txt",globalClassesKmerAbundance,globalClassesCountVectors);
         if (display)
         {
            cout << "Successfully updated classes and k-mers, creating " << blue << newFileCount << def << " new minimizers files and registering " << blue << newKmerCount << def << " new k-mers." << endl;
         }
      }

      if (display)
      {
         cout << "**************" << endl;
      }
   }

   // Espace d'expérimentation à détruire à la fin

   // Fin de l'espace d'expérimentation

   auto t2 = high_resolution_clock::now(); // End measuring time
   std::chrono::duration<float> fs = t2 - t1;

   saveGlobalClasses("../tmp/0_final_classes.txt", globalClassesKmerAbundance, globalClassesCountVectors);

   if (display)
   {
      cout << "Done in " << blue << fs.count() << def << "s" << endl;
   }

   return 0;
}