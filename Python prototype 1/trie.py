# Adapted from https://albertauyeung.github.io/2020/06/15/python-trie.html/
import itertools
# TODO : implement compression (radix trie)
# TODO : développer save/read methods pour les trie

id_count = itertools.count()

class TrieNode:
    """A node in the trie structure"""

    def __init__(self, char):
        # the character stored in this node
        self.char = char

        # whether this can be the end of a word
        self.is_end = False

        # a counter indicating how many times a word is inserted
        # (if this node's is_end is True)
        self.counter = 0

        # a dictionary of child nodes
        # keys are characters, values are nodes
        self.children = {}

        self.my_id = next(id_count)


class Trie(object):
    """The trie object"""

    def __init__(self):
        """
        The trie has at least the root node.
        The root node does not store any character
        """
        self.root = TrieNode("")

        self.size = 1
        self.leaves = set()

    def insert(self, word, count):
        """Insert a word into the trie"""
        node = self.root

        # Loop through each character in the word
        # Check if there is no child containing the character, create a new child for the current node
        for char in word:
            if char in node.children:
                node = node.children[char]
            else:
                # If a character is not found,
                # create a new node in the trie
                new_node = TrieNode(char)
                node.children[char] = new_node
                node = new_node
                self.size += 1

        # Mark the end of a word
        node.is_end = True
        self.leaves.add(node)

        # Increment the counter to indicate that we see this word once more
        node.counter += count

    def dfs(self, node, prefix):
        """Depth-first traversal of the trie

        Args:
            - node: the node to start with
            - prefix: the current prefix, for tracing a
                word while traversing the trie
        """
        if node.is_end:
            self.output.append((prefix + node.char, node.equivalence_class))

        for child in node.children.values():
            self.dfs(child, prefix + node.char)

    def query(self, x):
        """Given an input (a prefix), retrieve all words stored in
        the trie with that prefix, sort the words by the number of
        times they have been inserted
        """
        # Use a variable within the class to keep all possible outputs
        # As there can be more than one word with such prefix
        self.output = []
        node = self.root

        # Check if the prefix is in the trie
        for char in x:
            if char in node.children:
                node = node.children[char]
            else:
                # cannot found the prefix, return empty list
                return []

        # Traverse the trie to get all candidates
        self.dfs(node, x[:-1])

        # Sort the results in reverse order and return
        return sorted(self.output, key=lambda x: x[0])

    def assign_equivalence_class_to_leaves(self,classes:dict[float,list[int,int]]=None):

        if classes is None:
            classes = {}

        if len(classes.keys()) > 0:
            i = max([classes[count][0] for count in classes.keys()])+1
        else:
            i = 0

        for leaf in self.leaves:
            if leaf.counter not in classes.keys():
                classes[leaf.counter] = [i, 0]
                i += 1

            leaf.equivalence_class = classes[leaf.counter][0]
            classes[leaf.counter][1] += 1

        return classes

    def enumerate(self):
        return self.query('A') + self.query('C') + self.query('G') + self.query('T') + self.query('@')

    def to_file(self,filename):
        with open(filename, "w") as f:
            for kmer,equiv_class in self.enumerate():
                f.write(kmer+'\t'+str(equiv_class)+'\n')