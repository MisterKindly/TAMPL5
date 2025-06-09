#include "MockAccount.h"

class MockAccount : public Account {
public:
    MockAccount(int id, int balance);
    virtual ~MockAccount();

    int GetBalance() const override { 
        return GetBalanceMock(); 
    }
    
    void ChangeBalance(int diff) override { 
        ChangeBalanceMock(diff); 
    }
    
    void Lock() override { 
        LockMock(); 
    }
    
    void Unlock() override { 
        UnlockMock(); 
    }

    MOCK_METHOD(int, GetBalanceMock, (), (const));
    MOCK_METHOD(void, ChangeBalanceMock, (int diff));
    MOCK_METHOD(void, LockMock, ());
    MOCK_METHOD(void, UnlockMock, ());
};
