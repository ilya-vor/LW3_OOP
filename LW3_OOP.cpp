#include <iostream>
#include <chrono>
#include <cstdlib>

class Object {
public:
    virtual void show_properties() = 0;
    virtual ~Object() {};
};

class Storage : public Object {
private:
    int index_last = 0;
    int capacity = 10;
    int current_index = 0;
    Object** container;
public:
    Storage() : container(new Object* [capacity]) {}
    void show_properties() {
        printf("[Storage::void show_properties()] index_last = %d, capacity = %d\n", index_last, capacity);
    }
    
    void show() {
        for (int i = 0; i < index_last; i++) {
            std::cout << "[" << i << "] = ";
            container[i]->show_properties();
            std::cout << "\n";
        }
    }

    void push_back(Object* obj) {
        if (index_last < capacity) {
            container[index_last] = obj;
            index_last++;
        }
        else {
            Object** new_container = new Object * [capacity * 2];
            for (int i = 0; i < capacity; i++) {
                new_container[i] = container[i];
            }
            new_container[index_last] = obj;
            index_last++;
            container = new_container;
            capacity *= 2;
        }
    }
    void push_middle(Object* obj) {
         show();
         int middle_index = index_last / 2;
         push_back(container[index_last]);
         index_last++;
         for (int i = index_last - 1; i > middle_index; i--) {
             container[i] = container[i - 1];
        }
         container[middle_index] = obj;
         show();
    }
    ~Storage() override{
        for (int i = 0; i < index_last; i++) {
            delete (container[i]);
        }
    }
};

class Point: public Object {
private:
    int x, y;
public:
    Point(): x(0), y(0) {}
    Point(int x, int y): x(x), y(y) {}
    Point(const Point& p): x(p.x), y(p.y) {}
    void show_properties() {
        printf("[Point::show()] x = %d, y = %d\n", x, y);
    }
    void set_properties(int x, int y) {
        this->x = x;
        this->y = y;
    }
    void set(Point& p) {
        this->x = p.x;
        this->y = p.y;
    }
    ~Point() override{}
};

int main()
{
    setlocale(NULL,"RU");
    auto start = std::chrono::steady_clock::now();
    
    Storage* storage = new Storage();
    for (int i = 0; i < 3;  i++) {
        Point* p = new Point(rand() % 10, rand() % 10);
        storage->push_back(p);
    }
    Point* p = new Point(rand() % 10, rand() % 10);
    storage->push_middle(p);
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Программа выполнялась " << duration.count() << " миллисекунд\n";


}
