#include <atomic>

#include <stdio.h>

#include <gtest/gtest.h>

#include "shared_ptr/shared_ptr.h"

namespace {

struct Data {
    Data() {
        static std::atomic_int id_alloc(0);
        id = id_alloc++;

        printf("Data %d created\n", id);
    }

    ~Data() {
        printf("Data %d distroyed\n", id);
    }

    void Foo() {
        printf("Data %d do sth.\n", id);
    }

    int id;
};


TEST(SharedPtr, null) {
    lfs::SharedPtr<int> sptr0;
    ASSERT_EQ(1, sptr0.Count());
    lfs::SharedPtr<int> sptr1(nullptr);
    ASSERT_EQ(1, sptr1.Count());
}

TEST(SharedPtr, copy_null) {
    lfs::SharedPtr<int> sptr(nullptr);
    auto new_sptr(sptr);
    ASSERT_EQ(2, new_sptr.Count());
}

TEST(SharedPtr, create) {
    lfs::SharedPtr<int> sptr(new int(3));
    ASSERT_EQ(1, sptr.Count());
    ASSERT_NE(sptr.operator->(), nullptr);
    ASSERT_EQ(*sptr, 3);
}

TEST(SharedPtr, copy_ctor) {
    lfs::SharedPtr<int> sptr0(new int(3));
    ASSERT_NE(sptr0.operator->(), nullptr);
    ASSERT_EQ(*sptr0, 3);

    ASSERT_EQ(1, sptr0.Count());
    {
        auto new_sptr(sptr0);
        ASSERT_EQ(2, new_sptr.Count());
    }
    ASSERT_EQ(1, sptr0.Count());
}

TEST(SharedPtr, copy_assign) {
    lfs::SharedPtr<Data> sptr0(new Data());
    ASSERT_EQ(sptr0.Count(), 1);

    {
        lfs::SharedPtr<Data> sptr1(new Data());
        ASSERT_EQ(sptr1.Count(), 1);
        sptr1 = sptr0;
        ASSERT_EQ(sptr1.Count(), 2);
        ASSERT_EQ(sptr0.Count(), 2);
    }

    ASSERT_EQ(sptr0.Count(), 1);

    auto f = [](lfs::SharedPtr<Data> sptr) {
        ASSERT_EQ(sptr.Count(), 2);
        sptr->Foo();
    };

    f(sptr0);
    ASSERT_EQ(sptr0.Count(), 1);
}

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
