#ifndef __CUSTOM_TYPE__
#define __CUSTOM_TYPE__

#include <string>
#include <ostream>

class Student {
private:
	std::string name;
	unsigned int age;
	unsigned int id;
public:
	Student() : name(""), age(0), id(0) {
	}

	Student(std::string name, unsigned int age, unsigned int id) : name(name), age(age), id(id) {
	}

	void setStudentInfo(std::string name, unsigned int age, unsigned int id) {
		this->name = name;
		this->age = age;
		this->id = id;
	}

	Student& operator=(const Student& other) {
		name = other.name;
		age = other.age;
		id = other.id;

		return *this;
	}

	bool operator==(const Student& other) const {
		if (name != other.name) return false;
		else if (age != other.age) return false;
		else if (id != other.id) return false;
		else return true;
	}

	bool operator!=(const Student& other) const {
		if (name == other.name) return false;
		else if (age == other.age) return false;
		else if (id == other.id) return false;
		else return true;
	}

	bool operator<(const Student& other) const {
		return id < other.id;
	}

	bool operator>(const Student& other) const {
		return id > other.id;
	}

	friend std::ostream& operator<< (std::ostream& out, Student& student);
};

std::ostream& operator<< (std::ostream& out, Student& student) {
	out << student.id << " " << student.name << " (" << student.age << ")";
	return out;
}

#endif // !__CUSTOM_TYPE__
