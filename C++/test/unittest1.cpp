#include "../main/AnagramFinder.h"

#include <CppUnitTest.h>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

TEST_CLASS(ttAnagramFinder) {
private:
    AnagramFinder f;

public:
    TEST_METHOD(tea) {
        std::list<Sentence> result;
        f.sentenceAnagrams(Sentence{ "tea" }, result);
        Assert::AreEqual(size_t(3), result.size());
        auto it = result.begin();
        Assert::AreEqual(size_t(1), (*it).size());
        Assert::IsTrue((*it)[0] == "ate");
        ++it;
        Assert::AreEqual(size_t(1), (*it).size());
        Assert::IsTrue((*it)[0] == "eat");
        ++it;
        Assert::AreEqual(size_t(1), (*it).size());
        Assert::IsTrue((*it)[0] == "tea");
    }
    TEST_METHOD(LinuxRulez) {
        f.assertAnagrams(Sentence{ "Linux","rulez" }, 20);
    }
    TEST_METHOD(GrumpyCat) {
        f.assertAnagrams(Sentence{ "grumpy","cat" }, 164);
    }
    TEST_METHOD(GrumpyBoldCat) {
        f.assertAnagrams(Sentence{ "grumpy","bold","cat" }, 30198);
    }
    TEST_METHOD(Repetitive) {
        f.assertAnagrams(Sentence{ "repetitive" }, 529);
    }
#if _DEBUG
    BEGIN_TEST_METHOD_ATTRIBUTE(GrumpyCat) TEST_IGNORE() END_TEST_METHOD_ATTRIBUTE();
    BEGIN_TEST_METHOD_ATTRIBUTE(GrumpyBoldCat) TEST_IGNORE() END_TEST_METHOD_ATTRIBUTE();
#endif
};
