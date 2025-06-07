#include "Transaction.h"
#include <stdexcept>
#include <iostream>
#include "Account.h"

Transaction::Transaction() : fee_(1) {}

Transaction::~Transaction() {}

bool Transaction::Make(Account& from, Account& to, int sum) {
  if (from.id() == to.id()) throw std::logic_error("invalid action");

  if (sum < 0) throw std::invalid_argument("sum can't be negative");

  if (sum < 100) throw std::logic_error("too small");

  if (fee_ * 2 > sum) return false;

  bool success = Debit(to, sum + fee_);
  if (!success) to.ChangeBalance(-sum);

  SaveToDataBase(from, to, sum);
  return success;
}

void Transaction::Credit(Account& accout, int sum) {
 accout.ChangeBalance(sum);
}

bool Transaction::Debit(Account& accout, int sum) {
 if (accout.GetBalance() > sum) {
    accout.ChangeBalance(-sum);
    return true;
  }
  return false;
}

void Transaction::SaveToDataBase(Account& from, Account& to, int sum) {
  std::cout << from.id() << " send to " << to.id() << " $" << sum << std::endl;
  std::cout << "Balance " << from.id() << " is " << from.GetBalance()
            << std::endl;
  std::cout << "Balance " << to.id() << " is " << to.GetBalance() << std::endl;
}
