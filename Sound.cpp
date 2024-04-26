#include <iostream>
#include "wavfile.h"
using namespace std;

void allocateArray(unsigned char*& arr, int size) {
	arr = new unsigned char[size];
}

void deallocateArray(unsigned char*& arr) {
	delete[] arr;
}

void playfile(char* str) {
	if (playWavFile(str) == 0)
		cout << "Error: File not found!" << endl;
}

void read_data(char* str) {
	int sampleRate = 0, size = 100000000;
	unsigned char* ptr = new unsigned char[size];
	readWavFile(str, ptr, size, sampleRate);
	delete[] ptr;
}

void doubleArray(unsigned char*& ptr, unsigned char*& up_ptr, int size) {
	for (int i = 0, x = 0; i <= size; i++, x++) {
		up_ptr[x] = ptr[i];
		up_ptr[++x] = ptr[i];
	}
}

void shrinkArray(unsigned char*& ptr, unsigned char*& down_ptr, int size) {
	for (int i = 0, x = 0; i <= size / 2; i++, x++) {
		down_ptr[i] = ptr[x];
		x++;
	}
}

void upSample(char* str) {
	int sampleRate = 0, size = 100000000;
	unsigned char* ptr;
	allocateArray(ptr, size);
	readWavFile(str, ptr, size, sampleRate);
	unsigned char* up_ptr;
	allocateArray(up_ptr, size * 2);
	doubleArray(ptr, up_ptr, size);
	char new_str[50] = "UpSample.wav";
	writeWavFile(new_str, up_ptr, size * 2, sampleRate);
	playfile(new_str);
	deallocateArray(ptr);
}


void downSample(char* str) {
	int sampleRate = 0, size = 100000000;
	unsigned char* ptr;
	allocateArray(ptr, size);
	readWavFile(str, ptr, size, sampleRate);
	unsigned char* down_ptr;
	allocateArray(down_ptr, size / 2);
	shrinkArray(ptr, down_ptr, size);
	char new_str[50] = "DownSample.wav";
	writeWavFile(new_str, down_ptr, (size / 2), sampleRate);
	playfile(new_str);
	deallocateArray(ptr);
}


void fillwithmean(unsigned char*& in, unsigned char*& out, int n, int size) {
	for (int i = 0; i < size; i++) {
		int sum = 0, count = 0;
		for (int j = i - n; j < i + n; j++) {
			if (i >= 0 && i < size) {
				sum = sum + in[j];
				count++;
			}
		}
		out[i] = sum / count;
	}
}


void mergeArray(unsigned char*& arr1, unsigned char*& arr2, int size1, int size2, unsigned char*& ptr) {
	int i = 0, j = 0, a = 0;
	while (i < size1 && j < size2) {
		ptr[a++] = arr1[i++];
		ptr[a++] = arr2[j++];
	}
	while (i < size1) {
		ptr[a++] = arr1[i++];
	}
	while (j < size2) {
		ptr[a++] = arr2[j++];
	}
}


int main(){
	char str[50] = "";
	bool flag = true;
	while (flag) {
		cout << "Enter file name with (.wav) or exit: ";
		cin >> str;
		if (strcmp(str, "exit") != 0) {
			read_data(str);
			playfile(str);
			int input = 0;
			while (input != 5) {
				cout << endl << "Press 1 to Up Sample." << endl << "Press 2 to Down Sample." << endl << "Press 3 to Move Average." << endl;
				cout << "Press 4 to Merge with another file." << endl << "Press 5 to try another file!" << endl;
				cin >> input;
				if (input == 1) {
					upSample(str);
				}
				if (input == 2) {
					downSample(str);
				}
				if (input == 3) {
					int sampleRate = 0, size = 100000000;
					unsigned char* ptr = new unsigned char[size];
					readWavFile(str, ptr, size, sampleRate);
					unsigned char* ptr2 = new unsigned char[size];
					int n = 0;
					cout << "Enter Value for N: ";
					cin >> n;
					fillwithmean(ptr, ptr2, n, size);
					char new_str[50] = "FWM_Sample.wav";
					writeWavFile(new_str, ptr2, size, sampleRate);
					playfile(new_str);
					deallocateArray(ptr);
					deallocateArray(ptr2);
				}
				if (input == 4) {
					int sampleRate = 0, size = 100000000;
					unsigned char* ptr = new unsigned char[size];
					readWavFile(str, ptr, size, sampleRate);
					char str2[50];
					cout << "Enter second file name with (.wav) or exit: ";
					cin >> str2;
					int sampleRate2 = 0, size2 = 100000000;
					unsigned char* ptr2 = new unsigned char[size2];
					readWavFile(str2, ptr2, size2, sampleRate2);
					unsigned char* ptr3 = new unsigned char[size + size2];
					mergeArray(ptr, ptr2, size, size2, ptr3);
					char name1[20] = "Merged_1.wav";
					cout << endl << "Merged_1.wav with Sampling Rate of " << str << endl << endl;
					writeWavFile(name1, ptr3, (size + size2), sampleRate);
					playfile(name1);
					char name2[20] = "Merged_2.wav";
					cout << endl << "Merged_2.wav with Sampling Rate of " << str2 << endl << endl;
					writeWavFile(name2, ptr3, (size + size2), sampleRate2);
					playfile(name2);
				}
			}
		}
		else
			flag = 0;
	}
	return 0;
}