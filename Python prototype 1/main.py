from utils import *
from files_handling import *
import os
import time
from copy import copy
import itertools

# Press the green button in the gutter to run the script.
if __name__ == '__main__':

    k = 21
    m = 15
    # cap = float('Inf')
    cap = 100000
    #jusqu'à 100 000 c'est rapide, après c'est long
    # 300 000 | 100s ~2m
    # 500 000 | 285s ~4m
    # 1 000 000 | 792s ~13m
    # 10 000 000 | 1064s ~18m
    # 100 000 000 | 1067s ~18m
    # all of them | 1164s ~20m


    global_classes_number = itertools.count()
    global_classes = {}

    tim = time.time()

    nb_file = 0

    print('Parameters')
    print('\tk-mer size: ' + string_color(k,'blue'))
    print('\tminimizer size: ' + string_color(m, 'blue'))
    print('\tMax number of strings read on each file: ' + string_color(cap,'blue'))
    print('**************')

    for file in os.listdir('unitigs'):
        nb_file+=1
        print('Processing file\t' + string_color(file,'header'))

        file_content = read_fasta('unitigs/' + file, cap)

        print('\tSuccessfully read file.')

        # range dans un dictionnaire tous les kmer du fichier et leur valeur de comptage; les clés sont les minimiseurs et les valeurs des tries
        minimizer_dict = fasta_to_kmer_count(file_content, k, m)

        nb_kmers = 0
        # nb_nodes = 0

        for minimizer in minimizer_dict.keys():
            # nb_nodes += minimizer_dict[minimizer].size
            # nb_kmers += len(minimizer_dict[minimizer].leaves)
            nb_kmers += len(minimizer_dict[minimizer].keys())

        # print('\tFound ' + string_color(len(minimizer_dict.keys()),'blue')+' minimizers for a total of ' + string_color(nb_kmers,'blue')+
        #       ' k-mers and ' + string_color(nb_nodes,'blue') +' nodes in the different tries.')

        print('\tFound ' + string_color(len(minimizer_dict.keys()),'blue')+' minimizers for a total of ' + string_color(nb_kmers,'blue')+
              ' k-mers.')

        file_classes = {}

        for minimizer in minimizer_dict.keys():
            file_classes = assign_equivalence_class(minimizer_dict[minimizer],file_classes)
            # file_classes = minimizer_dict[minimizer].assign_equivalence_class_to_leaves(file_classes)

        file_classes = inverse_classes(file_classes)

        print('\tFound '+string_color(len(file_classes),'blue')+' classes of counts.')

        # CLASSES
        #   chaque kmer est associé à une classe
        #   chaque classe est associé à un nombre de kmer (et un vecteur de comptage)
        #   quand on retire un kmer d'une classe, on diminue le compteur
        #   si le compteur arrive à 0, on supprime la classe

        if nb_file == 1:
            # C'est le premier fichier qu'on traite

            os.makedirs('tmp',exist_ok=True)

            for minimizer in minimizer_dict.keys():
                dict_to_file(minimizer_dict[minimizer],'tmp/'+minimizer)
                # minimizer_dict[minimizer].to_file('tmp/'+minimizer)

            for local_class in file_classes.keys():
                kmer_count, count = file_classes[local_class]
                global_classes[next(global_classes_number)] = [kmer_count] + [0] * (nb_file - 1) + [count]

            print('Successfully created '+string_color(len(minimizer_dict.keys()),'blue')+' minimizers files and registered ' +string_color(nb_kmers,'blue')+ ' kmers.')
        else:
            # les classes existantes sont conservées avec une valeur de comptage de 0 dans le fichier
            for equiv_class in global_classes.keys():
                global_classes[equiv_class].append(0)

            new_file_count = 0 # compte le nombre de nouveaux fichiers de minimizers créés
            new_kmer_count = 0 # compte le nombre de nouveaux kmer enregistrés

            local_to_global_classes_with_0 = {} # classes de la forme (0,...,0,x)
            local_and_global_classes_with_non_0 = {} #classes de la forme (x1,...,xn,x)

            for minimizer in minimizer_dict.keys():
                if not os.path.isfile('tmp/'+minimizer):
                    #le minimizer n'a pas encore été vu
                    new_file_count += 1

                    # on crée les nouvelles classes globales
                    for local_class in file_classes.keys():
                        if local_class not in local_to_global_classes_with_0.keys():
                            kmer_count,count = file_classes[local_class]

                            global_class_id = next(global_classes_number)
                            global_classes[global_class_id] = [kmer_count] + [0] * (nb_file - 1) + [count]
                            local_to_global_classes_with_0[local_class] = global_class_id

                    # on modifie les classes directement sur le trie / dict
                    # for leaf in trie_dict[minimizer].leaves:
                    #     leaf.equivalence_class = local_to_global_classes_with_0[leaf.equivalence_class]
                    #     new_kmer_count+=1

                    for kmer in minimizer_dict[minimizer].keys():
                        minimizer_dict[minimizer][kmer] = local_to_global_classes_with_0[minimizer_dict[minimizer][kmer]]
                        new_kmer_count += 1

                    # on crée le nouveau fichier de minimizer
                    dict_to_file(minimizer_dict[minimizer], 'tmp/' + minimizer)
                    # trie_dict[minimizer].to_file('tmp/' + minimizer)

                else:

                    kmer_dict = file_to_dict('tmp/'+minimizer)

                    # for kmer, local_class in minimizer_dict[minimizer].enumerate():
                    for kmer in minimizer_dict[minimizer].keys():

                        local_class = minimizer_dict[minimizer][kmer]

                        if kmer not in kmer_dict.keys():
                            # le kmer n'a pas été trouvé dans le fichier minimizer
                            new_kmer_count +=1

                            if local_class not in local_to_global_classes_with_0.keys():
                                kmer_count,count = file_classes[local_class]
                                global_class_id= next(global_classes_number)
                                global_classes[global_class_id] = [kmer_count] + [0] * (nb_file - 1) + [count]
                                local_to_global_classes_with_0[local_class]=global_class_id

                            kmer_dict[kmer]=local_to_global_classes_with_0[local_class]

                        else:

                            global_equiv_class = kmer_dict[kmer]

                            if (local_class,global_equiv_class) not in local_and_global_classes_with_non_0.keys():
                                # on crée la nouvelle classe d'équivalence
                                kmer_count, count = file_classes[local_class]

                                global_class_id = next(global_classes_number)
                                global_classes[global_class_id] = [0] + copy(global_classes[global_equiv_class][1:]) + [count]
                                local_and_global_classes_with_non_0[(local_class,global_equiv_class)] = global_class_id

                            #on déplace le compte du kmer de son ancienne vers sa nouvelle classe
                            global_classes[local_and_global_classes_with_non_0[(local_class,global_equiv_class)]][0] +=1
                            global_classes[global_equiv_class][0] -= 1

                            # il n'y a plus aucun kmer de l'ancienne classe, on peut la supprimer
                            if global_classes[global_equiv_class][0] == 0:
                                del global_classes[global_equiv_class]

                            kmer_dict[kmer] = local_and_global_classes_with_non_0[(local_class,global_equiv_class)]

                    dict_to_file(kmer_dict,'tmp/'+minimizer)

            save_global_classes('tmp/final_classes', global_classes)
            print('Successfully updated classes and kmers, creating '+string_color(new_file_count,'blue')+' new minimizers files and registering '+string_color(new_kmer_count,'blue')+' new kmers.')

        print('**************')

    save_global_classes('tmp/0_final_classes', global_classes)
    print('Done in ' + string_color(time.time() - tim,'blue') + ' seconds.')
