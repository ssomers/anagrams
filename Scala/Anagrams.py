from collections import defaultdict
import cProfile
import os
import timeit
import unittest

def loadDictionary():
    with open(os.path.join("src", "main", "resources", "forcomp", "linuxwords.txt")) as f:
        return [line.rstrip() for line in f.readlines()]

def wordOccurrences(w):
    occ = defaultdict(int)
    for c in w:
        #if c.isalpha():
        c = c.lower()
        occ[c] += 1

    l = ()
    for k in sorted(occ.keys()):
        l += ((k, occ[k]),)
    return l

def sentenceOccurrences(s):
    return wordOccurrences(''.join(s))

dictionaryByOccurrences = None

def readDictionaryByOccurrences():
    #print("readDictionaryByOccurrences")
    global dictionaryByOccurrences
    dictionaryByOccurrences = defaultdict(tuple)
    for w in loadDictionary():
        occ = wordOccurrences(w)
        dictionaryByOccurrences[occ] += (w,)

def wordAnagrams(word):
    dictionaryByOccurrences[wordOccurrences(word)]

def combinations(occurrences):
    if len(occurrences) == 0:
        return [()]
    c,maxo = occurrences[0]
    crest = combinations(occurrences[1:])
    return crest + [((c,o),) + cr for o in range(1, maxo+1) for cr in crest]

def subtract(x, y):
    if len(y) == 0:
        return x
    xc, xo = x[0]
    yc, yo = y[0]
    if xc != yc:
        return ((xc, xo),) + subtract(x[1:], y)
    elif xo != yo:
        return ((xc, xo - yo),) + subtract(x[1:], y[1:])
    else:
        return subtract(x[1:], y[1:])

def sentenceAnagrams(sentence):
    so = sentenceOccurrences(sentence)
    dbo = dictionaryByOccurrences

    def subsentencesAfterPrefix(occurrences, suboccurrences):
        remainingOccurrences = subtract(occurrences, suboccurrences)
        if len(remainingOccurrences) == 0:
            return [[]]
        else:
            return subsentences(remainingOccurrences)

    def subsentences(occurrences):
        oc = combinations(occurrences)
        return [ [word] + subsentence for o in oc for word in dbo.get(o, []) for subsentence in subsentencesAfterPrefix(occurrences, o)]

    l = subsentences(so)
    if len(l) == 0:
        return [[]]
    else:
        return l

class TestAnagrams(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        readDictionaryByOccurrences()

    def test_ate(self):
        self.assertEqual(sentenceAnagrams(["eat"]), [["ate"], ["eat"], ["tea"]])
    def test_linux_rulez(self):
        a = sentenceAnagrams(["linux","rulez"])
        self.assertEqual(len(a), 20)
    def test_cat(self):
        a = sentenceAnagrams(["grumpy","cat"])
        self.assertEqual(len(a), 164)
    def test_grumpy_bold_cat(self):
        a = sentenceAnagrams(["grumpy","bold","cat"])
        self.assertEqual(len(a), 30198)
    def test_repetitive(self):
        a = sentenceAnagrams(["repetitive"])
        self.assertEqual(len(a), 529)

#unittest.main()
if __name__ == "__main__":
    for s in timeit.repeat(readDictionaryByOccurrences, number=1, repeat=1):
        print("{:6,.3f}s prep".format(s))

    for N,sentence in [(100, ["grumpy","cat"]), (1, ["bold","grumpy","cat"])]:
        for s in timeit.repeat(lambda: sentenceAnagrams(sentence), number=N, repeat=(N > 1 and 3 or 1)):
            print("{:6,.3f}s {:}".format(s / N, ' '.join(sentence)))
