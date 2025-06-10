#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include "../src/Account.h"
#include "../src/Transaction.h"
#include "mock/MockAccount.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::Ref;

class MockTransaction : public Transaction {
public:
    MOCK_METHOD(void, SaveToDataBase, (Account& from, Account& to, int sum), (override));
};

TEST(TransactionTests, MakeInvalidAccounts) {
    MockAccount acc1(1, 1000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc1, 100), std::logic_error);
}

TEST(TransactionTests, MakeNegativeSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, -50), std::invalid_argument);
}

TEST(TransactionTests, MakeSmallSum) {
    MockAccount acc1(1, 1000);
    MockAccount acc2(2, 2000);
    Transaction tr;
    EXPECT_THROW(tr.Make(acc1, acc2, 99), std::logic_error);
}

TEST(TransactionTests, MakeSuccess) {
    MockAccount from(1, 1000);
    MockAccount to(2, 2000);
    MockTransaction tr;

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(from, ChangeBalance(-301)).Times(1);
    EXPECT_CALL(from, GetBalance()).Times(1).WillOnce(Return(1000));
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(tr, SaveToDataBase(Ref(from), Ref(to), 300)).Times(1);

    EXPECT_TRUE(tr.Make(from, to, 300));
}

TEST(TransactionTests, MakeInsufficientFunds) {
    MockAccount from(1, 100);
    MockAccount to(2, 200);
    MockTransaction tr;

    EXPECT_CALL(from, Lock()).Times(1);
    EXPECT_CALL(to, Lock()).Times(1);
    EXPECT_CALL(to, ChangeBalance(300)).Times(1);
    EXPECT_CALL(to, ChangeBalance(-300)).Times(1);
    EXPECT_CALL(from, ChangeBalance(_)).Times(0);
    EXPECT_CALL(from, GetBalance()).Times(1).WillOnce(Return(100));
    EXPECT_CALL(from, Unlock()).Times(1);
    EXPECT_CALL(to, Unlock()).Times(1);
    EXPECT_CALL(tr, SaveToDataBase(Ref(from), Ref(to), 300)).Times(1);

    EXPECT_FALSE(tr.Make(from, to, 300));
}

TEST(TransactionTests, DatabaseSaving) {
    const int initialFrom = 200;
    const int initialTo = 250;
    const int amount = 150;
    const int fee = 1;

    MockAccount from(1, initialFrom);
    MockAccount to(2, initialTo);
    Transaction tr;

    testing::Sequence seq;
    
    EXPECT_CALL(from, Lock()).InSequence(seq);
    EXPECT_CALL(to, Lock()).InSequence(seq);
    
    EXPECT_CALL(from, GetBalance())
        .InSequence(seq)
        .WillOnce(Return(initialFrom)); 
    
    EXPECT_CALL(from, ChangeBalance(-(amount + fee)))
        .InSequence(seq);
    
    EXPECT_CALL(to, ChangeBalance(amount))
        .InSequence(seq);
    
    EXPECT_CALL(from, GetBalance())
        .InSequence(seq)
        .WillOnce(Return(initialFrom - amount - fee));  // 49
    
    EXPECT_CALL(to, GetBalance())
        .InSequence(seq)
        .WillOnce(Return(initialTo + amount));  // 400
    
    EXPECT_CALL(to, Unlock()).InSequence(seq);
    EXPECT_CALL(from, Unlock()).InSequence(seq);

    testing::internal::CaptureStdout();
    bool result = tr.Make(from, to, amount);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(result);
    EXPECT_EQ(output, "1 send to 2 $150\nBalance 1 is 49\nBalance 2 is 400\n");
}
