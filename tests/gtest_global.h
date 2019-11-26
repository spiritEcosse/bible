#ifndef GTEST_GLOBAL_H
#define GTEST_GLOBAL_H

#include <gtest/gtest.h>
#include <gmock/gmock.h>

using ::testing::_;
using ::testing::TestWithParam;
using ::testing::NotNull;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;
using ::testing::Test;
using ::testing::ValuesIn;
using ::testing::InSequence;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Mock;
using ::testing::Invoke;
using ::testing::DefaultValue;
using ::testing::Eq;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::ReturnPointee;
using ::testing::Property;
using ::testing::internal::BuiltInDefaultValue;
using ::testing::StrictMock;

#endif // GTEST_GLOBAL_H
