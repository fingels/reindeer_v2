compl = {'A': 'T', 'T': 'A', 'C': 'G', 'G': 'C'}

def canonical(string: str):
    '''
    Given the correspondance A<->T and C<->G, returns whichever is the smallest (w.r.t. the lexicographical order) between `string` and its reverse complement
    '''

    for i in range(len(string)):
        if string[i] < compl[string[-i - 1]]:
            return string
        elif compl[string[-i - 1]] < string[i]:
            return ''.join([compl[string[-i - 1]] for i in range(len(string))])
    return string

def slice(string: str, k: int,canon:bool=True):
    '''
    Slice `string` in bits of size `k`, overlapping over `k-1` letters. If the option `canon` is True, returns the canonical version of each `k`-mer instead.

    See also canonical(...)
    '''
    slice_list = []
    for i in range(len(string) - k + 1):
        if canon:
            slice_list.append(canonical(string[i:i + k]))
        else:
            slice_list.append(string[i:i + k])
    return slice_list


# def find_minimizer(string: str, m: int,canon:bool=True):
#     '''
#     Returns the smallest (w.r.t. the lexicographical order) substring of size `m` of `string`. If the option `canon`is True, compute also the minimizer on the reverse complement and returns whichever is the smallest between the two minimizers.
#     '''
#     assert len(string) >= m, 'String is too short'
#
#     #TODO : refactor to return starting index of minimizer + boolean for reverse complement or not
#
#     index_min = 0
#     for i in range(1, len(string) - m + 1):
#         for j in range(m):  # on parcoure string[i:i+m] tant que les lettres coîncident avec string[index_min:index_min+m]
#             if string[i + j] < string[index_min + j]:
#                 index_min = i
#                 break
#             elif string[index_min + j] < string[i + j]:
#                 break
#
#     if canon:
#         # Compute the minimizer on the reverse complement
#         reverse_index_min=0
#         for i in range(1,len(string)-m+1):
#             for j in range(m):
#                 if compl[string[-i-j-1]]<compl[string[-reverse_index_min-j-1]]:
#                     reverse_index_min = i
#                     break
#                 elif compl[string[-reverse_index_min-j-1]] < compl[string[-i-j-1]]:
#                     break
#
#         # Finds the smallest between minimizer and reverse complement minimizer
#         for j in range(m):
#             if string[index_min+j]<compl[string[-reverse_index_min-j-1]]:
#                 return string[index_min:index_min + m]
#             elif compl[string[-reverse_index_min-j-1]]<string[index_min+j]:
#                 return ''.join([compl[string[-reverse_index_min-i - 1]] for i in range(m)])
#
#         return string[index_min:index_min + m]
#     else:
#         return string[index_min:index_min + m]

def find_minimizer(string: str, m: int):
    '''
    Returns the index where the smallest (w.r.t. the lexicographical order) substring of size `m` of `string` begins.
    '''
    assert len(string) >= m, 'String too short'

    index_min = 0
    for i in range(1, len(string) - m + 1):
        for j in range(m):  # on parcoure string[i:i+m] tant que les lettres coîncident avec string[index_min:index_min+m]
            if string[i + j] < string[index_min + j]:
                index_min = i
                break
            elif string[index_min + j] < string[i + j]:
                break

    return index_min

