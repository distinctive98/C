#include<stdio.h>
#include<string.h>

#include "jansson.h"

int main(void)
{
	//일반 테스트용 변수
	json_t	*root = NULL;
	char	*str;

	//배열 테스트용 변수
	json_t *arr;
	json_t *json;
	int i;

	root = json_object(); //json 데이터를 담을 수 있게 됨	
	json_object_set(root, "cust_id", json_integer(123));	//key:"cust_id", value:123
	json_object_set(root, "cust_name", json_string("abcde"));

	str = json_dumps(root, JSON_ENCODE_ANY);	//json 문자열 추출
	printf("str = %s\n", str);

	free(str);	//반드시 메모리 해제를 시켜주어야 함
	json_decref(root);	//반드시 사용한 자원을 반납해야 함

	//배열 테스트
	root = json_object();
	arr = json_array();
	
	for(i=0; i<10; i++)
	{
		json = json_object();
		json_object_set(json, "cust_id", json_integer(i+1));
		json_object_set(json, "cust_name", json_string("test"));

		/* json_array_append가 계속 추가하는 함수, set은 마지막만 변경됨 */
		json_array_append(arr, json);
	}
	/* array를 root에 set */
	json_object_set(root, "arr", arr);

	str = json_dumps(root, JSON_ENCODE_ANY);
	printf("str = %s\n", str);

	free(str);
	/* root에 다 포함되므로 따로 해제 안 해도 됨 */
	json_decref(root);
}
