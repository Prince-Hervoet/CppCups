#pragma once

#include <map>
#include <set>
#include <string>
#include <vector>

class Module {
 public:
  Module() {}
  Module(std::string& name, std::vector<std::string>& deps)
      : name(name), deps(deps) {}
  virtual void execute() = 0;
  const std::string& GetName() const { return name; }
  const std::vector<std::string>& GetDeps() const { return deps; }

 private:
  std::string name;
  std::vector<std::string> deps;
};

class Executor {
 public:
  void AddModule(Module* module) {
    if (!module) return;
    name_to_module.insert(
        std::pair<std::string, Module*>(module->GetName(), module));
  }

  void ExecuteAll() {
    if (name_to_module.size() == 0) return;
    std::set<std::string> flags;
    for (auto it = name_to_module.begin(); it != name_to_module.end(); ++it) {
      if (flags.count(it->first) > 0) continue;
      execute(it->second, flags);
    }
  }

  size_t GetSize() { return name_to_module.size(); }

 private:
  std::map<std::string, Module*> name_to_module;
  void execute(Module* module, std::set<std::string>& flags) {
    flags.insert(module->GetName());
    auto deps = module->GetDeps();
    for (int i = 0; i < deps.size(); ++i) {
      if (flags.count(deps[i]) > 0) continue;
      execute(name_to_module[deps[i]], flags);
    }
    module->execute();
  }
};