#include <iostream>

class Object {
public:
    virtual void show_properties() = 0;
    virtual ~Object() {};
};

class Storage : public Object {
private:
    int current_index = 0;
    int capacity = 10;
    Object** container;
public:
    Storage() : container(new Object* [capacity]) {}
    void show_properties() {
        printf("[Storage::void show_properties()] current_index = %d, capacity = %d\n", current_index, capacity);
    }
    void push_back(Object* obj) {
        if (current_index < capacity) {
            container[current_index] = obj;
            current_index++;
        }
        else {
            Object** new_container = new Object * [capacity * 2];
            for (int i = 0; i < capacity; i++) {
                new_container[i] = container[i];
            }
            new_container[current_index] = obj;
            current_index++;
            container = new_container;
            capacity *= 2;
        }
    }
    ~Storage() override{
        for (int i = 0; i < current_index; i++) {
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
       

}
