#include "tests.hpp"


void	print3x3TestResult(bool passed, const glm::mat3& glmMat, const mat3& myMat)
{
	if (passed == true)
	{
		std::cout << GREEN << "[OK] " << RESET << "\n";
	}
	else
	{
		std::cout << RED << "[FAIL] " << RESET << "\n";
		std::cout << "Expected:\n";
		print3x3(glmMat);
		std::cout << "Got:\n";
		print3x3(myMat);
	}
}

void	print4x4TestResult(bool passed, const glm::mat4& glmMat, const mat4& myMat)
{
	if (passed == true)
	{
		std::cout << GREEN << "[OK] " << RESET << "\n";
	}
	else
	{
		std::cout << RED << "[FAIL] " << RESET << "\n";
		std::cout << "Expected:\n";
		print4x4(glmMat);
		std::cout << "Got:\n";
		print4x4(myMat);
	}
}

bool	compareVec3(const vec3& myVec, const glm::vec3& glmVec)
{
	bool	result = true;

	if (std::abs(myVec.x - glmVec.x) > IMPRECISION_TOLERANCE ||
		std::abs(myVec.y - glmVec.y) > IMPRECISION_TOLERANCE ||
		std::abs(myVec.z - glmVec.z) > IMPRECISION_TOLERANCE)
	{
		result = false;
	}
	return result;
}

bool	compareMat4(const mat4& myMat, const glm::mat4& glmMat)
{
	for (int row = 0; row < 4; row++)
	{
		for (int col = 0; col < 4; col++)
		{
			if (std::abs(myMat.data[row][col] - glmMat[col][row]) > IMPRECISION_TOLERANCE)
			{
				return false;
			}
		}
	}
	return true;
}

int	mat4InitializerTests()
{
	std::cout << "Matrix initializers:\n";

	int		failures = 0;
	bool	result;
	void	(*printFunction)(bool, const glm::mat4&, const mat4&) = print4x4TestResult;

	{
		mat4		myMat{};
		glm::mat4	glmMat{};

		result = compareMat4(myMat, glmMat);
		failures += result;
		printFunction(result, glmMat, myMat);
	}
	{
		mat4		myMat(2.0f);
		glm::mat4	glmMat(2.0f);

		result = compareMat4(myMat, glmMat);
		failures += result;
		printFunction(result, glmMat, myMat);
	}
	{
		mat4	myMat(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f});
		glm::mat4	glmMat(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
		);

		result = compareMat4(myMat, glmMat);
		failures += result;
		printFunction(result, glmMat, myMat);
	}
	return failures;
}

int	mat4MultiplicationTests()
{
	std::cout << "Matrix multiplication:\n";

	int		failures = 0;
	bool	result;
	void	(*printFunction)(bool, const glm::mat4&, const mat4&) = print4x4TestResult;

	{
		mat4		myMatA(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
		);
		mat4		myMatB(
			{16.0f, 15.0f, 14.0f, 13.0f},
			{12.0f, 11.0f, 10.0f, 9.0f},
			{8.0f, 7.0f, 6.0f, 5.0f},
			{4.0f, 3.0f, 2.0f, 1.0f}
		);
		glm::mat4	glmMatA(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
		);
		glm::mat4	glmMatB(
			{16.0f, 15.0f, 14.0f, 13.0f},
			{12.0f, 11.0f, 10.0f, 9.0f},
			{8.0f, 7.0f, 6.0f, 5.0f},
			{4.0f, 3.0f, 2.0f, 1.0f}
		);

		mat4		myResult = myMatA * myMatB;
		glm::mat4	glmResult = glmMatA * glmMatB;
		result = compareMat4(myResult, glmResult);
		failures += result;
		printFunction(result, glmResult, myResult);
	}
	{
		mat4		myMatA(
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		);
		mat4		myMatB(
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f},
			{17.0f, 18.0f, 19.0f, 20.0f}
		);
		glm::mat4	glmMatA(
			{1.0f, 0.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 1.0f, 0.0f},
			{0.0f, 0.0f, 0.0f, 1.0f}
		);
		glm::mat4	glmMatB(
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f},
			{17.0f, 18.0f, 19.0f, 20.0f}
		);

		mat4		myResult = myMatA * myMatB;
		glm::mat4	glmResult = glmMatA * glmMatB;
		result = compareMat4(myResult, glmResult);
		failures += result;
		printFunction(result, glmResult, myResult);
	}
	return failures;
}

int	mat4TranslationTests()
{
	std::cout << "Matrix translation:\n";

	int		failures = 0;
	bool	result;
	void	(*printFunction)(bool, const glm::mat4&, const mat4&) = print4x4TestResult;

	{
		mat4		myMat{};
		glm::mat4	glmMat{};
		vec3		translation(3.0f, 4.0f, 5.0f);
		glm::vec3	glmTranslation(3.0f, 4.0f, 5.0f);

		myMat.translate(translation);
		glmMat = glm::translate(glmMat, glmTranslation);

		result = compareMat4(myMat, glmMat);
		failures += result;
		printFunction(result, glmMat, myMat);
	}
	{
		mat4		myMat(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
		);
		glm::mat4	glmMat(
			{1.0f, 2.0f, 3.0f, 4.0f},
			{5.0f, 6.0f, 7.0f, 8.0f},
			{9.0f, 10.0f, 11.0f, 12.0f},
			{13.0f, 14.0f, 15.0f, 16.0f}
		);
		vec3		translation(-2.0f, -4.0f, -6.0f);
		glm::vec3	glmTranslation(-2.0f, -4.0f, -6.0f);

		myMat.translate(translation);
		glmMat = glm::translate(glmMat, glmTranslation);

		result = compareMat4(myMat, glmMat);
		failures += result;
		printFunction(result, glmMat, myMat);
	}
	return failures;
}

int	mat3Tests()
{
	int 	failures = 0;

	// failures += mat3InitializerTests();
	// std::cout << "\nMatrix multiplication:\n";
	// failures += mat3MultiplicationTests();
	return failures;
}

int	mat4Tests()
{
	int 	failures = 0;
	
	failures += mat4InitializerTests();
	failures += mat4MultiplicationTests();
	failures += mat4TranslationTests();
	// std::cout << "\nMatrix rotation:\n";
	// failures += mat4RotationTests();
	return failures;
}

int	runMatrixTests()
{
	int	failures = 0;

	failures += mat3Tests();
	if (failures > 0)
	{
		return failures;
	}
	failures += mat4Tests();
	return failures;
}