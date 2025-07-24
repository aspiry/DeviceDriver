#include "gmock/gmock.h"
#include "device_driver.h"
#include <stdexcept>
#include <string>
using namespace testing;
class FlashMock :public FlashMemoryDevice {
public:
	
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};
TEST(DeviceDriver, FiveRead) {
	FlashMock mock;
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return(1));

		DeviceDriver driver{ &mock };
	
		int data = driver.read(0xB);
		EXPECT_EQ(1, data);
}
TEST(DeviceDriver, FiveReadException) {
	FlashMock mock;
	EXPECT_CALL(mock, read((long)0xB))
		.WillOnce(Return(0xA))
		.WillOnce(Return(0xA))
		.WillOnce(Return(0xA))
		.WillOnce(Return(0xA))
		.WillRepeatedly(Return(0xB));
		
	
	DeviceDriver driver{ &mock };
	EXPECT_THROW(driver.read(0xB), std::exception);
	
}
TEST(DeviceDriver, CannotWrite) {
	NiceMock<FlashMock> mock;
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return(0xB));

	DeviceDriver driver{ &mock };
	try
	{
		driver.write(0xB, 0xB);
	}
	catch (std::runtime_error& e)
	{
		EXPECT_EQ(std::string{ e.what() }, std::string{ "already writed" });
	}
}
TEST(DeviceDriver, WriteAvailable) {
	NiceMock<FlashMock> mock;
	EXPECT_CALL(mock, read)
		.Times(5)
		.WillRepeatedly(Return(0xFF));

	DeviceDriver driver{ &mock };
	try
	{
		long addr = 0xB;
		int data = 0xA;
		driver.write(addr, data);
		EXPECT_CALL(mock, read(addr))
			.WillRepeatedly(Return(data));

		EXPECT_EQ(data, driver.read(addr));
	}
	catch (std::runtime_error& e)
	{
		FAIL();
	}
}
int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}