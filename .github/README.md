# msserialization

English | [简体中文](README_CN.md)

A lightweight, header-only C++ serialization framework inspired by Boost.Serialization, but more concise and easier to use.

## Features

- 🚀 **Lightweight**: Pure header file implementation, no compilation required, ready to use
- 🎯 **Non-intrusive**: Existing data structures require no modifications
- 🔄 **Bidirectional compatibility**: Serialization and deserialization use the same code
- 📦 **Forward/backward compatible**: Adding or removing data members does not affect other members
- 🔀 **Automatic type conversion**: Supports automatic conversion between JSON strings and C++ numeric types
- 🏗️ **Comprehensive support**:
  - Basic types (bool, numbers, strings, etc.)
  - STL containers (vector, map, set, etc.)
  - Nested custom types
  - Raw pointers and smart pointers
  - Polymorphic types (base class pointers to derived class objects)
- 🔌 **Format-agnostic**: Supports multiple serialization format backends
- 🛠️ **Easy to use**: Clean API, intuitive macro definitions

## Quick Start

### 1. Integrate into Your Project

Integrate into your project via CMake:

```cmake
add_subdirectory(msserialization)
target_link_libraries(${PROJECT_NAME} msserialization)
```

### 2. Define Serializable Types

```cpp
#include "msserialize/siexse.hpp"

// Define your data type
class Person {
public:
    std::string name;
    int age = 0;
    double height = 0.0;
    std::vector<std::string> hobbies;
};

// Register serializable members with macro
SiExSe(Person, name, age, height, hobbies)
```

### 3. Serialize/Deserialize

#### Using JSON (RapidJSON backend)

```cpp
#include "msadapter/rajson.hpp"

void example_json() {
    Person person;
    person.name = "Alice";
    person.age = 30;
    person.height = 1.65;
    person.hobbies = {"reading", "swimming"};
    
    // Serialize to JSON string
    std::string json_str = MSRPC::ToJsonS(person);
    // json_str: {"name":"Alice","age":30,"height":1.65,"hobbies":["reading","swimming"]}
    
    // Deserialize from JSON string
    Person person2;
    MSRPC::FromJsonS(person2, json_str);
    
    // File operations
    MSRPC::ToJsonF(person, "person.json");
    MSRPC::FromJsonF(person2, "person.json");
}
```

#### Using YAML (yaml-cpp backend)

```cpp
#include "msadapter/cppyaml.hpp"

void example_yaml() {
    Person person;
    // ... initialize data
    
    // Serialize to YAML string
    std::string yaml_str = MSRPC::ToYamlS(person);
    
    // Deserialize from YAML string
    Person person2;
    MSRPC::FromYamlS(person2, yaml_str);
    
    // File operations
    MSRPC::ToYamlF(person, "person.yaml");
    MSRPC::FromYamlF(person2, "person.yaml");
}
```

## Advanced Features

### Polymorphic Type Support

The framework supports serialization of base class pointers pointing to derived class objects:

```cpp
#include "msserialize/siexse.hpp"

// Base class
class Shape {
public:
    virtual ~Shape() {}
    virtual std::string type() const = 0;
    virtual double area() const = 0;
    
    std::string color = "black";
};

SiExSe(Shape, color);

// Derived class: Circle
class Circle : public Shape {
public:
    std::string type() const override { return "circle"; }
    double area() const override { return 3.14159 * radius * radius; }
    
    double radius = 0.0;
    double x = 0.0, y = 0.0;
};

SiExSeInhe(Circle, Shape, radius, x, y);

// Derived class: Rectangle
class Rectangle : public Shape {
public:
    std::string type() const override { return "rectangle"; }
    double area() const override { return width * height; }
    
    double width = 0.0, height = 0.0;
};

SiExSeInhe(Rectangle, Shape, width, height);

// Register polymorphic type mapping
BeginBaExSe(Shape)
    static std::map<std::string, ShapeConvT> map = {
        {"circle", ShapeConv<Ar, Circle>},
        {"rectangle", ShapeConv<Ar, Rectangle>},
    };
    std::string type = tValue ? tValue->type() : "";
    ar.io("type", type);
EndBaExSe(map, type)
```

Usage example:

```cpp
#include "msadapter/rajson.hpp"
#include <memory>

void example_polymorphic() {
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>());
    shapes.push_back(std::make_shared<Rectangle>());
    
    // Serialize container with polymorphic types
    std::string json = MSRPC::ToJsonS(shapes);
    
    // Deserialize
    std::vector<std::shared_ptr<Shape>> shapes2;
    MSRPC::FromJsonS(shapes2, json);
}
```

### Automatic Type Conversion

The framework automatically handles conversion between JSON strings and C++ numeric types:

```cpp
struct Config {
    int port;           // JSON string "8080" automatically converted to int
    double timeout;     // JSON string "5.5" automatically converted to double
    std::string id;     // JSON number 123 automatically converted to string
};

SiExSe(Config, port, timeout, id);

void example_conversion() {
    std::string json = R"({"port":"8080","timeout":"5.5","id":123})";
    Config config;
    MSRPC::FromJsonS(config, json);
    // config.port = 8080 (int)
    // config.timeout = 5.5 (double)
    // config.id = "123" (string)
}
```

## API Reference

### Core Macros

| Macro | Description |
|-------|-------------|
| `SiExSe(Type, members...)` | Register serializable members of a type |
| `SiExSeInhe(Type, Base, members...)` | Register serializable members of a derived type |
| `BeginBaExSe(BaseType)`<br>`EndBaExSe(map, type_field)` | Define type mapping for polymorphic types |

### Serialization Functions

```cpp
// Serialize to string
template<typename T>
std::string ToJsonS(const T& obj);

template<typename T>
std::string ToYamlS(const T& obj);

// Deserialize from string
template<typename T>
void FromJsonS(T& obj, const std::string& str);

template<typename T>
void FromYamlS(T& obj, const std::string& str);

// File operations
template<typename T>
void ToJsonF(const T& obj, const std::string& filename);

template<typename T>
void FromJsonF(T& obj, const std::string& filename);
```

### Supported Format Backends

| Format | Adapter Header | Dependency Library |
|--------|----------------|--------------------|
| JSON | `msadapter/rajson.hpp` | RapidJSON |
| JSON | `msadapter/nljson.hpp` | nlohmann-json |
| JSON | `msadapter/qjson.hpp` | Qt QJson |
| YAML | `msadapter/cppyaml.hpp` | yaml-cpp |

## License

MIT License

## Contributing

Issues and Pull Requests are welcome!