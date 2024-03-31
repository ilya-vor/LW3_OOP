#include <iostream>
#include <chrono>
#include <cstdlib>
class Object {
public:
    virtual void show_properties() = 0;
    virtual void clear_properties() = 0;
    virtual ~Object() {};
};
class Point : public Object {
private:
    int x, y;
    int id = rand() % 100;
public:
    Point() : x(0), y(0) {}
    Point(int x, int y) : x(x), y(y) {}
    Point(const Point& p) : x(p.x), y(p.y) {}
    void show_properties() { printf("[Point::show()] id = %d, x = %d, y = %d\n", id, x, y); }
    void set_properties(int x, int y) { this->x = x; this->y = y; }
    void set_properties(Point& p) { this->x = p.x; this->y = p.y; }
    void clear_properties() override{ x = 0; y = 0; id = 0; }
    ~Point() override {}
};
class Section : public Object {
private:
    Point* strt;
    Point* end;
    int id = rand() % 100;
public:
    Section() : strt(new Point()), end(new Point()) { }
    Section(int x1, int y1, int x2, int y2) : strt(new Point(x1, y1)), end(new Point(x2, y2)) { }
    Section(const Section& s) : strt(new Point(*(s.strt))), end(new Point(*(s.end))) { }
    void show_properties() override {
        printf("[Section::show_properties()] id = %d\n", id);
        strt->show_properties();
        end->show_properties();
    }
    void clear_properties() override { id = 0; }
    ~Section() {
        delete(strt);
        delete(end);
        strt = nullptr;
        end = nullptr;
    }
};
class Storage{
private:
    int index_last_emty_cell = 0;
    int capacity = 10;
    Object** container;
private:
    bool validation_index(int index) {
        int last_index = this->get_index_last_cell();
        if (index < 0) {
            printf("Ошибка: Вы ввели отрицательный индекс %d. Индекс не может быть отрицательным. Операция не была выполнена.\n", index);
            return false;
        }
        else if (index > last_index) {
            printf("Ошибка: Вы ввели индекс %d. Последний индекс для текущего массива это %d. Операция не была выполнена.\n", index, last_index);
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
        for (int i = index_last_emty_cell - 1; i > starting_from; i--) { container[i] = container[i - 1]; }
    }
    void move_all_cells_to_the_left(int starting_from) {
        for (int i = starting_from; i < index_last_emty_cell - 1; i++) { container[i] = container[i + 1]; }
        container[index_last_emty_cell - 1] = nullptr;
        index_last_emty_cell = std::max(0, index_last_emty_cell - 1);
    }
public:
    Storage() : container(new Object* [capacity]) {}
    Storage(int len) : container(new Object* [len]), capacity(len) {}
    void show_properties() {
        printf("[Storage::void show_properties()] index_last_emty_cell = %d, capacity = %d\n", index_last_emty_cell, capacity);
    }
    void show_all_objects() {
        std::cout << "[Storage::show()]\n";
        for (int i = 0; i < index_last_emty_cell; i++) {
            std::cout << "[" << i << "] = ";
            container[i]->show_properties();
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
        if (validation_index(index)) { return container[index]; }
        else { return nullptr; }
    }
    Object* cut_object_adress(int index) {
        if (validation_index(index)) {
            Object *obj = container[index];
            move_all_cells_to_the_left(index);
            return obj;
        }
        else { return nullptr; }
    }
    void delete_objects(int first, int last) {
        if (validation_index(first) && validation_index(last)) {
            for (int i = first; i <= last; i++) {
                delete container[first];
                container[first] = nullptr;
                move_all_cells_to_the_left(first);
            }
        }
    }
    void delete_object(int index) { this->delete_objects(index, index); }
    void set_object(int index, Object* obj) {
        if (validation_index(index)) {
            container[index] = obj;
        }
    }
    void call_method_for_everyone(void(Object::* method)()) {
        for (int i = 0; i < index_last_emty_cell; i++) {
            (container[i]->*method)();
        }
    }
    int get_index_last_cell() { return std::max(0, this->index_last_emty_cell - 1); }
    void random_action() {
        int i = rand() % 10;
        int rand_index = rand() % (this->get_index_last_cell() + 1);
        int rand_index2 = rand() % (this->get_index_last_cell() + 1);
        if (i < 5) {
            Point* p0 = new Point();
            if (i == 0) { this->push_back(p0); }
            else if (i == 1) { this->push_middle(p0); }
            else if (i == 2) { this->push_forward(p0); }
            else if (i == 3) { this->insert_index(p0, rand_index); }
            else { this->copy_object_adress(rand_index); }
        }
        else {
            Section* s0 = new Section();
            if (i == 5) { this->cut_object_adress(rand_index); }
            else if (i == 6) { this->delete_objects(rand_index, rand_index2); }
            else if (i == 7) { this->delete_object(rand_index); }
            else if (i == 8) { this->set_object(rand_index, s0); }
            else { this->call_method_for_everyone(&Object::clear_properties); }
        }
    }
    ~Storage(){
        for (int i = 0; i < index_last_emty_cell; i++) {
            delete container[i];
            container[i] = nullptr;
        }
        container = nullptr;
    }
};
int main()
{
    setlocale(NULL,"RU");
    auto start = std::chrono::steady_clock::now();
    Storage* storage = new Storage(10);
    for (int i = 0; i <= 100;  i++) {
        storage->random_action();
    }
    for (int i = 0; i <= 1000; i++) {
        storage->random_action();
    }
    for (int i = 0; i <= 10000; i++) {
        storage->random_action();
    }
    delete storage;
    auto end = std::chrono::steady_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Программа выполнялась " << duration.count() << " миллисекунд\n";
}