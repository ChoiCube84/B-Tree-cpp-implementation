#include <random>
#include <string>
#include <vector>
#include <iostream>

#include "b_tree.h"
#include "custom_type.h"

using namespace std;

void CustomTypeTest(void);

int main(void) {
	CustomTypeTest();
	return 0;
}

void CustomTypeTest(void) {
	BTree<Student>* tree = new BTree<Student>(5);

	std::random_device rd;
	std::mt19937 gen(rd());

	std::uniform_int_distribution<int> randomA(0, 19);
	std::uniform_int_distribution<int> randomB(0, 19);
	std::uniform_int_distribution<int> randomC(0, 19);
	std::uniform_int_distribution<unsigned int> randomAge(18, 30);
	std::uniform_int_distribution<unsigned int> randomId(20220001, 20229999);

	vector<string> A = { "¼Õ", "ÀÌ", "Á¤", "±è", "Á¤", "±è", "Á¤", "±è", "À±", "¼®", "À±", "¹Ú", "±è", "ÃÖ", "±Ç", "±è", "ÃÖ", "±è", "Á¤", "Á¤" };
	vector<string> B = { "Âù", "À¯", "Áø", "ÁØ", "Ä¡", "½É", "Áö", "ÇØ", "  ", "°­", "Àç", "Áö", "ÇØ", "¼¼", "°æ", "¹Î", "Çö", "¿¬", "°æ", "Çö" };
	vector<string> C = { "¹Î", "¼º", "¹Î", "¿õ", "ÅÃ", "¼Ö", "·®", "¿ø", "Çö", "¿ì", "Àº", "¸ñ", "±¸", "¼­", "Áß", "Çü", "¼Ö", "¿õ", "¿µ", "ºó" };

	Student student;
	vector<Student> students;

	size_t numberOfStudents = 200;

	cout << "========== Insert test start ==========" << endl;
	for (size_t i = 0; i < numberOfStudents; i++) {
		string name = "";
		
		name += A[randomA(rd)];
		name += B[randomB(rd)];
		name += C[randomC(rd)];

		unsigned int age = randomAge(rd);
		unsigned int id = randomId(rd);
		
		student.setStudentInfo(name, age, id);

		students.emplace_back(student);
		tree->insert(student);

		cout << tree->preOrder(true) << endl << endl;
	}
	cout << "========== Insert test done! ==========" << endl << endl;

	shuffle(students.begin(), students.end(), gen);

	cout << "========== Delete test start ==========" << endl;
	for (auto i : students) {
		tree->remove(i);
		cout << tree->preOrder(true) << endl << endl;
	}
	cout << "========== Delete test done! ==========" << endl << endl;

	delete tree;
}