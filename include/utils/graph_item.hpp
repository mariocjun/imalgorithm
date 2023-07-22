#ifndef ALGOVI_INCLUDE_UTILS_GRAPHITEM_HPP_
#define ALGOVI_INCLUDE_UTILS_GRAPHITEM_HPP_

template<typename T>
class graph_item {
 public:
  virtual ~graph_item() = default;
  virtual T getData() const = 0;
  virtual void setData(T data) = 0;

 protected:
  explicit graph_item(T data) : data_(data) {}

  T data_;
};

#endif //ALGOVI_INCLUDE_UTILS_GRAPHITEM_HPP_
