#include "header.h"

int main() {
	
	/*int* int_num = (int*)my_malloc(sizeof(int) * 3);
	int_num[0] = 2345664;
	int_num[1] = 23;
	int_num[2] = 34;
	output_information();

	my_free(int_num);
	output_information();

	defrag_memory();
	output_information();	*/

	int* int_num2 = (int*)my_malloc(sizeof(int));
	*int_num2 = 764633;
	output_information();

	double* double_num = (double*)my_malloc(sizeof(double));
	*double_num = 23456.5366;
	output_information();

	//my_free(int_num);
	//output_information();

	/*my_free(int_num2);
	output_information();

	double* double_num2 = (double*)my_malloc(sizeof(double));
	*double_num2 = 23456.5366;
	output_information();

	my_free(double_num);
	output_information();

	defrag_memory();
	output_information();*/

	/*double* double_num3 = (double*)my_malloc(sizeof(double));
	*double_num3 = 23456.5366;
	output_information();*/
	
	/*char* char_ptr = (char*)my_malloc(sizeof(char));
	*char_ptr = 33;
	output_information();*/

	//my_free(int_num);
	/*my_free(double_num);
	output_information();*/

	/*defrag_memory();
	output_information();*/
	
	return 0;
}