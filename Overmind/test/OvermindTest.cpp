#include "OvermindImpl.h"
#include "Constructor.h"
#include <gtest/gtest.h>
#include <memory>

using namespace OvermindImpl;

class OvermindTest : public ::testing::Test
{
public:

    void SetUp()
    {
        m_overmind.reset(new Overmind);
    }
    void TearDown()
    {
        m_overmind.reset();
    }
protected:
    std::unique_ptr<Overmind> m_overmind;
};

TEST_F(OvermindTest, ErrorCheck)
{
    ASSERT_NE(Status::OK, m_overmind->ExecuteScript("blah.blah"));
    std::string message = m_overmind->GetLastError().c_str();
    ASSERT_STRNE("", message.c_str());
}

TEST_F(OvermindTest, ExecuteTestScript)
{
    ASSERT_EQ(Status::OK, m_overmind->ExecuteScript("test_scripts/empty.lua"));
    std::string message = m_overmind->GetLastError().c_str();
    ASSERT_STREQ("", message.c_str()) << "Error received: " << message.c_str();
}

TEST_F(OvermindTest, RegisterObject)
{
    ILibrary& lib = Constructor::GetConstructor().GetLibrary();

    ASSERT_EQ(Status::ResourceNotFound, lib.CheckObjectStatus("NewObject"));

    ASSERT_EQ(Status::OK, m_overmind->ExecuteScript("test_scripts/registerObject.lua"));
    std::string message = m_overmind->GetLastError().c_str();
    ASSERT_STREQ("", message.c_str()) << "Error received: " << message.c_str();

    ASSERT_EQ(Status::OK, lib.CheckObjectStatus("NewObject"));
}

// eof