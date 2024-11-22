#include <iostream>
#include <string>
#include <vector>

struct Student {
    std::string name;
    int id;

    Student(std::string n, int i) : name(n), id(i) {}
};

struct NodeName {
    std::string name;
    int index;
    NodeName *left, *right;
    NodeName(std::string n, int i) : name(n), index(i), left(nullptr), right(nullptr) {}
};

struct NodeID {
    int id;
    int index;
    NodeID *left, *right;
    NodeID(int i, int idx) : id(i), index(idx), left(nullptr), right(nullptr) {}
};

NodeName* insertName(NodeName* root, std::string name, int index) {
    if (root == nullptr) return new NodeName(name, index);
    if (name < root->name) root->left = insertName(root->left, name, index);
    else root->right = insertName(root->right, name, index);
    return root;
}

NodeID* insertID(NodeID* root, int id, int index) {
    if (root == nullptr) return new NodeID(id, index);
    if (id < root->id) root->left = insertID(root->left, id, index);
    else root->right = insertID(root->right, id, index);
    return root;
}

void inorderTraversalName(NodeName* root, const std::vector<Student>& students) {
    if (root != nullptr) {
        inorderTraversalName(root->left, students);
        std::cout << "Имя: " << root->name << ", ID: " << students[root->index].id << std::endl;
        inorderTraversalName(root->right, students);
    }
}

void inorderTraversalID(NodeID* root, const std::vector<Student>& students) {
    if (root != nullptr) {
        inorderTraversalID(root->left, students);
        std::cout << "ID: " << root->id << ", Имя: " << students[root->index].name << std::endl;
        inorderTraversalID(root->right, students);
    }
}

bool searchByName(NodeName* root, const std::vector<Student>& students, const std::string& nameToFind) {
    if (root == nullptr) return false;
    if (nameToFind == root->name) {
        std::cout << "Студент найден:\n";
        std::cout << "Имя: " << students[root->index].name << ", ID: " << students[root->index].id << std::endl;
        return true;
    } else if (nameToFind < root->name) {
        return searchByName(root->left, students, nameToFind);
    } else {
        return searchByName(root->right, students, nameToFind);
    }
}

bool searchByID(NodeID* root, const std::vector<Student>& students, int idToFind) {
    if (root == nullptr) return false;
    if (idToFind == root->id) {
        std::cout << "Студент найден:\n";
                std::cout << "ID: " << students[root->index].id << ", Имя: " << students[root->index].name << std::endl;
        return true;
    } else if (idToFind < root->id) {
        return searchByID(root->left, students, idToFind);
    } else {
        return searchByID(root->right, students, idToFind);
    }
}

NodeName* removeName(NodeName* root, const std::string& nameToRemove) {
    if (root == nullptr) return root;
    if (nameToRemove < root->name) {
        root->left = removeName(root->left, nameToRemove);
    } else if (nameToRemove > root->name) {
        root->right = removeName(root->right, nameToRemove);
    } else {
        if (root->left == nullptr) {
            NodeName* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            NodeName* temp = root->left;
            delete root;
            return temp;
        }
        NodeName* temp = root->right;
        while (temp->left != nullptr) temp = temp->left;
        root->name = temp->name;
        root->index = temp->index;
        root->right = removeName(root->right, temp->name);
    }
    return root;
}

NodeID* removeID(NodeID* root, int idToRemove) {
    if (root == nullptr) return root;
    if (idToRemove < root->id) {
        root->left = removeID(root->left, idToRemove);
    } else if (idToRemove > root->id) {
        root->right = removeID(root->right, idToRemove);
    } else {
        if (root->left == nullptr) {
            NodeID* temp = root->right;
            delete root;
            return temp;
        } else if (root->right == nullptr) {
            NodeID* temp = root->left;
            delete root;
            return temp;
        }
        NodeID* temp = root->right;
        while (temp->left != nullptr) temp = temp->left;
        root->id = temp->id;
        root->index = temp->index;
        root->right = removeID(root->right, temp->id);
    }
    return root;
}

void deleteTree(NodeName* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void deleteTree(NodeID* root) {
    if (root != nullptr) {
        deleteTree(root->left);
        deleteTree(root->right);
        delete root;
    }
}

void removeRecord(NodeName*& nameRoot, NodeID*& idRoot, std::vector<Student>& students, const std::string& nameToRemove) {
    int indexToRemove = -1;
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].name == nameToRemove) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        std::cout << "Студент с именем \"" << nameToRemove << "\" не найден.\n";
        return;
    }

    nameRoot = removeName(nameRoot, nameToRemove);
    idRoot = removeID(idRoot, students[indexToRemove].id);
    students.erase(students.begin() + indexToRemove);

    for (int i = 0; i < students.size(); ++i){
        nameRoot = removeName(nameRoot, students[i].name);
        idRoot = removeID(idRoot, students[i].id);
        nameRoot = insertName(nameRoot, students[i].name, i);
        idRoot = insertID(idRoot, students[i].id, i);
    }

    std::cout << "Запись удалена.\n";

    std::cout << "\nОтсортированный список студентов по имени (после удаления):\n";
    inorderTraversalName(nameRoot, students);

    std::cout << "\nОтсортированный список студентов по ID (после удаления):\n";
    inorderTraversalID(idRoot, students);
}

