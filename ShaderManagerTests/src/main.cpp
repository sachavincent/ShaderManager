#include "stdafx.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>

#define _HAS_EXCEPTIONS 0
#include "utils/FileWatcher.h"
#include "ShaderFile.h"
#include "ShaderManager.h"
// Global operator new overloads
//void* operator new(std::size_t size) {
//    void* ptr = std::malloc(size);
//    if (!ptr) throw std::bad_alloc();
//    return ptr;
//}
//
//void* operator new[](std::size_t size) {
//    void* ptr = std::malloc(size);	
//    if (!ptr) throw std::bad_alloc();
//    return ptr;
//}
//
//// Global operator delete overloads
//void operator delete(void* ptr) noexcept {
//    if (ptr) {
//        std::free(ptr);
//    }
//}
//
//void operator delete[](void* ptr) noexcept {
//    if (ptr) {
//        std::free(ptr);
//    }
//}
//
//void operator delete(void* ptr, std::size_t) noexcept {
//    if (ptr) {
//        std::free(ptr);
//    }
//}
//
//void operator delete[](void* ptr, std::size_t) noexcept {
//    if (ptr) {
//        std::free(ptr);
//    }
//}
int main(int argc, char** argv) {

	sm::ShaderManager m;
	sm::utils::FileWatcher f(m);
	f.AddFile(sm::ShaderFile{ L"C:\\Users\\sacha\\Documents\\repos\\ShaderManager\\ShaderManagerTests\\assets\\ShaderManagerTests\\Shaders", sm::ShaderType::MESH });

	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}