// Вам дан класс Document, от которого унаследованы два класса: PlainTextDocument и HTMLDocument.
// В коде также определен тип DocumentCollection И написаны две функции: AddDocument и PrintCollection.
// Вот ссылка на код: https://clck.ru/Ce6rF. Однако этот код не компилируется, а если исправить ошибки
// компиляции, то эти функции почему-то работают совсем не так, как задумано. Вам надо исправить этот
// код. Требования к нему такие:

// 1. Иерархия классов должна сохраниться: PlainTextDocument и HTMLDocument должны быть наследникамии Document.
// 2. Функциия Save в классе Document должна вести себя полиморфно.
// 3. Тип DocumentCollection должен быть вектором (какого-то типа).
// 4. Сигнатуры функций AddDocument и PrintCollection должны сохраниться. Второй параметр
//    функции AddDocument может принимать значения "plain" или "html".
// Утечек памяти быть не должно.
#include <iostream>
#include <string>
#include <vector>
#include <memory>

class Document {
protected:
    const std::string Content;
public:
    Document(const std::string& s): Content(s) {}
    virtual void Save() const {
    }
    virtual ~Document() = default;
};

class PlainTextDocument: public Document {
public:
    PlainTextDocument(const std::string& s): Document(s) {}
    void Save() const override {
        std::cout << Content << "\n";
    }
};

class HTMLDocument: public Document {
public:
    HTMLDocument(const std::string& s): Document(s) {}
    void Save() const override {
        std::cout << "<HTML><BODY>" << Content << "</BODY></HTML>\n";
    }
};

using DocumentCollection = std::vector<std::unique_ptr<Document>>;

void AddDocument(
    const std::string& content, const std::string& type,
    DocumentCollection& collection
) {
    if (type == "plain")
        collection.emplace_back(std::make_unique<PlainTextDocument>(content));
    else if (type == "html")
        collection.emplace_back(std::make_unique<HTMLDocument>(content));
}

void PrintCollection(const DocumentCollection& collection) {
    for (const auto& doc : collection)
        doc->Save();
}
