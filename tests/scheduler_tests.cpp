#include <cmath>
#include "lib/TaskScheduler.h"
#include "gtest/gtest.h"

TEST(SchedulerTests, Test1) {
    std::vector<std::string> order;
    TTaskScheduler scheduler;
    auto First = scheduler.add([&]() {order.push_back("First"); return 0;});
    auto Second = scheduler.add([&](int a) {order.push_back("Second"); return 0;}, scheduler.getFutureResult<int>(First));
    auto Third = scheduler.add([&](int a) {order.push_back("Third"); return 0;}, scheduler.getFutureResult<int>(Second));
    auto Fourth = scheduler.add([&](int a) {order.push_back("Fourth"); return 0;}, scheduler.getFutureResult<int>(Third));
    auto Fifth = scheduler.add([&](int a) {order.push_back("Fifth"); return 0;}, scheduler.getFutureResult<int>(Third));
    auto Sixth = scheduler.add([&](int a) {order.push_back("Sixth"); return 0;}, scheduler.getFutureResult<int>(Third));
    auto Seventh = scheduler.add([&](const std::vector<FutureResult<int>>& a) {order.push_back("Seventh"); return 0;},
                                 std::vector<FutureResult<int>>({scheduler.getFutureResult<int>(Fourth),
                                             scheduler.getFutureResult<int>(Fifth),
                                             scheduler.getFutureResult<int>(Sixth)}));
    auto Eighth = scheduler.add([&](int a) {order.push_back("Eighth"); return 0;}, scheduler.getFutureResult<int>(Seventh));
    auto Ninth = scheduler.add([&](int a) {order.push_back("Ninth"); return 0;}, 0);
    auto Tenth = scheduler.add([&](int a, int b) {order.push_back("Tenth"); return 0;}, scheduler.getFutureResult<int>(Eighth), scheduler.getFutureResult<int>(Ninth));

    scheduler.executeAll();

    ASSERT_EQ(order.size(), 10);
    ASSERT_EQ(order[0], "First");
    ASSERT_EQ(order[1], "Second");
    ASSERT_EQ(order[2], "Third");
    ASSERT_EQ(order[3], "Fourth");
    ASSERT_EQ(order[4], "Fifth");
    ASSERT_EQ(order[5], "Sixth");
    ASSERT_EQ(order[6], "Seventh");
    ASSERT_EQ(order[7], "Eighth");
    ASSERT_EQ(order[8], "Ninth");
    ASSERT_EQ(order[9], "Tenth");
}

TEST(SchedulerTests, Test2) {
    float a = 1;
    float b = -4;
    float c = 4;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([](float a, float c) { return -4 * a * c; }, a, c);
    auto id2 = scheduler.add([](float b, float v) { return b * b + v; }, b, scheduler.getFutureResult<float>(id1));
    auto id3 = scheduler.add([](float b, float d) { return -b + std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
    auto id4 = scheduler.add([](float b, float d) { return -b - std::sqrt(d); }, b, scheduler.getFutureResult<float>(id2));
    auto id5 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id3));
    auto id6 = scheduler.add([](float a, float v) { return v / (2 * a); }, a, scheduler.getFutureResult<float>(id4));

    scheduler.executeAll();

    ASSERT_EQ(scheduler.getResult<float>(id5), 2);
    ASSERT_EQ(scheduler.getResult<float>(id6), 2);
}

TEST(SchedulerTests, Test3) {
    size_t a = 10;

    TTaskScheduler scheduler;

    auto id1 = scheduler.add([] () {return 5;});

    auto future_result = scheduler.getFutureResult<int>(id1);
    auto id2 = scheduler.add([] (int previous) {return 2 * previous;}, future_result);

    scheduler.executeAll();

    ASSERT_EQ(10, scheduler.getResult<int>(id2));
}

TEST(SchedulerTests, Test4) {
    std::vector<char> a = {'2', '3', '9'};
    std::string connect_nums;

    TTaskScheduler scheduler;


    auto id1 = scheduler.add([&]() {connect_nums.push_back(a[0]); return 0;});
    auto id2 = scheduler.add([&]() {connect_nums.push_back(a[1]); return 0;});
    auto id3 = scheduler.add([&]() {connect_nums.push_back(a[2]); return 0;});

    scheduler.executeAll();

    ASSERT_EQ(connect_nums, "239");
}
