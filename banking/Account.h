#pragma once

class Account {
 public:
  Account(int id, int balance);
  virtual ~Account() = default;  

  int GetBalance() const = 0;
  virtual void ChangeBalance(int diff) = 0;
  virtual void Lock() = 0;
  virtual void Unlock() = 0;
  
  int id() const { return id_; }

 private:
  int id_;
  int balance_;
  bool is_locked_;
};
