/* By Ethan Hughes */
/* Written on 1/5/2023 */

#include"mergesort-struct.h"

//Recursive merging function, returns start of sorted list
struct pair *merge(int len, struct pair arr[], int a, int b){
	if(len == 1){ //Return single value
		return arr;
	} else if (len == 2){ //Check whether to swap when given two values
		if(arr[a].key < arr[b].key){
			struct pair temp = arr[a];
			arr[a] = arr[b];
			arr[b] = temp;
			return arr;
		} else {
			return arr;
		}
	} else { //Run recursive merge when given more than two values
		int c = (a+b)/2;
		int d = c+1;

		merge(len-len/2, arr, a, c);

		merge(len/2, arr, d, b);

		//Sort returned values and return sorted sub-list
		struct pair temp[len];
		int c1 = a;
		int c2 = d;
		for(int i = 0; i < len; i++){
			if(c1 < d && c2 <= b){
				if(arr[c1].key >= arr[c2].key){
					temp[i] = arr[c1];
					c1++;
				} else {
					temp[i] = arr[c2];
					c2++;
				}
			} else if(c1 < d){
				temp[i] = arr[c1];
				c1++;
			} else {
				temp[i] = arr[c2];
				c2++;
			}
		}
		for(int i = 0; i < len; i++){
			arr[a+i] = temp[i];
		}
		return arr;
	}
}

//Base merge sorting function, implements recursive function, returns start of sorted list
struct pair *mergesort(int len, struct pair values[]){
	if(len > 1){
		merge(len, values, 0, len-1);
	}
	return values;
}
