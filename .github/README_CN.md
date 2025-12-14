# msserialization序列化框架

[English](README.md) | 简体中文

一个轻量级、纯头文件的C++序列化框架，灵感来自Boost.Serialization，但更加简洁易用。

## 特性

- 🚀 **轻量级**：纯头文件实现，无需编译，开箱即用
- 🎯 **非侵入式**：现有数据结构无需任何修改
- 🔄 **双向兼容**：序列化和反序列化使用同一套代码
- 📦 **向前/向后兼容**：数据结构成员增删不影响已有数据
- 🔀 **自动类型转换**：支持JSON字符串与C++数值类型的自动转换
- 🏗️ **全面支持**：
  - 基本类型（bool、数字、字符串等）
  - STL容器（vector、map、set等）
  - 嵌套自定义类型
  - 原生指针和智能指针
  - 多态类型（基类指针指向派生类对象）
- 🔌 **格式无关**：支持多种序列化格式后端
- 🛠️ **简单易用**：简洁的API，直观的宏定义

## 快速开始

### 1. 集成到项目

通过CMake集成到你的项目：

```cmake
add_subdirectory(msserialization)
target_link_libraries(${PROJECT_NAME} msserialization)
```

### 2. 定义可序列化类型

```cpp
#include "msserialize/siexse.hpp"

// 定义你的数据类型
class Person {
public:
    std::string name;
    int age = 0;
    double height = 0.0;
    std::vector<std::string> hobbies;
};

// 使用宏注册可序列化成员
SiExSe(Person, name, age, height, hobbies)
```

### 3. 序列化/反序列化

#### 使用JSON (RapidJSON后端)

```cpp
#include "msadapter/rajson.hpp"

void example_json() {
    Person person;
    person.name = "Alice";
    person.age = 30;
    person.height = 1.65;
    person.hobbies = {"reading", "swimming"};
    
    // 序列化为JSON字符串
    std::string json_str = MSRPC::ToJsonS(person);
    // json_str: {"name":"Alice","age":30,"height":1.65,"hobbies":["reading","swimming"]}
    
    // 从JSON字符串反序列化
    Person person2;
    MSRPC::FromJsonS(person2, json_str);
    
    // 文件操作
    MSRPC::ToJsonF(person, "person.json");
    MSRPC::FromJsonF(person2, "person.json");
}
```

#### 使用YAML (yaml-cpp后端)

```cpp
#include "msadapter/cppyaml.hpp"

void example_yaml() {
    Person person;
    // ... 初始化数据
    
    // 序列化为YAML字符串
    std::string yaml_str = MSRPC::ToYamlS(person);
    
    // 从YAML字符串反序列化
    Person person2;
    MSRPC::FromYamlS(person2, yaml_str);
    
    // 文件操作
    MSRPC::ToYamlF(person, "person.yaml");
    MSRPC::FromYamlF(person2, "person.yaml");
}
```

## 高级特性

### 多态类型支持

框架支持基类指针指向派生类对象的序列化：

```cpp
#include "msserialize/siexse.hpp"

// 基类
class Shape {
public:
    virtual ~Shape() {}
    virtual std::string type() const = 0;
    virtual double area() const = 0;
    
    std::string color = "black";
};

SiExSe(Shape, color);

// 派生类：圆形
class Circle : public Shape {
public:
    std::string type() const override { return "circle"; }
    double area() const override { return 3.14159 * radius * radius; }
    
    double radius = 0.0;
    double x = 0.0, y = 0.0;
};

SiExSeInhe(Circle, Shape, radius, x, y);

// 派生类：矩形
class Rectangle : public Shape {
public:
    std::string type() const override { return "rectangle"; }
    double area() const override { return width * height; }
    
    double width = 0.0, height = 0.0;
};

SiExSeInhe(Rectangle, Shape, width, height);

// 注册多态类型映射
BeginBaExSe(Shape)
    static std::map<std::string, ShapeConvT> map = {
        {"circle", ShapeConv<Ar, Circle>},
        {"rectangle", ShapeConv<Ar, Rectangle>},
    };
    std::string type = tValue ? tValue->type() : "";
    ar.io("type", type);
EndBaExSe(map, type)
```

使用示例：

```cpp
#include "msadapter/rajson.hpp"
#include <memory>

void example_polymorphic() {
    std::vector<std::shared_ptr<Shape>> shapes;
    shapes.push_back(std::make_shared<Circle>());
    shapes.push_back(std::make_shared<Rectangle>());
    
    // 序列化包含多态类型的容器
    std::string json = MSRPC::ToJsonS(shapes);
    
    // 反序列化
    std::vector<std::shared_ptr<Shape>> shapes2;
    MSRPC::FromJsonS(shapes2, json);
}
```

### 自动类型转换

框架自动处理JSON字符串与C++数值类型之间的转换：

```cpp
struct Config {
    int port;           // JSON中的字符串"8080"会自动转换为int
    double timeout;     // JSON中的字符串"5.5"会自动转换为double
    std::string id;     // JSON中的数字123会自动转换为字符串
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

## API参考

### 核心宏

| 宏 | 说明 |
|----|------|
| `SiExSe(Type, members...)` | 注册类型的可序列化成员 |
| `SiExSeInhe(Type, Base, members...)` | 注册派生类型的可序列化成员 |
| `BeginBaExSe(BaseType)`<br>`EndBaExSe(map, type_field)` | 定义多态类型的类型映射 |

### 序列化函数

```cpp
// 序列化为字符串
template<typename T>
std::string ToJsonS(const T& obj);

template<typename T>
std::string ToYamlS(const T& obj);

// 从字符串反序列化
template<typename T>
void FromJsonS(T& obj, const std::string& str);

template<typename T>
void FromYamlS(T& obj, const std::string& str);

// 文件操作
template<typename T>
void ToJsonF(const T& obj, const std::string& filename);

template<typename T>
void FromJsonF(T& obj, const std::string& filename);
```

### 支持的格式后端

| 格式 | 适配器头文件 | 依赖库 |
|------|--------------|--------|
| JSON | `msadapter/rajson.hpp` | RapidJSON |
| JSON | `msadapter/nljson.hpp` | nlohmann-json |
| JSON | `msadapter/qjson.hpp` | Qt QJson |
| YAML | `msadapter/cppyaml.hpp` | yaml-cpp |

## 许可证

MIT License

## 贡献

欢迎提交Issue和Pull Request！