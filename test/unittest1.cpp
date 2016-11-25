#include "CppUnitTest.h"
#include "../main/AnagramFinder.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTest {
    TEST_CLASS(UnitTest1) {
private:
    AnagramFinder f;
public:
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
    };
}
