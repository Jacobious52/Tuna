
#include "api/cpp/tuna.hpp"

int main()
{
    TEST("DNA TEST 1", EQUAL("marry", "john"));
    TEST("DNA TEST 2", EQUAL("marry", "marry"));
    TEST("vector test", EQUAL(std::vector<int>{1,2,3,4, 8}, std::vector<int>{1,2,3,4}));

    TEST("4 < 3?", SMALLER(4, 3));
    TEST("4 > 3?", GREATER(4, 3));

    INFO("Hello world, this is me!");
    WARN("Im in a bad mood");

    return 0;
}
