
#include <gtest/gtest.h>

#include <tuple>
#include <EC/Meta/Meta.hpp>
#include <EC/EC.hpp>

struct C0 {};
struct C1 {};
struct C2 {};
struct C3 {};

struct T0 {};
struct T1 {};

using ListComponentsAll = EC::Meta::TypeList<C0, C1, C2, C3>;
using ListComponentsSome = EC::Meta::TypeList<C1, C3>;

using ListTagsAll = EC::Meta::TypeList<T0, T1>;

using ListAll = EC::Meta::TypeList<C0, C1, C2, C3, T0, T1>;

TEST(Meta, Contains)
{
    int size = ListComponentsAll::size;
    EXPECT_EQ(size, 4);

    bool result = EC::Meta::Contains<C0, ListComponentsAll>::value;
    EXPECT_TRUE(result);
    result = EC::Meta::Contains<C1, ListComponentsAll>::value;
    EXPECT_TRUE(result);
    result = EC::Meta::Contains<C2, ListComponentsAll>::value;
    EXPECT_TRUE(result);
    result = EC::Meta::Contains<C3, ListComponentsAll>::value;
    EXPECT_TRUE(result);

    size = ListComponentsSome::size;
    EXPECT_EQ(size, 2);

    result = EC::Meta::Contains<C0, ListComponentsSome>::value;
    EXPECT_FALSE(result);
    result = EC::Meta::Contains<C1, ListComponentsSome>::value;
    EXPECT_TRUE(result);
    result = EC::Meta::Contains<C2, ListComponentsSome>::value;
    EXPECT_FALSE(result);
    result = EC::Meta::Contains<C3, ListComponentsSome>::value;
    EXPECT_TRUE(result);
}

TEST(Meta, ContainsAll)
{
    bool contains = EC::Meta::ContainsAll<ListComponentsSome, ListComponentsAll>::value;
    EXPECT_TRUE(contains);

    contains = EC::Meta::ContainsAll<ListComponentsAll, ListComponentsSome>::value;
    EXPECT_FALSE(contains);

    contains = EC::Meta::ContainsAll<ListComponentsAll, ListComponentsAll>::value;
    EXPECT_TRUE(contains);
}

TEST(Meta, IndexOf)
{
    int index = EC::Meta::IndexOf<C0, ListComponentsAll>::value;
    EXPECT_EQ(index, 0);
    index = EC::Meta::IndexOf<C1, ListComponentsAll>::value;
    EXPECT_EQ(index, 1);
    index = EC::Meta::IndexOf<C2, ListComponentsAll>::value;
    EXPECT_EQ(index, 2);
    index = EC::Meta::IndexOf<C3, ListComponentsAll>::value;
    EXPECT_EQ(index, 3);

    index = EC::Meta::IndexOf<C1, ListComponentsSome>::value;
    EXPECT_EQ(index, 0);
    index = EC::Meta::IndexOf<C3, ListComponentsSome>::value;
    EXPECT_EQ(index, 1);
}

TEST(Meta, Bitset)
{
    EC::Bitset<ListComponentsAll, ListTagsAll> bitset;
    EXPECT_EQ(bitset.size(), ListComponentsAll::size + ListTagsAll::size);

    bitset[EC::Meta::IndexOf<C1, ListComponentsAll>::value] = true;
    EXPECT_TRUE(bitset.getComponentBit<C1>());
    bitset.flip();
    EXPECT_FALSE(bitset.getComponentBit<C1>());

    bitset.reset();
    bitset[ListComponentsAll::size + EC::Meta::IndexOf<T0, ListTagsAll>::value] = true;
    EXPECT_TRUE(bitset.getTagBit<T0>());
    bitset.flip();
    EXPECT_FALSE(bitset.getTagBit<T0>());
}

TEST(Meta, Combine)
{
    using CombinedAll = EC::Meta::Combine<ListComponentsAll, ListTagsAll>;

    int listAllTemp = ListAll::size;
    int combinedAllTemp = CombinedAll::size;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<C0, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<C0, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<C1, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<C1, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<C2, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<C2, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<C3, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<C3, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<T0, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<T0, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);

    listAllTemp = EC::Meta::IndexOf<T1, ListAll>::value;
    combinedAllTemp = EC::Meta::IndexOf<T1, CombinedAll>::value;
    EXPECT_EQ(combinedAllTemp, listAllTemp);
}

TEST(Meta, Morph)
{
    using TupleAll = std::tuple<C0, C1, C2, C3>;
    using MorphedTuple = EC::Meta::Morph<TupleAll, EC::Meta::TypeList<> >;

    int morphedTupleTemp = MorphedTuple::size;
    int componentsTemp = ListComponentsAll::size;
    EXPECT_EQ(morphedTupleTemp, componentsTemp);

    morphedTupleTemp = EC::Meta::IndexOf<C0, MorphedTuple>::value;
    componentsTemp = EC::Meta::IndexOf<C0, ListComponentsAll>::value;
    EXPECT_EQ(morphedTupleTemp, componentsTemp);

    morphedTupleTemp = EC::Meta::IndexOf<C1, MorphedTuple>::value;
    componentsTemp = EC::Meta::IndexOf<C1, ListComponentsAll>::value;
    EXPECT_EQ(morphedTupleTemp, componentsTemp);

    morphedTupleTemp = EC::Meta::IndexOf<C2, MorphedTuple>::value;
    componentsTemp = EC::Meta::IndexOf<C2, ListComponentsAll>::value;
    EXPECT_EQ(morphedTupleTemp, componentsTemp);

    morphedTupleTemp = EC::Meta::IndexOf<C3, MorphedTuple>::value;
    componentsTemp = EC::Meta::IndexOf<C3, ListComponentsAll>::value;
    EXPECT_EQ(morphedTupleTemp, componentsTemp);

    using MorphedComponents = EC::Meta::Morph<ListComponentsAll, std::tuple<> >;
    bool isSame = std::is_same<MorphedComponents, TupleAll>::value;
    EXPECT_TRUE(isSame);
}

