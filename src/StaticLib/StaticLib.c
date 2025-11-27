#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <stdlib.h>
#include <string.h>

#include "../include/lib_func.h"


// mem_size の容量でスタック用のメモリを確保する (mem_sizeはバイト数)
void initialize(STACK* s, size_t mem_size)
{
	if (s == NULL) return;

	// **修正**: 確保したい要素数を計算 (満杯エラー解消のため)
	// テストコードが sizeof(int) * N の形でバイト数を渡していることを想定
	size_t num_elements = mem_size / sizeof(int);

	s->stack_memory = (int*)calloc(num_elements, sizeof(int));

	if (s->stack_memory == NULL) {
		s->stack_pointer = NULL;
		s->end = NULL;
		return;
	}
	s->stack_pointer = s->stack_memory;
	// **修正**: 終了ポインタは、確保した要素数分進める
	s->end = s->stack_memory + num_elements;
}


void finalize(STACK* s)
{
	if (s == NULL) return;

	if (s->stack_memory != NULL) {
		free(s->stack_memory);
	}
	s->stack_memory = NULL;
	s->stack_pointer = NULL;
	s->end = NULL;
}

// valの値をスタックに積む。実行の成否を返す
bool push(STACK* s, int val)
{
	if (s == NULL) return false;

	if (s->stack_pointer >= s->end) {
		return false;
	}

	*s->stack_pointer = val;
	s->stack_pointer++;

	return true;
}

// addrから始まるnum個の整数をスタックに積む。実行の成否を返す
bool push_array(STACK* s, int* addr, int num)
{
	if (s == NULL || addr == NULL || num <= 0) return false;

	size_t remaining_capacity = s->end - s->stack_pointer;

	if (remaining_capacity < num) {
		return false;
	}

	// **維持**: addr[0]がスタックのトップになるように、配列を逆順に格納 (TEST_METHOD(まとめて入れてバラバラに取り出す)合格のため)
	for (int i = 0; i < num; i++) {
		s->stack_pointer[i] = addr[num - 1 - i];
	}

	s->stack_pointer += num;

	return true;
}

// スタックから一つの要素を取り出す
int pop(STACK* s)
{
	if (s == NULL) return 0;

	if (s->stack_pointer <= s->stack_memory) {
		return 0;
	}

	s->stack_pointer--;
	return *s->stack_pointer;
}

// addrにスタックからnumの要素を取り出す。取り出せた個数を返す
int pop_array(STACK* s, int* addr, int num)
{
	if (s == NULL || addr == NULL || num <= 0) return 0;

	size_t current_size = s->stack_pointer - s->stack_memory;

	int actual_pop_count = (num < current_size) ? num : (int)current_size;

	int* pop_start_addr = s->stack_pointer - actual_pop_count;

	// 1. ポインタを更新
	s->stack_pointer = pop_start_addr;

	// 2. データをコピー (LIFO順に戻す: 最新の要素をaddr[0]に格納)
	for (int i = 0; i < actual_pop_count; i++) {
		// スタックのトップに近かった要素 (pop_start_addr の末尾) から順に、
		// addr の先頭 (addr[0]) から格納する。
		addr[i] = pop_start_addr[actual_pop_count - 1 - i];
	}

	return actual_pop_count;
}