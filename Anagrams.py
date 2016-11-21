from collections import defaultdict
import os
import time

def loadDictionary():
    with open(os.path.join("src", "main", "resources", "forcomp", "linuxwords.txt")) as f:
        return [line.rstrip() for line in f.readlines()]

def wordOccurrences(w):
    occ = defaultdict(int)
    for c in w:
        c = c.lower()
        occ[c] += 1

    l = ()
    for k in sorted(occ.keys()):
        l += ((k, occ[k]),)
    return l

def sentenceOccurrences(s):
    return wordOccurrences(''.join(s))

def readDictionaryByOccurrences():
    mo = defaultdict(tuple)
    for w in loadDictionary():
        occ = wordOccurrences(w)
        mo[occ] += (w,)
    return mo

def wordAnagrams(word):
    dictionaryByOccurrences[wordOccurrences(word)]

def combinations(occurrences):
    if len(occurrences) == 0:
        return [()]
    c,maxo = occurrences[0]
    crest = combinations(occurrences[1:])
    return [((c,o),) + cr for o in range(1, maxo+1) for cr in crest] + crest

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

class Stopwatch:
    def __init__(self):
        self.before = time.time()
    def time(self, descr):
        now = time.time()
        print("%.2fs %s" % (now - self.before, descr))
        self.before = time.time()

sw = Stopwatch()
dictionaryByOccurrences = readDictionaryByOccurrences()
sw.time("reading dict")
for i in range(3):
    a = sentenceAnagrams(["bold","grumpy","cat"])
    assert(len(a) == 30198)
    sw.time("finding anagrams")
