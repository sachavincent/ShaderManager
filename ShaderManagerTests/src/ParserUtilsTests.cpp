#include "stdafx.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utils/ParserUtils.h>

using namespace ::testing;

TEST(ParserUtilsTests_FindPositionsOfSubstring, EmptyStringEmptySubstring) {
	// Empty string and empty substring, we expect to get 0 matches
	auto res = sm::parsing::utils::FindPositionsOfSubstring("", "");

	ASSERT_THAT(res, IsEmpty());
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, EmptyStringNonEmptySubstring) {
	// Empty string and non-empty substring, we expect to get 0 matches
	auto res = sm::parsing::utils::FindPositionsOfSubstring("", "Foo");

	ASSERT_THAT(res, IsEmpty());
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, NonEmptyStringEmptySubstring) {
	// Non-empty string and empty substring, we expect to get 0 matches
	auto res = sm::parsing::utils::FindPositionsOfSubstring("Bar", "");

	ASSERT_THAT(res, IsEmpty());
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, NoMatch) {
	// No match for substring in string, we expect to get 0 matches
	auto res = sm::parsing::utils::FindPositionsOfSubstring("Bar", "Foo");

	ASSERT_THAT(res, IsEmpty());
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, Equals) {
	// String and substring are equal, we expect to get 1 match at index 0
	auto res = sm::parsing::utils::FindPositionsOfSubstring("Bar", "Bar");

	ASSERT_THAT(res, ElementsAre(0));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, OneMatchStart) {
	// String contains substring at the start, we expect to get 1 match at index 0
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarFoo", "Bar");

	ASSERT_THAT(res, ElementsAre(0));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, OneMatchMiddle) {
	// String contains substring in the middle, we expect to get 1 match at index 3
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarFooBaz", "Foo");

	ASSERT_THAT(res, ElementsAre(3));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, OneMatchEnd) {
	// String contains substring at the end (last char), we expect to get 1 match at index 8
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarFooBaz", "z");

	ASSERT_THAT(res, ElementsAre(8));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, TwoMatches) {
	// String contains 2 instances of substring, we expect to get 2 matches at index 0 and 6
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarFooBar", "Bar");

	ASSERT_THAT(res, ElementsAre(0, 6));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, TwoMatchesSequential) {
	// String contains 2 instances of substring next to each other, we expect to get 2 matches at index 0 and 3
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarBarFoo", "Bar");

	ASSERT_THAT(res, ElementsAre(0, 3));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, TwoMatchesJoined) {
	// String contains 2 instances of substring which are interlocked, we expect to get 2 matches at index 0 and 3
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarBarBarFoo", "BarB");

	ASSERT_THAT(res, ElementsAre(0, 3));
}

TEST(ParserUtilsTests_FindPositionsOfSubstring, ThreeMatches) {
	// String contains 3 instances of substring, we expect to get 3 matches at index 0 and 6 and 12
	auto res = sm::parsing::utils::FindPositionsOfSubstring("BarFooBarFooBar", "Bar");

	ASSERT_THAT(res, ElementsAre(0, 6, 12));
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNoHash) {
	auto res = sm::parsing::utils::CheckIncludeFormat("include \"empty.glsl\"\n");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNoQuotes) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include empty.glsl\n");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNothing) {
	auto res = sm::parsing::utils::CheckIncludeFormat("");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNoString) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include \"\"\n");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNoSpace) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include\"empty.glsl\"\n");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorStringSpace) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include \"empty. glsl\"\n");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, ErrorNoNewLine) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include \"empty.glsl\"");

	ASSERT_THAT(res, false);
}

TEST(ParserUtilsTests_CheckIncludeFormat, SuccessExtraSpaces) {
	auto res = sm::parsing::utils::CheckIncludeFormat("   #include     \"empty.glsl\"         \n");

	ASSERT_THAT(res, true);
}

TEST(ParserUtilsTests_CheckIncludeFormat, SuccessNormal) {
	auto res = sm::parsing::utils::CheckIncludeFormat("#include \"empty.glsl\"\n");

	ASSERT_THAT(res, true);
}

