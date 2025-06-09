#pragma once
#include "../src/Account.h"
#include <gmock/gmock.h>

class MockAccount : public Account {
 public:
  
  MockAccount(int id, int balance);
  
  
  virtual ~MockAccount();

  
  MOCK_METHOD(int, GetBalance, (), (const, override));
  MOCK_METHOD(void, ChangeBalance, (int diff), (override));
  MOCK_METHOD(void, Lock, (), (override));
  MOCK_METHOD(void, Unlock, (), (override));
};
