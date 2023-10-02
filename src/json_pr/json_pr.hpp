#pragma once

#include <map>
#include <string>
#include <vector>

namespace letMeSee {
class JsonPr {
 public:
  enum JsonType { jnull, jbool, jint, jdouble, jstring, jarray, jobject };

  JsonPr() { type = jnull; }

  JsonPr(bool value) {
    type = jbool;
    data.valueBool = value;
  }

  JsonPr(int value) {
    type = jint;
    data.valueInt = value;
  }

  JsonPr(double value) {
    type = jdouble;
    data.valueDouble = value;
  }

  JsonPr(std::string& value) {
    type = jstring;
    data.valueString = new std::string(value);
  }

  JsonPr(const JsonPr& other) {
    if (this == &other) return;
    type = other.type;
    if (type == jarray) {
      data.valueArray = new std::vector<JsonPr>(*(other.data.valueArray));
    }
  }

  JsonPr(JsonPr&& other) {
    if (this == &other) return;
    type = other.type;
    data = other.data;
    other.type = jnull;
    memset(&(other.data), 0, sizeof(other.data));
  }

  ~JsonPr() {
    if (type == jarray) {
      delete data.valueArray;
    } else if (type == jobject) {
      delete data.valueObject;
    } else if (type == jstring) {
      delete data.valueString;
    }
    memset(&data, 0, sizeof(data));
  }

  void Append(const JsonPr& value) {
    if (type != jnull && type != jarray) return;
    if (type == jnull) {
      type = jarray;
      data.valueArray = new std::vector<JsonPr>();
    }
    data.valueArray->emplace_back(value);
  }

  void Put(std::string& key, const JsonPr& value) {
    if (type != jnull && type != jobject) return;
    if (type == jnull) {
      type = jobject;
      data.valueObject = new std::map<std::string, JsonPr>();
    }
    data.valueObject->insert(std::pair<std::string, JsonPr>(key, value));
  }

 private:
  typedef union JsonData {
    bool valueBool;
    int valueInt;
    double valueDouble;
    std::string* valueString;
    std::vector<JsonPr>* valueArray;
    std::map<std::string, JsonPr>* valueObject;
  } JsonData;

  JsonData data;
  JsonType type;
};
}  // namespace letMeSee