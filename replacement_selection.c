#include "replacement_selection.h"

//생성자
ReplaceSelection new_ReplaceSelction
(const char* input_filename, const char* output_filename)
{
	ReplaceSelection temp =
	{
		.input_filename = input_filename,
		.input_stream = NULL,
		.record_buffer = {0,},
		.written_flags = {false,},
		.frozen_flags = {false,},
		.run = {0,},
		.run_filename_index = 0,

		.sort = __ReplacementSelction_sort
	};

	return temp;
}

void fill_flags(bool flags[REP_SEL_BUFFER_LENGTH], bool value)
{
	for (int i = 0; i < REP_SEL_BUFFER_LENGTH; i++)
		flags[i] = value;
}

//동결되지 않은 정상값이 남아있는지 확인합니다.
bool has_unfrozen_record(ReplaceSelection const* const self)
{
	for (int i = 0; i < REP_SEL_BUFFER_LENGTH; i++)
		if (self->frozen_flags[i] == false)
			return true;

	return false;
}


//동결되지 않은 정상 값 중에서 최소값을 찾아 인덱스를 반환합니다.
//오류값은 -1입니다.
int find_smallest_unfrozen_record(ReplaceSelection const* const self)
{
	size_t min_index = -1;

	for (int i = 0; i < REP_SEL_BUFFER_LENGTH; i++)
		if (self->frozen_flags[i])
			continue;
		else
		{
			if (min_index == -1)
				min_index = i;
			else
				if (self->record_buffer[min_index] > self->record_buffer[i])
					min_index = i;
		}

	return min_index;
}


//eof 체크
bool is_eof(ReplaceSelection const* const self)
{
	return feof(self->input_stream);
}
bool is_not_eof(ReplaceSelection const* const self)
{
	return !feof(self->input_stream);
}

//하나 읽어옴
value_type read_one(ReplaceSelection* self)
{
	value_type temp;
	fscanf(self->input_stream, "%d ", &temp);
	return temp;
}


//런 하나를 파일로 출력
void output_run(ReplaceSelection* self)
{
	char run_filename[10];
	sprintf(run_filename, "run%02d.txt", self->run_filename_index);
	self->run_filename_index++;
	FILE* output_stream = fopen(run_filename, "wt");

	for (int i = 0; i < REP_SEL_RUN_LENGTH; i++)
		fprintf(output_stream, "%d ", self->run[i]);

	fflush(output_stream);
	fclose(output_stream);
}


//메인 정렬 메서드.
void __ReplacementSelction_sort(ReplaceSelection* self)
{
	self->input_stream = fopen(self->input_filename, "rt");
	if (self->input_stream == NULL)
	{
		puts("No File");
		return;
	}

	for (int i = 0; i < REP_SEL_BUFFER_LENGTH; i++)
	{
		if (is_eof(self))
			break;

		self->record_buffer[i] = read_one(self);
	}

	while (is_not_eof(self))
	{
		for (int i = 0; i < REP_SEL_BUFFER_LENGTH; i++)
			if (!self->written_flags[i])
				self->frozen_flags[i] = false;

		size_t run_index = 0;
		while (has_unfrozen_record(self))
		{
			//최소값 인덱스 획득
			int smallest_unfrozen_index = find_smallest_unfrozen_record(self);

			//런에 추가
			self->run[run_index] = self->record_buffer[smallest_unfrozen_index];

			//빈자리 채움
			if (is_not_eof(self))
			{
				self->record_buffer[smallest_unfrozen_index] = read_one(self);

				//기존 런 뒷값보다 작으면 동결
				if (self->record_buffer[smallest_unfrozen_index] < self->run[run_index])
					self->frozen_flags[smallest_unfrozen_index] = true;

				run_index++;
			}

			//런 꽉차면 파일로 저장
			if (run_index == REP_SEL_RUN_LENGTH)
				output_run(self);
		}
	}

	fclose(self->input_stream);
}


int main()
{
	ReplaceSelection rep_sel =
		new_ReplaceSelction("records.txt", "result.txt");

	rep_sel.sort(&rep_sel);
}