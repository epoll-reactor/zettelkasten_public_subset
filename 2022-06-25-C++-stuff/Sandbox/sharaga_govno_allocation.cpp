// Dec 3, 2021
#include <iostream>

class object {
public:
	virtual ~object() {}

	virtual void print() const = 0;
};

class a : public object {
public:
	void print() const override { std::cout << 'a'; }
};
class b : public object {
public:
	void print() const override { std::cout << 'b'; }
};
class c : public object {
public:
	void print() const override { std::cout << 'c'; }
};
class d : public object {
public:
	void print() const override { std::cout << 'd'; }
};

class area {
public:
	area(int width, int height)
		: width_ (width)
		, height_(height) {
		objects_ = new object**[width_];
		for (int i = 0; i < width_; ++i) {
			objects_[i] = new object*[height_]{0};
		}
	}

	~area() {
		for (int i = 0; i < width_; ++i) {
			for (int j = 0; j < height_; ++j) {
				delete objects_[i][j];
			}
			delete[] objects_[i];
		}
		delete[] objects_;
	}

	void add(int at_height, int at_width, char what) {
		object*& pointer = objects_[at_width][at_height];
		delete pointer;
		switch (what) {
			case 'a': pointer = new a; break;
			case 'b': pointer = new b; break;
			case 'c': pointer = new c; break;
			case 'd': pointer = new d; break;
			default: break;
		}
	}

	void change(int at_height, int at_width, char from, char to) {
		if (from == to) return;
		object*& pointer = objects_[at_width][at_height];
		delete pointer;
		switch (to) {
			case 'a': pointer = new a; break;
			case 'b': pointer = new b; break;
			case 'c': pointer = new c; break;
			case 'd': pointer = new d; break;
			default: break;
		}
	}

	void move(int from_height, int from_width, int to_height, int to_width) {
		object*& from_pointer = objects_[from_width][from_height];
		object*&   to_pointer = objects_[  to_width][  to_height];
		delete to_pointer;
		to_pointer = from_pointer;
		from_pointer = nullptr;
	}

	friend std::ostream& operator<<(std::ostream& out_stream, const area& a) {
		for (size_t i = 0; i < a.width_; ++i) {
			for (size_t j = 0; j < a.height_; ++j) {
				if (a.objects_[i][j]) {
					a.objects_[i][j]->print();
				} else {
					out_stream << '.';
				}
			}
			out_stream << '\n';
		}
		return out_stream << '\n';
	}

private:
	int       width_;
	int       height_;
	object*** objects_;
};

int main() {
	area a(10, 20);
	a.add(0, 0, 'a');
	a.add(3, 2, 'b');
	a.add(9, 8, 'c');
	a.add(2, 7, 'd');
	std::cout << a;
	a.change(2, 7, 'd', 'c');
	std::cout << a;
	a.move(0, 0, 1, 1);
	std::cout << a;
	a.move(1, 1, 3, 2);
	std::cout << a;
}
