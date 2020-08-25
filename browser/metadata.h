#ifndef METADATA_H
#define METADATA_H

#include <map>

class MetaData {
 private:
  unsigned refcnt;
  std::map<std::string,std::string> strings;
  std::map<std::string,double> reals;
  std::map<std::string,int64_t> integers;
  std::map<std::string,bool> booleans;
 public:
  MetaData() { refcnt = 1; }

  MetaData(const MetaData&) = delete;
  void operator=(const MetaData&) = delete;

  void incref() {
    refcnt++;
    //printf("meta %08x inc: %d\n", (int)(uint64_t)this, refcnt);
  }

  void decref() {
    refcnt--;
    //printf("meta %08x dec: %d\n", (int)(uint64_t)this, refcnt);
    if (refcnt == 0)
      delete this;
  }

  unsigned RefCnt() const { return refcnt; }

  void clear() {
    strings.clear();
    reals.clear();
    integers.clear();
    booleans.clear();
  }

  MetaData *dup() const {
    MetaData *copy = new MetaData();
    copy->merge(this);
    return copy;
  }

  void merge(const MetaData *other) {
    for (const auto & kv : other->strings)
      set(kv.first, kv.second);
    for (const auto & kv : other->reals)
      set(kv.first, kv.second);
    for (const auto & kv : other->integers)
      set(kv.first, kv.second);
    for (const auto & kv : other->booleans)
      set(kv.first, kv.second);
  }

  void copyFrom(const MetaData *other, std::string key) {
    auto si = other->strings.find(key);
    if (si != other->strings.end())
      set(key, si->second);
    auto ri = other->reals.find(key);
    if (ri != other->reals.end())
      set(key, ri->second);
    auto ii = other->integers.find(key);
    if (ii != other->integers.end())
      set(key, ii->second);
    auto bi = other->booleans.find(key);
    if (bi != other->booleans.end())
      set(key, bi->second);
  }

  const std::map<std::string,std::string>& Strings() const { return strings; }
  const std::map<std::string,double>& Reals() const { return reals; }
  const std::map<std::string,int64_t>& Integers() const { return integers; }
  const std::map<std::string,bool>& Booleans() const { return booleans; }

  void set(const std::string key, const char *val) { strings[key] = std::string(val); }
  void set(const std::string key, const std::string val) { strings[key] = val; }
  void set(const std::string key, double val) { reals[key] = val; }
  void set(const std::string key, int64_t val) { integers[key] = val; }
  void set(const std::string key, bool val) { booleans[key] = val; }

  void del(std::string key) {
    auto si = strings.find(key);
    if (si != strings.end()) strings.erase(si);
    auto ri = reals.find(key);
    if (ri != reals.end()) reals.erase(ri);
    auto ii = integers.find(key);
    if (ii != integers.end()) integers.erase(ii);
    auto bi = booleans.find(key);
    if (bi != booleans.end()) booleans.erase(bi);
  }

  bool has(std::string key) const {
    if (strings.find(key) != strings.end()) return true;
    if (reals.find(key) != reals.end()) return true;
    if (integers.find(key) != integers.end()) return true;
    if (booleans.find(key) != booleans.end()) return true;
    return false;
  }

  bool get(std::string key, std::string& out) const {
    auto iter = strings.find(key);
    if (iter != strings.end()) {
      out = iter->second;
      return true;
    }
    return false;
  }

  bool get(std::string key, double& out) const {
    auto iter = reals.find(key);
    if (iter != reals.end()) {
      out = iter->second;
      return true;
    }
    return false;
  }

  bool get(std::string key, int64_t& out) const {
    auto iter = integers.find(key);
    if (iter != integers.end()) {
      out = iter->second;
      return true;
    }
    return false;
  }

  bool get(std::string key, bool& out) const {
    auto iter = booleans.find(key);
    if (iter != booleans.end()) {
      out = iter->second;
      return true;
    }
    return false;
  }

  void dump() const {
    for (const auto & kv : strings)
      printf("%s = %s\n", kv.first.c_str(), kv.second.c_str());
  }
};

class MetaDataRef {
 private:
  MetaData *data;
 public:
  MetaDataRef() { data = NULL; }
  MetaDataRef(MetaData *meta) {
    if (meta != NULL) meta->incref();
    data = meta;
  }
  MetaDataRef(const MetaDataRef& other) {
    if (other.data != NULL) other.data->incref();
    data = other.data;
  }
  MetaDataRef(const std::string key, const std::string val) {
    data = new MetaData();
    data->set(key, val);
  }
  MetaDataRef(const std::string key, int64_t val) {
    data = new MetaData();
    data->set(key, val);
  }
  ~MetaDataRef() { clear(); }

  MetaDataRef& operator=(const MetaDataRef& other) {
    // in case they point to the same thing inc before dec
    if (other.data != NULL) other.data->incref();
    clear();
    data = other.data;
    return *this;
  }

  MetaData *Ptr() const { return data; }

  void del(std::string key) { if (data != NULL) data->del(key); }

  bool has(std::string key) const { return (data != NULL) && data->has(key); }

  bool get(std::string key, std::string& val) const { return (data!=NULL) ? data->get(key, val) : false; }
  bool get(std::string key, double& val) const { return (data!=NULL) ? data->get(key, val) : false; }
  bool get(std::string key, int64_t& val) const { return (data!=NULL) ? data->get(key, val) : false; }
  bool get(std::string key, bool& val) const { return (data!=NULL) ?  data->get(key, val) : false; }

  void alloc() {
    if (data == NULL)
      data = new MetaData();
  }

  void clear() {
    if (data != NULL) data->decref();
    data = NULL;
  }

  void destructiveMerge(const MetaDataRef other) {
    if (other.data != NULL) {
      if (data != NULL) {
	data->merge(other.data);
      } else {
	data = other.data;
	data->incref();
      }
    }
  }

  void copyOnWriteMerge(const MetaDataRef other) {
    if (other.data != NULL) {
      if (data != NULL) {
	auto copy = data->dup();
	data->decref();
	data = copy;
	data->merge(other.data);
      } else {
	data = other.data;
	data->incref();
      }
    }
  }

  MetaDataRef dup() {
    if (data != NULL) {
      MetaData *copy = data->dup();
      MetaDataRef ref(copy);
      copy->decref();
      return ref;
    }
    return *this;
  }
};

template <int MX> class MetaStack {
 private:
  MetaData *stack[MX];
  unsigned ptr;
 public:
  MetaStack() { ptr = 0; }
  ~MetaStack() { while (ptr > 0) pop(); }

  void push(MetaData *meta) {
    if (ptr < MX) {
      stack[ptr] = meta;
      meta->incref();
    }
    ptr++;
  }

  void pop() {
    ptr--;
    if (ptr < MX && stack[ptr] != NULL) {
      stack[ptr]->decref();
      stack[ptr] = NULL;
    }
  }

  unsigned Count() const { return (ptr <= MX) ? ptr : MX; }

  const MetaData * operator[](unsigned idx) const {
    return stack[idx];
  }

  MetaDataRef merged() {
    MetaDataRef ref;
    for (unsigned i = 0; i < ptr; i++)
      ref.copyOnWriteMerge(stack[i]);
    return ref;
  }
};

#endif