void removeRecordById(NodeName*& nameRoot, NodeID*& idRoot, std::vector<Student>& students, int idToRemove) {
    int indexToRemove = -1;
    for (size_t i = 0; i < students.size(); ++i) {
        if (students[i].id == idToRemove) {
            indexToRemove = i;
            break;
        }
    }

    if (indexToRemove == -1) {
        std::cout << "Студент с ID " << idToRemove << " не найден.\n";
        return;
    }

    nameRoot = removeName(nameRoot, students[indexToRemove].name);
    idRoot = removeID(idRoot, idToRemove);
    students.erase(students.begin() + indexToRemove);

    for (int i = 0; i < students.size(); ++i){
        nameRoot = removeName(nameRoot, students[i].name);
        idRoot = removeID(idRoot, students[i].id);
        nameRoot = insertName(nameRoot, students[i].name, i);
        idRoot = insertID(idRoot, students[i].id, i);
    }

    std::cout << "Запись удалена.\n";

    std::cout << "\nОтсортированный список студентов по имени (после удаления):\n";
    inorderTraversalName(nameRoot, students);

    std::cout << "\nОтсортированный список студентов по ID (после удаления):\n";
    inorderTraversalID(idRoot, students);
}


int main() {
    int n;
    std::cout << "Введите количество студентов: ";
    std::cin >> n;
    std::cin.ignore(); 

    std::vector<Student> students;
    for (int i = 0; i < n; ++i) {
        std::string name;
        int id;
        std::cout << "Введите имя студента " << i + 1 << ": ";
        std::getline(std::cin, name);
        std::cout << "Введите ID студента " << i + 1 << ": ";
        std::cin >> id;
        students.push_back(Student(name, id));
        std::cin.ignore(); 
    }

    NodeName* nameRoot = nullptr;
    NodeID* idRoot = nullptr;
    for (int i = 0; i < students.size(); ++i) {
        nameRoot = insertName(nameRoot, students[i].name, i);
        idRoot = insertID(idRoot, students[i].id, i);
    }

    std::cout << "\nОтсортированный список студентов по имени:\n";
    inorderTraversalName(nameRoot, students);

    std::cout << "\nОтсортированный список студентов по ID:\n";
    inorderTraversalID(idRoot, students);

    int choice;
    do {
        std::cout << "\nВыберите действие:\n";
        std::cout << "1 - Найти студента по имени\n";
        std::cout << "2 - Найти студента по ID\n";
        std::cout << "3 - Завершить программу\n";
        std::cout << "4 - Изменить запись\n";
        std::cout << "5 - Удалить запись по имени\n";
        std::cout << "6 - Удалить запись по ID\n";
        std::cout << "Ваш выбор: ";
        std::cin >> choice;
        std::cin.ignore(); 

        switch (choice) {
            case 1: {
                std::string nameToSearch;
                std::cout << "Введите имя студента для поиска: ";
                std::getline(std::cin, nameToSearch);
                if (!searchByName(nameRoot, students, nameToSearch)) {
                    std::cout << "Студент с именем \"" << nameToSearch << "\" не найден.\n";
                }
                break;
            }
            case 2: {
                int idToSearch;
                std::cout << "Введите ID студента для поиска: ";
                std::cin >> idToSearch;
                if (!searchByID(idRoot, students, idToSearch)) {
                    std::cout << "Студент с ID " << idToSearch << " не найден.\n";
                }
                break;
            }
            case 3:
                std::cout << "Программа завершена.\n";
                break;
            case 4: { 
                int indexToEdit;
                std::cout << "Введите индекс студента для редактирования (0-" << students.size() -1 << "): ";
                std::cin >> indexToEdit;
                if (indexToEdit >= 0 && indexToEdit < students.size()) {
                    std::string newName;
                    int newID;
                    std::cout << "Введите новое имя: ";
                    std::getline(std::cin >> std::ws, newName); 
                    std::cout << "Введите новый ID: ";
                    std::cin >> newID;

                    nameRoot = removeName(nameRoot, students[indexToEdit].name);
                    idRoot = removeID(idRoot, students[indexToEdit].id);
                    students[indexToEdit].name = newName;
                    students[indexToEdit].id = newID;
                    nameRoot = insertName(nameRoot, newName, indexToEdit);
                    idRoot = insertID(idRoot, newID, indexToEdit);

                    std::cout << "\nОтсортированный список студентов по имени (после редактирования):\n";
                    inorderTraversalName(nameRoot, students);

                    std::cout << "\nОтсортированный список студентов по ID (после редактирования):\n";
                    inorderTraversalID(idRoot, students);

                } else {
                    std::cout << "Неверный индекс.\n";
                }
                break;
            }
            case 5: {
                std::string nameToRemove;
                std::cout << "Введите имя студента для удаления: ";
                std::getline(std::cin, nameToRemove);
                removeRecord(nameRoot, idRoot, students, nameToRemove);
                break;
            }
            case 6: {
                int idToRemove;
                std::cout << "Введите ID студента для удаления: ";
                std::cin >> idToRemove;
                removeRecordById(nameRoot, idRoot, students, idToRemove);
                break;
            }
            default:
                std::cout << "Неверный выбор. Попробуйте ещё раз.\n";
        }
    } while (choice != 3);

    deleteTree(nameRoot);
    deleteTree(idRoot);
    return 0; 
}