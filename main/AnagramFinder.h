#pragma once

#include <fstream>
#include <iostream>
#include <iterator>
#include <list>
#include <map>
#include <string>
#include <vector>

/** A word is simply a `String`. */
using Word = std::string;

/** A sentence is a `List` of words. */
using Sentence = std::vector<Word>;

/** `Occurrences` is a `List` of pairs of characters and positive integers saying
* how often the character appears.
* This list is sorted alphabetically w.r.t. to the character in each pair.
* All characters in the occurrence list are lowercase.
*
* Any list of pairs of lowercase characters and their frequency which is not sorted
* is **not** an occurrence list.
*
* Note: If the frequency of some character is zero, then that character should not be
* in the list.
*/
using Occurrences = std::vector<std::pair<char, int>>;

struct AnagramFinder {
    static std::vector<Word> loadDictionary() {
        std::vector<Word> result;
        std::ifstream f("C:\\Users\\stein\\Documents\\Scala\\IdeaProjects\\forcomp\\src\\main\\resources\\forcomp\\linuxwords.txt");
        std::string line;
        while (std::getline(f, line)) {
            result.push_back(line);
        }
        return result;
    }

    /** The dictionary is simply a sequence of words.
    * It is predefined and obtained as a sequence using the utility method `loadDictionary`.
    */
    std::vector<Word> dictionary = loadDictionary();

    /** Converts the word into its character occurrence list.
    *
    * Note: the uppercase and lowercase version of the character are treated as the
    * same character, and are represented as a lowercase character in the occurrence list.
    *
    * Note: you must use `groupBy` to implement this method!
    */
    static void wordOccurrences(Word const& w, Occurrences& result) {
        _ASSERT(result.empty());

        std::map<char, int> occ;
        for (char c : w) {
            c = tolower(c);
            occ[c] += 1;
        }

        result.reserve(occ.size());
        std::copy(occ.begin(), occ.end(), std::back_inserter(result));
        //for (auto const& kv : occ) {
        //    result.push_back(kv);
        //}
    }

    /** Converts a sentence into its character occurrence list. */
    static void sentenceOccurrences(Sentence const& s, Occurrences& result) {
        Word superWord;
        for (Word const& w : s) {
            superWord += w;
        }
        wordOccurrences(superWord, result);
    }

    /** The `dictionaryByOccurrences` is a `Map` from different occurrences to a sequence of all
    * the words that have that occurrence count.
    * This map serves as an easy way to obtain all the anagrams of a word given its occurrence list.
    *
    * For example, the word "eat" has the following character occurrence list:
    *
    * `List(('a', 1), ('e', 1), ('t', 1))`
    *
    * Incidentally, so do the words "ate" and "tea".
    *
    * This means that the `dictionaryByOccurrences` map will contain an entry:
    *
    * List(('a', 1), ('e', 1), ('t', 1)) -> Seq("ate", "eat", "tea")
    *
    */
    std::map<Occurrences, std::vector<Word>> dictionaryByOccurrences;
    std::map<Occurrences, std::vector<Word>> const& dbo = dictionaryByOccurrences;

    AnagramFinder() {
        readDictionaryByOccurrences();
    }

    void readDictionaryByOccurrences() {
        for (Word const& w : dictionary) {
            Occurrences occ;
            wordOccurrences(w, occ);
            dictionaryByOccurrences[occ].push_back(w);
        }
    }

    /** Returns all the anagrams of a given word. */
    std::vector<Word> const& wordAnagrams(Word const& w) const {
        Occurrences occ;
        wordOccurrences(w, occ);
        auto it = dictionaryByOccurrences.find(occ);
        _ASSERT(it != dictionaryByOccurrences.end());
        return (*it).second;
    }

    /** Returns the list of all subsets of the occurrence list.
    * This includes the occurrence itself, i.e. `List(('k', 1), ('o', 1))`
    * is a subset of `List(('k', 1), ('o', 1))`.
    * It also include the empty subset `List()`.
    *
    * Example: the subsets of the occurrence list `List(('a', 2), ('b', 2))` are:
    *
    * List(
    * List(),
    * List(('a', 1)),
    * List(('a', 2)),
    * List(('b', 1)),
    * List(('a', 1), ('b', 1)),
    * List(('a', 2), ('b', 1)),
    * List(('b', 2)),
    * List(('a', 1), ('b', 2)),
    * List(('a', 2), ('b', 2))
    * )
    *
    * Note that the order of the occurrence list subsets does not matter -- the subsets
    * in the example above could have been displayed in some other order.
    */
    static void combinations(Occurrences::const_iterator occurrencesB, Occurrences::const_iterator occurrencesE, std::vector<Occurrences>& result) {
        if (occurrencesB == occurrencesE) {
            result.push_back(Occurrences());
            return;
        }
        char c = (*occurrencesB).first;
        int maxo = (*occurrencesB).second;
        std::vector<Occurrences> crest;
        combinations(occurrencesB + 1, occurrencesE, crest);
        result.reserve(crest.size() * (1 + maxo));
        std::copy(crest.begin(), crest.end(), std::back_inserter(result));
        for (int o = 1; o <= maxo; ++o) {
            for (auto const& cr : crest) {
                Occurrences newOcc;
                newOcc.reserve(1 + cr.size());
                newOcc.push_back(std::make_pair(c, o));
                std::copy(cr.begin(), cr.end(), std::back_inserter(newOcc));
                result.push_back(newOcc);
            }
        }
    }

