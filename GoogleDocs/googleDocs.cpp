#include <bits/stdc++.h>
using namespace std;

class DocumentElement
{
public:
  virtual string render() = 0;
};

class TextElement : public DocumentElement
{
private:
  string text;

public:
  TextElement(const string &text) { this->text = text; }
  string render() override
  {
    return text;
  }
};

class ImageElement : public DocumentElement
{
private:
  string imagePath;

public:
  ImageElement(const string &path) { this->imagePath = path; }
  string render() override
  {
    return imagePath;
  }
};

class TabSpaceElement : public DocumentElement
{
public:
  string render() override
  {
    return "\t";
  }
};

class newLineElement : public DocumentElement
{
public:
  string render() override
  {
    return "\n";
  }
};

class Document
{
private:
  vector<DocumentElement *> elements;

public:
  void addElement(DocumentElement *element)
  {
    elements.push_back(element);
  }

  string render()
  {
    string result;
    for (auto element : elements)
    {
      result += element->render();
    }
    return result;
  }
};

class presistence
{
public:
  virtual void save(string data) = 0;
};

class saveToFile : public presistence
{
public:
  void save(string data) override
  {
    ofstream outFile("document.txt");
    if (outFile.is_open())
    {
      outFile << data;
      outFile.close();
    }
    else
    {
      cout << "Unable to open file for writing." << endl;
    }
  }
};

class saveToDB : public presistence
{
public:
  void save(string data) override
  {
    // Simulate saving to a database
    cout << "Saving to database: " << data << endl;
  }
};

class DocumentEditor
{
private:
  Document *document;
  presistence *storage;
  string renderedDocument;

public:
  DocumentEditor(Document *document, presistence *storage)
  {
    this->document = document;
    this->storage = storage;
  }

  void addText(const string &text)
  {
    document->addElement(new TextElement(text));
  }

  void addImage(const string &path)
  {
    document->addElement(new ImageElement(path));
  }

  void addTabSpace()
  {
    document->addElement(new TabSpaceElement());
  }

  void addNewLine()
  {
    document->addElement(new newLineElement());
  }

  string renderDocument()
  {
    if (renderedDocument.empty())
    {
      renderedDocument = document->render();
    }
    return renderedDocument;
  };

  void saveDocument()
  {
    storage->save(renderedDocument);
  }
};

int main()
{
  Document *doc = new Document();
  presistence *fileStorage = new saveToFile();
  DocumentEditor editor(doc, fileStorage);

  editor.addText("Hello, World!");
  editor.addNewLine();
  editor.addText("This is a simple document editor.");
  editor.addTabSpace();
  editor.addText("It supports text and images.");
  editor.addNewLine();
  editor.addImage("path/to/image.jpg");

  cout << "Rendered Document:\n"
       << editor.renderDocument() << endl;
  editor.saveDocument();
  delete doc;
  delete fileStorage;
  return 0;
};
