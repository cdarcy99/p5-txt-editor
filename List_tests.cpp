#include "List.hpp"
#include "unit_test_framework.hpp"

using namespace std;

// Add your test cases here

TEST(emptyList){
    // not empty after something is added
    List<int> l;
    l.push_back(1);
    ASSERT_FALSE(l.empty());
    ASSERT_EQUAL(l.size(),1);

    // clear empties again
    l.push_back(2);
    l.clear();
    ASSERT_TRUE(l.empty());
}


TEST(pushing){
    // front and back are same with one element
    List<int> l;
    l.push_back(1);
    ASSERT_EQUAL(l.front(),1);
    ASSERT_EQUAL(l.back(),1);

    // keeps pushing back when multiple things are pushed
    l.push_back(2);
    l.push_back(3);
    ASSERT_EQUAL(l.back(),3);
    ASSERT_EQUAL(l.size(), 3);

    // push front goes to front
    l.push_front(4);
    l.push_front(5);
    ASSERT_EQUAL(l.front(),5);
    ASSERT_EQUAL(l.size(),5);

    // also tests mixing front and back pushes

}

TEST(removing){
    // removes 1 front element (alone)
    List<int> l;
    l.push_back(1);
    l.pop_front();
    ASSERT_TRUE(l.empty());

    // removes 1 back element (alone)
    l.push_back(1);
    l.pop_back();
    ASSERT_TRUE(l.empty());

    // can remove mutliple until empty
    l.push_back(1);
    l.push_back(2);
    l.pop_front();
    l.pop_front();
    ASSERT_TRUE(l.empty());
}


TEST(copy_ctor){
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    List<int> copy(l);
    ASSERT_EQUAL(copy.front(), 1);
    ASSERT_EQUAL(copy.back(), 3);
    ASSERT_EQUAL(copy.size(), 3);

    // ensure a deep copy
    copy.pop_front();
    ASSERT_EQUAL(l.size(), 3);
    ASSERT_EQUAL(copy.size(), 2);
}

TEST(deep_copy){
    // deep copy for assignment operator
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    List<int> l2;
    l2 = l;
    l2.pop_front();
    ASSERT_EQUAL(l.size(), 2);   // original unchanged
    ASSERT_EQUAL(l2.size(), 1);
}

TEST(operators){
    // assignment
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    List<int> list2;
    list2 = l;
    ASSERT_EQUAL(list2.front(), 1);
    ASSERT_EQUAL(list2.back(), 2);
    ASSERT_EQUAL(list2.size(), 2);

    // copy empty no errors
    l.clear();
    List<int> copy(l);
    ASSERT_TRUE(copy.empty());
}

TEST(iterators){
    // empty list
    List<int> l;
    ASSERT_TRUE(l.begin() == l.end());

    // can traverse to the next element if only 1
    l.push_back(5);
    auto it = l.begin();
    ASSERT_EQUAL(*it, 5);
    ++it;
    ASSERT_TRUE(it == l.end());

    // can traverse to next element
    l.push_back(2);
    it = l.begin(); 
    auto old = it++;
    ASSERT_EQUAL(*old, 5);
    ASSERT_EQUAL(*it, 2);

    // can move to the end
    l.push_back(3);
    it = l.end();
    --it;
    ASSERT_EQUAL(*it, 3);
    --it;
    ASSERT_EQUAL(*it, 2);
    --it;
    ASSERT_EQUAL(*it, 5);

    // default iterator is the first element
     List<int>::Iterator a;
    List<int>::Iterator b;
    ASSERT_TRUE(a == b);
}

TEST(erase){
    // iterator keeps up when erasing
    List<int> l;
    l.push_back(1);
    auto it = l.erase(l.begin());
    ASSERT_TRUE(l.empty());
    ASSERT_TRUE(it == l.end());

    // erase middle
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    it = l.begin();
    ++it;
    it = l.erase(it);
    ASSERT_EQUAL(*it, 3);
    ASSERT_EQUAL(l.front(), 1);
    ASSERT_EQUAL(l.back(), 3);
    ASSERT_EQUAL(l.size(), 2);
}

TEST(insert){
    //in the middle
    List<int> l;
    l.push_back(1);
    l.push_back(3);
    auto it = l.begin();
    ++it;
    l.insert(it, 2);
    auto check = l.begin();
    ASSERT_EQUAL(*check, 1); 
    ++check;
    ASSERT_EQUAL(*check, 2); 
    ++check;
    ASSERT_EQUAL(*check, 3);

    // if empty
    l.clear();
    l.insert(l.end(), 42);
    ASSERT_EQUAL(l.front(), 42);
    ASSERT_EQUAL(l.back(), 42);
    ASSERT_EQUAL(l.size(), 1);
}

TEST(insert_end){
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    
    l.insert(l.end(), 3);
    ASSERT_EQUAL(l.back(), 3);
    ASSERT_EQUAL(l.size(), 3);
    
    // no dangling last pointer
    l.push_back(4);
    ASSERT_EQUAL(l.back(), 4);
    ASSERT_EQUAL(l.size(), 4);
    
    // traverse 
    auto it = l.begin();
    ASSERT_EQUAL(*it, 1); ++it;
    ASSERT_EQUAL(*it, 2); ++it;
    ASSERT_EQUAL(*it, 3); ++it;
    ASSERT_EQUAL(*it, 4); ++it;
    ASSERT_TRUE(it == l.end());
}
TEST(erase_back){
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l.push_back(3);
    
    // erase the last element
    auto it = l.begin();
    ++it; 
    ++it;        //3
    it = l.erase(it);
    ASSERT_TRUE(it == l.end());
    ASSERT_EQUAL(l.back(), 2);   // last must now be 2
    ASSERT_EQUAL(l.size(), 2);
    
    // make no dangling next pointer
    l.push_back(99);
    ASSERT_EQUAL(l.back(), 99);
    ASSERT_EQUAL(l.size(), 3);
}

TEST(extras){
    // self assign
    List<int> l;
    l.push_back(1);
    l.push_back(2);
    l = l;  // should not crash or lose data
    ASSERT_EQUAL(l.size(), 2);
    ASSERT_EQUAL(l.front(), 1);
    ASSERT_EQUAL(l.back(), 2);

    //non empty assignment
    List<int> l2;
    l2.push_back(99);  // l2 already has data
    l2 = l;            // old 99 must be gone
    ASSERT_EQUAL(l2.front(), 1);
    ASSERT_EQUAL(l2.back(), 2);
    ASSERT_EQUAL(l2.size(), 2);

    // empty to non empty
    l.clear();
    l2.clear();
    l2.push_back(1);
    l2.push_back(2);
    l2 = l;  // should wipe l2
    ASSERT_TRUE(l2.empty());
    ASSERT_EQUAL(l2.size(), 0);
}

TEST(iterator_equality){
    List<int> l;
    l.push_back(1);
    
    List<int>::Iterator def;  // default construct
    ASSERT_TRUE(def != l.end());   // default != list's though both nullptr
    ASSERT_TRUE(def != l.begin()); 

    ASSERT_TRUE(l.begin() == l.begin());

    // iterators to different locations are unequal
    auto it = l.begin();
    ++it;
    ASSERT_TRUE(it != l.begin());
    ASSERT_TRUE(it == l.end());
}


TEST_MAIN()