    /** Subtracts occurrence list `y` from occurrence list `x`.
    *
    * The precondition is that the occurrence list `y` is a subset of
    * the occurrence list `x` -- any character appearing in `y` must
    * appear in `x`, and its frequency in `y` must be smaller or equal
    * than its frequency in `x`.
    *
    * Note: the resulting value is an occurrence - meaning it is sorted
    * and has no zero-entries.
    */
    static void subtract(Occurrences& x, Occurrences::iterator xB, Occurrences::const_iterator yB, Occurrences::const_iterator yE) {
        if (yB == yE) return;
        _ASSERT(xB != x.end());
        char xc = (*xB).first;
        int xo = (*xB).second;
        char yc = (*yB).first;
        int yo = (*yB).second;
        if (xc == yc) {
            if (xo == yo) {
                xB = x.erase(xB);
                subtract(x, xB, yB + 1, yE);
            } else {
                (*xB).second -= yo;
                subtract(x, xB + 1, yB + 1, yE);
            }
        } else {
            subtract(x, xB + 1, yB, yE);
        }
    }

    /** Returns a list of all anagram sentences of the given sentence.
    *
    * An anagram of a sentence is formed by taking the occurrences of all the characters of
    * all the words in the sentence, and producing all possible combinations of words with those characters,
    * such that the words have to be from the dictionary.
    *
    * The number of words in the sentence and its anagrams does not have to correspond.
    * For example, the sentence `List("I", "love", "you")` is an anagram of the sentence `List("You", "olive")`.
    *
    * Also, two sentences with the same words but in a different order are considered two different anagrams.
    * For example, sentences `List("You", "olive")` and `List("olive", "you")` are different anagrams of
    * `List("I", "love", "you")`.
    *
    * Here is a full example of a sentence `List("Yes", "man")` and its anagrams for our dictionary:
    *
    * List(
    * List(en, as, my),
    * List(en, my, as),
    * List(man, yes),
    * List(men, say),
    * List(as, en, my),
    * List(as, my, en),
    * List(sane, my),
    * List(Sean, my),
    * List(my, en, as),
    * List(my, as, en),
    * List(my, sane),
    * List(my, Sean),
    * List(say, men),
    * List(yes, man)
    * )
    *
    * The different sentences do not have to be output in the order shown above - any order is fine as long as
    * all the anagrams are there. Every returned word has to exist in the dictionary.
    *
    * Note: in case that the words of the sentence are in the dictionary, then the sentence is the anagram of itself,
    * so it has to be returned in this list.
    *
    * Note: There is only one anagram of an empty sentence.
    */
    void subsentencesAfterPrefix(Occurrences const& occurrences, Occurrences const& suboccurrences, std::list<Sentence>& result) const {
        Occurrences remainingOccurrences = occurrences;
        subtract(remainingOccurrences, remainingOccurrences.begin(), suboccurrences.begin(), suboccurrences.end());
        if (remainingOccurrences.empty()) {
            result.push_back(std::vector<Word>());
        } else {
            subsentences(remainingOccurrences, result);
        }
    }

    void subsentences(Occurrences const& occurrences, std::list<Sentence>& result) const {
        std::vector<Occurrences> oc;
        combinations(occurrences.begin(), occurrences.end(), oc);
        for (auto const& o : oc) {
            auto it = dbo.find(o);
            if (it != dbo.end()) {
                for (Word const& word : (*it).second) {
                    std::list<Sentence> subsentences;
                    subsentencesAfterPrefix(occurrences, o, subsentences);
                    for (auto const& subsentence : subsentences) {
                        Sentence sentence;
                        sentence.push_back(word);
                        std::copy(subsentence.begin(), subsentence.end(), std::back_inserter(sentence));
                        result.push_back(sentence);
                    }
                }
            }
        }
    }

    void sentenceAnagrams(Sentence const& sentence, std::list<Sentence>& result) const {
        Occurrences so;
        sentenceOccurrences(sentence, so);

        subsentences(so, result);
        //            if (l == Nil) List(List()) else l
    }

    void assertAnagrams(Sentence const& sentence, int count) const {
        std::list<Sentence> anagrams;
        sentenceAnagrams(sentence, anagrams);
        if (anagrams.size() != count) {
            for (auto s : anagrams) {
                for (auto w : s) {
                    std::cout << w << " ";
                }
                std::cout << "\n";
            }
        }
        _ASSERT(anagrams.size() == count);
    }
};
