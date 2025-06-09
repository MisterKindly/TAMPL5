#pragma once
#include <gmock/gmock.h>
#include "../src/Account.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance) : Account(id, balance) {}
    
    virtual ~MockAccount() {}
    
    MOCK_METHOD(int, GetBalance, (), (const, override));
    MOCK_METHOD(void, ChangeBalance, (int diff), (override));
    MOCK_METHOD(void, Lock, (), (override));
    MOCK_METHOD(void, Unlock, (), (override));
    
    void DefaultChangeBalance(int diff) {
        Account::ChangeBalance(diff);
    }
};
