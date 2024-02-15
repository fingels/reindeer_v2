class bcolors:
    HEADER = '\033[95m'
    OKBLUE = '\033[94m'
    OKCYAN = '\033[96m'
    OKGREEN = '\033[92m'
    # WARNING = '\033[93m'
    # FAIL = '\033[91m'
    ENDC = '\033[0m'
    # BOLD = '\033[1m'
    # UNDERLINE = '\033[4m'

def string_color(input,color=None):

    match color:
        case 'header':
            prefix = bcolors.HEADER
        case 'blue':
            prefix = bcolors.OKBLUE
        case 'cyan':
            prefix = bcolors.OKCYAN
        case 'green':
            prefix = bcolors.OKGREEN
        case _:
            prefix = bcolors.ENDC

    if isinstance(input,str):
        return prefix+input+bcolors.ENDC
    elif isinstance(input,int):
        return prefix+'{:,}'.format(input).replace(',',' ')+bcolors.ENDC
    elif isinstance(input,float):
        return prefix+'{:.4f}'.format(input)+bcolors.ENDC
    else:
        return input

def inverse_classes(classes:dict[float,list[int,int]]):

    new_dict ={}

    for count in classes.keys():
        equiv_class, kmer_count = classes[count]
        new_dict[equiv_class] = [kmer_count,count]

    return new_dict

def assign_equivalence_class(dic:dict[str,float],classes:dict[float,list[int,int]]=None):
    if classes is None:
        classes = {}

    if len(classes.keys()) > 0:
        i = max([classes[count][0] for count in classes.keys()]) + 1
    else:
        i = 0

    for kmer in dic.keys():
        if dic[kmer] not in classes.keys():
            classes[dic[kmer]] = [i, 0]
            i += 1

        classes[dic[kmer]][1] += 1
        dic[kmer] = classes[dic[kmer]][0] # pas bien

    return classes

def save_global_classes(filename:str,file_classes:dict[int,list[int,float]]):
    with open(filename, "w") as f:

        f.write('#\t'+str(len(file_classes.keys()))+'\n')

        for key in file_classes.keys():
            kmer_count= file_classes[key][0]
            count_vector = file_classes[key][1:]

            string = str(key)+'\t'+str(kmer_count)+'\t'
            for count in count_vector:
                string += str(count)+';'

            f.write(string[:-1]+'\n')