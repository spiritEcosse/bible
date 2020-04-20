#include "gtest_global.h"

#include "mock_textprogressbar.h"


class TextProgressBarTest : public ::testing::Test
{
protected:
    // You can remove any or all of the following functions if its body
    // is empty.

    TextProgressBarTest()
        : textProgressBar(&mockTextProgressBar)
    {}

    ~TextProgressBarTest() override {
     // You can do clean-up work that doesn't throw exceptions here.
    //      delete mockModulesModel;
    }

    // If the constructor and destructor are not enough for setting up
    // and cleaning up each test, you can define the following methods:

    void SetUp() override {
     // Code here will be called immediately after the constructor (right
     // before each test).
    }

    void TearDown() override {
     // Code here will be called immediately after each test (right
     // before the destructor).
    }

    // Objects declared here can be used by all tests in the test case for ModulesModel.

    MockTextProgressBar mockTextProgressBar;
    TextProgressBar* textProgressBar;
    const QString m = BuiltInDefaultValue<QString>::Get();
    qint64 value = BuiltInDefaultValue<qint64>::Get();
    qint64 maximum = BuiltInDefaultValue<qint64>::Get();
};

TEST_F(TextProgressBarTest, clear)
{
    EXPECT_CALL(mockTextProgressBar, clear())
            .WillOnce(
                    Invoke(&mockTextProgressBar, &MockTextProgressBar::parentClear)
                );

    mockTextProgressBar.clear();
    EXPECT_EQ(mockTextProgressBar.value, 0);
    EXPECT_EQ(mockTextProgressBar.maximum, -1);
    EXPECT_EQ(mockTextProgressBar.iteration, 0);
}

TEST_F(TextProgressBarTest, update)
{
    EXPECT_CALL(mockTextProgressBar, update())
            .WillOnce(
                    Invoke(&mockTextProgressBar, &MockTextProgressBar::parentUpdate)
                );

    mockTextProgressBar.update();
    EXPECT_EQ(mockTextProgressBar.iteration, 1);
}

TEST_F(TextProgressBarTest, setMessage)
{
    EXPECT_CALL(mockTextProgressBar, setMessage(m))
            .WillOnce(
                    Invoke(&mockTextProgressBar, &MockTextProgressBar::parentSetMessage)
                );

    mockTextProgressBar.setMessage(m);
    EXPECT_EQ(mockTextProgressBar.message, m);
}

TEST_F(TextProgressBarTest, setStatus)
{
    EXPECT_CALL(mockTextProgressBar, setStatus(value, maximum))
            .WillOnce(
                Invoke(&mockTextProgressBar, &MockTextProgressBar::parentSetStatus)
                );

    mockTextProgressBar.setStatus(value, maximum);
    EXPECT_EQ(mockTextProgressBar.value, value);
    EXPECT_EQ(mockTextProgressBar.maximum, maximum);
}
