#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>

//버퍼사이즈
#define REP_SEL_BUFFER_LENGTH (10) 

//런 사이즈
#define REP_SEL_RUN_LENGTH (10) 

//런의 최대 개수??
#define __RUN_COUNT_MAX (10)

//레코드 값타입
typedef int value_type;

//대체 선택 클래스
typedef struct ReplaceSelection
{
	const char* const input_filename;

	FILE* input_stream;

	value_type record_buffer[REP_SEL_BUFFER_LENGTH];
	bool written_flags[REP_SEL_BUFFER_LENGTH];
	bool frozen_flags[REP_SEL_BUFFER_LENGTH];

	value_type run[REP_SEL_RUN_LENGTH];
	int run_filename_index;

	//public 메서드
	void (*sort)(struct ReplaceSelection*);
} ReplaceSelection;

ReplaceSelection new_ReplaceSelction(const char* input_filename, const char* output_filename);

void __ReplacementSelction_sort(ReplaceSelection* self);

//private 메서드
int find_smallest_unfrozen_record(ReplaceSelection const* const self);
bool has_unfrozen_record(ReplaceSelection const* const self);
bool is_eof(ReplaceSelection const* const self);
bool is_not_eof(ReplaceSelection const* const self);