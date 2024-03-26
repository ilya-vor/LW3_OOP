﻿#include <iostream>
#include <chrono>
#include <cstdlib>

class Object {
public:
    virtual void show_properties() = 0;
    virtual ~Object() {};
};

class Storage : public Object {
private:
    int index_last_emty_cell = 0;
    int capacity = 10;
    int current_index = 0;
    Object** container;
private:
    bool validation_index(int index) {
        if (index < 0) {
            printf("Ошибка: Вы ввели отрицательный индекс %d. Индекс не может быть отрицательным. Операция не была выполнена.\n", index);
            return false;
        }
        else if (index > index_last_emty_cell - 1) {
            printf("Ошибка: Вы ввели индекс %d. Последний индекс для текущего массива это %d. Операция не была выполнена.\n", index, index_last_emty_cell - 1);
            return false;
        }
        return true;
    }

    void capacityX2() {
        Object** new_container = new Object * [capacity * 2];
        for (int i = 0; i < capacity; i++) {
            new_container[i] = container[i];
        }
        capacity *= 2;
        container = new_container;
        new_container = nullptr;
    }
    void move_all_cells_to_the_right(int starting_from) {
        push_back(container[index_last_emty_cell]);
        for (int i = index_last_emty_cell - 1; i > starting_from; i--) {
            container[i] = container[i - 1];
        }
    }

    void move_all_cells_to_the_left(int starting_from) {
        if (starting_from != index_last_emty_cell - 1) {
            for (int i = starting_from; i < index_last_emty_cell - 1; i++) {
                container[i] = container[i + 1];
            }
            container[index_last_emty_cell - 1] = nullptr;
            index_last_emty_cell--;
        }
    }

public:
    Storage() : container(new Object* [capacity]) {}
    void show_properties() {
        printf("[Storage::void show_properties()] index_last_emty_cell = %d, capacity = %d\n", index_last_emty_cell, capacity);
    }
    
    void show() {
        std::cout << "[Storage::show()]\n";
        for (int i = 0; i < index_last_emty_cell; i++) {
            std::cout << "[" << i << "] = ";
            if (container[i] != nullptr) {
                container[i]->show_properties();
            }
        }
    }

    void push_back(Object* obj) {
        if (index_last_emty_cell >= capacity) {
            capacityX2();
        }
        container[index_last_emty_cell] = obj;
        index_last_emty_cell++;
    }

    void push_middle(Object* obj) {
        if (index_last_emty_cell >= capacity) {
            capacityX2();
        }
         int middle_index = index_last_emty_cell / 2;
         move_all_cells_to_the_right(middle_index);
         container[middle_index] = obj;
    }

    void push_forward(Object* obj) {
        move_all_cells_to_the_right(0);
        container[0] = obj;
    }

    void insert_index(Object* obj, int index) {
        if (validation_index(index)) {
            move_all_cells_to_the_right(index);
            container[index] = obj;
        }
    }

    Object* copy_object_adress(int index) {
        if (validation_index(index)) {
            return container[index];
        }
        else {
            return nullptr;
        }
    }

    Object* cut_object_adress(int index) {
        if (validation_index(index)) {
            Object *obj = container[index];
            move_all_cells_to_the_left(index);
            return obj;
        }
        else {
            return nullptr;
        }
    }
    void delete_objects(int first, int last) {
        for (int i = first; i <= last; i++) {
            delete container[first];
            container[first] = nullptr;
            move_all_cells_to_the_left(first);
        }
    }
    void delete_object(int index) {
        this->delete_objects(index, index);
    }

    void call_method_for_everyone(void(Object::* method)()) {
        for (int i = 0; i < index_last_emty_cell; i++) {
            if (container[i] != nullptr) {
                (container[i]->*method)();
            }
        }
    }

    ~Storage() override{
        for (int i = 0; i < index_last_emty_cell; i++) {
            delete container[i];
            container[i] = nullptr;
        }
    }
};

class Point: public Object {
private:
    int x, y, id;
public:
    Point() : x(0), y(0), id(rand() % 100) {}
    Point(int x, int y): x(x), y(y), id(rand() % 100) {}
    Point(const Point& p): x(p.x), y(p.y), id(rand() % 100)  {}
    void show_properties() {
        printf("[Point::show()] id = %d, x = %d, y = %d\n",id, x, y);
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
    storage->show();
    //storage->cut_object_adress(2)->show_properties();
    storage->delete_objects(1,2);
    storage->show();
    //storage->call_method_for_everyone(&Object::show_properties);
    
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Программа выполнялась " << duration.count() << " миллисекунд\n";


}
