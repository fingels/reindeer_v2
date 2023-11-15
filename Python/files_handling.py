from strings_handling import *
import sys
from trie import *


def read_fasta(file_name: str, cap: int):
    content = []

    with open(file_name, 'r') as f:
        lines = list(f)
        assert len(lines) % 2 == 0, 'FASTA files has odd number of lines'

        for i in range(min(len(lines) // 2, cap)):
            count = float(lines[2 * i].split(' ')[3].split(':')[2])
            string = lines[2 * i + 1].split('\n')[0]

            content.append((string, count))

    return content


# def fasta_to_kmer_count(content: tuple[str, float], k: int, m: int):
#     minimizer_dict = {}
#
#     for string, count in content:
#
#         kmer_list = slice(string, k)
#
#         for kmer in kmer_list:
#
#             i = find_minimizer(kmer, m)
#
#             if kmer[i:i + m] not in minimizer_dict.keys():
#                 minimizer_dict[kmer[i:i + m]] = Trie()
#
#             minimizer_dict[kmer[i:i + m]].insert(kmer[:i] + '@' + kmer[i + 1:], count)
#
#     return minimizer_dict

def fasta_to_kmer_count(content: tuple[str, float], k: int, m: int):
    minimizer_dict = {}

    for string, count in content:

        kmer_list = slice(string, k)

        for kmer in kmer_list:

            i = find_minimizer(kmer, m)

            minimizer = kmer[i:i + m]
            remainder = kmer[:i] + '@' + kmer[i + 1:]

            if minimizer not in minimizer_dict.keys():
                minimizer_dict[minimizer] = dict()

            if remainder not in minimizer_dict[minimizer]:
                minimizer_dict[minimizer][remainder]=count
            else:
                minimizer_dict[minimizer][remainder]+=count

    return minimizer_dict


def file_to_dict(filename):

    dic = {}

    with open(filename,'r') as f:
        for line in f.readlines():
            kmer, equiv_class = line.split('\t')
            equiv_class = int(equiv_class[:-1])
            dic[kmer] = equiv_class

    return dic

def dict_to_file(dic,filename):

    with open(filename,'w') as f:
        for kmer in dic.keys():
            f.write(kmer+'\t'+str(dic[kmer])+'\n')
