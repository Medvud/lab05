#include "Transaction.h"
#include "Account.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

class MockAcc : public Account {
public:
	MockAcc(int id, int balance) : Account(id, balance) {};
	~MockAcc() override = default;
	MOCK_METHOD(int, GetBalance, (), (const, override));
	MOCK_METHOD(void, ChangeBalance, (int), (override));
	MOCK_METHOD(void, Lock, (), (override));
	MOCK_METHOD(void, Unlock, (), (override));
};

//Тесты для класса аккаунт
TEST(Account, GetBalance) {
	Account a(1, 100);
	int bal = a.GetBalance();
	EXPECT_EQ(bal, 100);
}

TEST(Account, ChangeBalance) {
	Account a(1, 100);
	ASSERT_THROW(a.ChangeBalance(21), std::runtime_error);
	int bal1 = a.GetBalance();
	EXPECT_EQ(bal1, 100);
	a.Lock();
	a.ChangeBalance(21);
	int bal2 = a.GetBalance();
	EXPECT_EQ(bal2, 100 + 21);
}

TEST(Account, Lock) {
	Account a(1, 100);
	ASSERT_NO_THROW(a.Lock());
}

TEST(Account, Unlock) {
	Account a(1, 100);
	a.Lock();
	ASSERT_NO_THROW(a.ChangeBalance(20));
	int bal = a.GetBalance();
	ASSERT_EQ(bal, 120);
	a.Unlock();
	ASSERT_THROW(a.ChangeBalance(15), std::runtime_error);
}

//Тесты для Transaction

TEST(Transaction, Make) {
	Account a1(1, 400);
	Account a2(2, 200);
	Transaction t;
	int s1 = -50, s2 = 100;
	ASSERT_THROW(t.Make(a1, a1, s2), std::logic_error);
	ASSERT_THROW(t.Make(a1, a2, s1), std::invalid_argument);
	bool wow = t.Make(a1, a2, s2);
	ASSERT_TRUE(wow);
	int b1 = a1.GetBalance();
	int b2 = a2.GetBalance();
	ASSERT_EQ(b1, 300);
	ASSERT_EQ(b2, 300);
}

TEST(Transaction, SaveToDataBase) {
	Transaction t;
	Account a1(1, 300);
	Account a2(2, 300);
	bool wow = t.Make(a1, a2, 150);
	EXPECT_TRUE(wow);
}

TEST(Mock, Lock) {
	MockAcc a1(1, 200);
	MockAcc a2(2, 300);
	Transaction t;
	EXPECT_CALL(a1, Lock()).Times(::testing::AtLeast(1));
	ON_CALL(a1, Lock()).WillByDefault(::testing::Return());
	bool wow = t.Make(a1, a2, 150);
}

TEST(Mock, Unlock) {
	MockAcc a1(1, 200);
	MockAcc a2(2, 300);
	Transaction t;
	EXPECT_CALL(a1, Unlock()).Times(::testing::AtLeast(1));
	ON_CALL(a1, Unlock()).WillByDefault(::testing::Return());
	bool wow = t.Make(a1, a2, 150);
}

TEST(Mock, GetBalance) {
	MockAcc a1(1, 200);
	MockAcc a2(2, 300);
	Transaction t;
	EXPECT_CALL(a1, GetBalance()).Times(::testing::AtLeast(1));
	ON_CALL(a1, GetBalance()).WillByDefault(::testing::Return(200));
	bool wow = t.Make(a1, a2, 150);
}

TEST(Mock, ChangeBalance) {
	MockAcc a1(1, 200);
	MockAcc a2(2, 300);
	Transaction t;
	ON_CALL(a1, GetBalance()).WillByDefault(::testing::Return(200));
	ON_CALL(a2, GetBalance()).WillByDefault(::testing::Return(300));
	EXPECT_CALL(a1, ChangeBalance(testing::_)).Times(::testing::AtLeast(1));
	bool wow = t.Make(a1, a2, 150);
}

int main(int argc, char** argv) {
	::testing::InitGoogleMock(&argc, argv);
	return RUN_ALL_TESTS();
}

