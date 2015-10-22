#ifndef UNITTEST_TESTRUNNER_H
#define UNITTEST_TESTRUNNER_H

#include "Test.h"
#include "TestList.h"
#include "CurrentTest.h"
#include <string.h>

namespace UnitTest {

class TestReporter;
class TestResults;
class Timer;

int RunAllTests(const char* suitName = 0);

struct AlwaysTrue
{
	bool operator()(const Test* const) const
	{
		return true;	
	}
};

struct RunSpecificTest
{
	const char* testName;

	RunSpecificTest(const char* _testName) : testName(_testName) {}
	bool operator()(const Test* const t) const
	{
		return !strcmp(t->m_details.testName, testName);
	}
};

class TestRunner
{
public:
	explicit TestRunner(TestReporter& reporter);
	~TestRunner();

	template <class Predicate>
	int RunTestsIf(TestList const& list, char const* suiteName, 
				   Predicate& predicate, int maxTestTimeInMs) const
	{
	    Test* curTest = list.GetHead();

	    while (curTest != 0)
	    {
		    if (IsTestInSuite(curTest,suiteName) && predicate(curTest))
			{
				RunTest(m_result, curTest, maxTestTimeInMs);
			}

			curTest = curTest->next;
	    }

	    return Finish();
	}	
	template <class Predicate>
	int RunTestsIf(TestList const& list, char const* suiteName,
				   const Predicate& predicate, int maxTestTimeInMs) const
    {
        return RunTestsIf(list, suiteName, const_cast<Predicate&>(predicate), maxTestTimeInMs);
    }

private:
	TestReporter* m_reporter;
	TestResults* m_result;
	Timer* m_timer;

	int Finish() const;
	bool IsTestInSuite(const Test* const curTest, char const* suiteName) const;
	void RunTest(TestResults* const result, Test* const curTest, int const maxTestTimeInMs) const;
};

}

#endif
