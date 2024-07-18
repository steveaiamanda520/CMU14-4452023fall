// #include "primer/trie.h"
// #include <string_view>
// #include "common/exception.h"

// namespace bustub {

// template <class T>
// auto Trie::Get(std::string_view key) const -> const T * {
//   throw NotImplementedException("Trie::Get is not implemented.");

//   // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
//   // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
//   // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
//   // Otherwise, return the value.
// }

// template <class T>
// auto Trie::Put(std::string_view key, T value) const -> Trie {
//   // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
//   throw NotImplementedException("Trie::Put is not implemented.");

//   // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
//   // exists, you should create a new `TrieNodeWithValue`.
// }

// auto Trie::Remove(std::string_view key) const -> Trie {
//   throw NotImplementedException("Trie::Remove is not implemented.");

//   // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
//   // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
// }

// // Below are explicit instantiation of template functions.
// //
// // Generally people would write the implementation of template classes and functions in the header file. However, we
// // separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// // implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// // by the linker.

// template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint32_t *;

// template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint64_t *;

// template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const std::string *;

// // If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

// using Integer = std::unique_ptr<uint32_t>;

// template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const Integer *;

// template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

// }  // namespace bustub
// #include "primer/trie.h"
// #include <cstddef>
// #include <memory>
// #include <string_view>
// #include <vector>
// #include "common/exception.h"

// namespace bustub {

// template <class T>
// auto Trie::Get(std::string_view key) const -> const T * {
//   // throw NotImplementedException("Trie::Get is not implemented.");

//   size_t size = key.size();
//   auto target = this->root_;
//   for (size_t i = 0; i < size; i++) {
//     char k = key[i];
//     if (target && target->children_.find(k) != target->children_.end()) {
//       target = target->children_.at(k);
//     } else {
//       return nullptr;
//     }
//   }

//   //
//   auto res = dynamic_cast<const TrieNodeWithValue<T> *>(target.get());

//   if (res && res->is_value_node_) {
//     return res->value_.get();
//   }
//   return nullptr;

//   // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
//   // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
//   // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
//   // Otherwise, return the value.
// }

// template <class T>
// auto Trie::Put(std::string_view key, T value) const -> Trie {
//   // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
//   // throw NotImplementedException("Trie::Put is not implemented.");

//   size_t size = key.size();
//   auto target = this->root_;
//   auto value_ptr = std::make_shared<T>(std::move(value));

//   if (size == 0) {
//     return Trie(std::make_shared<const TrieNodeWithValue<T>>(target->children_, value_ptr));
//   }

//   auto stack = std::vector<std::shared_ptr<const TrieNode>>(size + 1, nullptr);
//   stack[0] = this->root_;

//   // record the put path
//   for (size_t i = 0; i < size; i++) {
//     char k = key[i];
//     if (target && target->children_.find(k) != target->children_.end()) {
//       target = target->children_.at(k);
//       stack[i + 1] = target;
//     } else {
//       target = nullptr;
//       stack[i + 1] = target;
//     }
//     // handle value node
//     if (i == size - 1) {
//       if (target) {
//         stack[i + 1] = std::make_shared<const TrieNodeWithValue<T>>(target->children_, value_ptr);
//       } else {
//         stack[i + 1] = std::make_shared<const TrieNodeWithValue<T>>(value_ptr);
//       }
//     }
//   }

//   // build node down to top
//   for (int i = static_cast<int>(size) - 1; i >= 0; i--) {
//     char k = key[i];
//     auto target = stack[i];
//     if (target) {
//       stack[i] = stack[i]->Clone();
//       const_cast<TrieNode*>(stack[i].get())->children_[k] = stack[i + 1];
//     } else {
//       std::map<char, std::shared_ptr<const TrieNode>> mp{{k, stack[i + 1]}};
//       stack[i] = std::make_shared<const TrieNode>(mp);
//     }
//   }

//   return Trie(stack[0]);

//   // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
//   // exists, you should create a new `TrieNodeWithValue`.
// }

// auto Trie::Remove(std::string_view key) const -> Trie {
//   // throw NotImplementedException("Trie::Remove is not implemented.");

//   size_t size = key.size();
//   auto target = this->root_;
//   auto stack = std::vector<std::shared_ptr<const TrieNode>>(size + 1, nullptr);
//   stack[0] = this->root_;

//   // record the remove path
//   for (size_t i = 0; i < size; i++) {
//     char k = key[i];
//     if (target && target->children_.find(k) != target->children_.end()) {
//       target = target->children_.at(k);
//       stack[i + 1] = target;
//     } else {
//       return Trie(stack[0]);
//     }
//   }

//   if (!stack.back()->is_value_node_) {
//     return Trie(stack[0]);
//   }

//   // handle last node on delete path
//   stack[stack.size() - 1] = std::make_shared<const TrieNode>(stack.back()->children_);

//   // handle prefix node's children map
//   // handle  from last second node
//   for (int i = static_cast<int>(size) - 1; i >= 0; i--) {
//     char k = key[i];
//     stack[i] = stack[i]->Clone();

//     if (stack[i + 1] && stack[i + 1]->children_.empty() && !stack[i + 1]->is_value_node_) {
//       const_cast<TrieNode *>(stack[i].get())->children_.erase(k);
//     } else {
//       const_cast<TrieNode *>(stack[i].get())->children_[k] = stack[i + 1];
//     }
//   }

//   if (stack[0] && stack[0]->children_.empty() && !stack[0]->is_value_node_) {
//     return Trie(nullptr);
//   }

//   return Trie(stack[0]);

//   // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
//   // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
// }

// // Below are explicit instantiation of template functions.
// //
// // Generally people would write the implementation of template classes and functions in the header file. However, we
// // separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// // implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// // by the linker.

// template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint32_t *;

// template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const uint64_t *;

// template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const std::string *;

// // If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

// using Integer = std::unique_ptr<uint32_t>;

// template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const Integer *;

// template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
// template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

// }  // namespace bustub


#include "primer/trie.h"
#include <string_view>
#include "common/exception.h"

namespace bustub {

template <class T>
auto Trie::Get(std::string_view key) const -> const T * {
    std::shared_ptr<const TrieNode> cur=this->root_;
    
    for(auto ch:key){
      if(!cur){
        return nullptr;
      }
      if(cur->children_.find(ch)!=cur->children_.end()){
        cur=cur->children_.at(ch);
      }else{
        return nullptr;
      }
    }
    
    auto res=dynamic_cast<const TrieNodeWithValue<T> *>(cur.get());
    if(res && res->is_value_node_){
      return res->value_.get();
    }
    return nullptr;

    // auto target=this->root_;
    // size_t size=key.size();
    // for(size_t i=0;i<size;i++){
    //   char k=key[i];
    //   if(target && target->children_.find(k)!=target->children_.end()){
    //     target=target->children_.at(k);
    //   }else{
    //     return nullptr;
    //   }
    // }

    // auto res=dynamic_cast<const TrieNodeWithValue<T> *>(target.get());
    // if(res && res->is_value_node_){
    //   return res->value_.get();
    // }
    // return nullptr;
}

// 初始化：

// 获取要插入的键 key 的长度 size。
// 获取 Trie 的根节点 root_。
// 记录插入路径：

// 使用一个 stack 数组来记录从根节点到要插入位置的节点路径，初始化为 nullptr。
// 从根节点开始，沿着键 key 的字符逐级向下查找，记录每个节点到 stack 中。
// 插入值节点：

// 如果 size == 0，即键为空，则直接返回一个包含新值节点的 Trie。
// 否则，遍历键的字符：
// 如果当前字符在当前节点的子节点中存在，则继续向下移动。
// 如果不存在，则将当前节点设为 nullptr，并记录到 stack 中。
// 对于最后一个字符，如果已经存在节点，则更新为包含新值节点；如果不存在节点，则创建新节点并设置新值节点。
// 从下到上构建节点：

// 从键的最后一个字符向前遍历 stack：
// 如果当前节点存在，则克隆它，更新其子节点映射为新的子节点。
// 如果当前节点不存在，则创建一个新节点，并设置其子节点映射为新的子节点。
// 返回更新后的 Trie 对象。
template <class T>
auto Trie::Put(std::string_view key, T value) const -> Trie {
  // size_t size=key.size();
  // std::shared_ptr<const TrieNode> target=root_;
  // std::shared_ptr<T> value_ptr=std::make_shared<T>(std::move(value));
  // if(size==0){
  //   return Trie(std::make_shared<const TrieNodeWithValue<T>>(target->children_,value_ptr));
  // }
  // auto stack=std::vector<std::shared_ptr<const TireNode>>(size+1,nullptr);
  // stack[0]=this->root_;
  // for(size_t i=0;i<size;i++){
  //   char k=key[i];
  //   if(target && target->children_.find(k)!=target->children_.end()){
  //     target=target->children_.at[k];
  //     stack[i+1]=target;
  //   }else{
  //     target=nullptr;
  //     stack[i+1]=target;
  //   }

  //   if(i==size-1){
  //     if(target){
  //       stack[i+1]=std::make_shared<const TrieNodeWithValue<T>>(target->children_,value_ptr);
  //     }
  //     else{
  //       stack[i+1]=std::make_shared<const TrieNodeWithValue<T>>(value_ptr);
  //     }
  //   }
  // }
  //    for(int i=static_cast<int>(size)-1;i>=0;i--){
  //     char k=key[i];
  //     auto target=stack[i];
  //     if(target){
  //       stack[i]=stack[i]->Clone();
  //       const_cast<TrieNode*>(stack[i].get())->children_[k]=stack[i+1];
  //     }else{
  //       std::map<char,std::shared_ptr<const TrieNode>> mp{{k,stack[i+1]}};
  //       stack[i]=std::make_shared<const TrieNode>(mp);
  //     }
  //   }

  //   return Trie(stack[0]);
  size_t size=key.size();
  auto target=this->root_;
  //设置根的数值
  auto value_ptr=std::make_shared<T>(std::move(value)); //加入是其他类型的对象，可以直接转移全部的资源执行权。

  if(size==0){
    return Trie(std::make_shared<const TrieNodeWithValue<T>>(target->children_,value_ptr));
  }


  auto stack =std::vector<std::shared_ptr<const TrieNode>>(size+1,nullptr);
  stack[0]=this->root_;

  for(size_t i=0;i<size;i++){
    char k=key[i];
    if(target && target->children_.find(k)!=target->children_.end()){
      target=target->children_.at(k);
      stack[i+1]=target;
    }else{
      target=nullptr;
      stack[i+1]=target;
    }

    if(i==size-1){
      if(target){
        //如果存在，则他更新他的val_ptr的值，当然也有他自己的孩子
        stack[i+1]=std::make_shared<const TrieNodeWithValue<T>>(target->children_,value_ptr);
      }else{
        stack[i+1]=std::make_shared<const TrieNodeWithValue<T>>(value_ptr);
      }
    }
  }

    for(int i=static_cast<int>(size)-1;i>=0;i--){
      char k=key[i];
      auto target=stack[i];
      if(target){
        stack[i]=stack[i]->Clone();
        const_cast<TrieNode*>(stack[i].get())->children_[k]=stack[i+1];
      }else{
        std::map<char,std::shared_ptr<const TrieNode>> mp{{k,stack[i+1]}};
        stack[i]=std::make_shared<const TrieNode>(mp);
      }
    }

    return Trie(stack[0]);
  
}


auto Trie::Remove(std::string_view key) const -> Trie {
  size_t size=key.size();
  auto target=this->root_;
  auto stack=std::vector<std::shared_ptr<const TrieNode>>(size+1,nullptr);
  stack[0]=this->root_;
  for(size_t i=0;i<size;i++){
    char k=key[i];
    if(target && target->children_.find(k)!=target->children_.end()){
      target=target->children_.at(k);
      stack[i+1]=target;
      }else{
        return Trie(stack[0]);
      }

  }
  if(!stack.back()->is_value_node_){
    return Trie(stack[0]);
  }

  stack[stack.size()-1]=std::make_shared<const TrieNode>(stack.back()->children_);
  for(int i=static_cast<int>(size)-1;i>=0;i--){
    char k=key[i];
    stack[i]=stack[i]->Clone();
    if(stack[i+1] && stack[i+1]->children_.empty() && !stack[i+1]->is_value_node_){
      const_cast<TrieNode*>(stack[i].get())->children_.erase(k);
    }else{
      const_cast<TrieNode*>(stack[i].get())->children_[k]=stack[i+1];
    }
  }

  if(stack[0] && stack[0]->children_.empty() && !stack[0]->is_value_node_){
    return Trie(nullptr);
  }
  return Trie(stack[0]);
  // throw NotImplementedException("Trie::Get is not implemented.");
  // auto newRoot = std::make_shared<TrieNode>(*root_);

  // // Traverse to the node corresponding to the key
  // auto current = newRoot;
  // std::vector<std::pair<std::shared_ptr<TrieNode>, char>> path;

  // for (char ch : key) {
  //   if (current->children_.find(ch) == current->children_.end()) {
  //     // Key does not exist in the trie, return the original trie
  //     return Trie(root_);
  //   }
  //   path.emplace_back(current, ch);
  //   current = std::static_pointer_cast<TrieNode>(current->children_[ch]);
  // }

  // // Remove the value at the leaf node
  // auto res = std::dynamic_pointer_cast<TrieNodeWithValue<T>>(current);
  // if (res) {
  //   res->value_.reset();
  //   res->is_value_node_ = false;
  // }

  // // Remove empty nodes if necessary
  // for (auto it = path.rbegin(); it != path.rend(); ++it) {
  //   auto node = it->first;
  //   char ch = it->second;
  //   if (node->children_[ch]->children_.empty() && !node->children_[ch]->value_) {
  //     node->children_.erase(ch);
  //   } else {
  //     break;
  //   }
  // }

  // return Trie(std::move(newRoot));

}

// template <class T>
// auto Trie::Get(std::string_view key) const -> const T * {
//   // throw NotImplementedException("Trie::Get is not implemented.");
// // std::shared_ptr<TrieNode> p = std::make_shared<TrieNode>(*root_);
//   auto p=this->root_;
//   // TrieNode* p=root_;
//   for(auto ch:key){
//     if(p->children_.find(ch)==p->children_.end())
//       return nullptr;
//     p=p->children_.at(ch);
//   }

//   auto res=dynamic_cast<const TrieNodeWithValue<T> *>(p.get());
//   if(res && res->is_value_node_){
//     return res->value_.get();
//   }
//   return nullptr;
//   // You should walk through the trie to find the node corresponding to the key. If the node doesn't exist, return
//   // nullptr. After you find the node, you should use `dynamic_cast` to cast it to `const TrieNodeWithValue<T> *`. If
//   // dynamic_cast returns `nullptr`, it means the type of the value is mismatched, and you should return nullptr.
//   // Otherwise, return the value.
// }

// template <class T>
// auto Trie::Put(std::string_view key, T value) const -> Trie {
//   // Note that `T` might be a non-copyable type. Always use `std::move` when creating `shared_ptr` on that value.
//   // throw NotImplementedException("Trie::Put is not implemented.");
//   // std::shared_ptr<TrieNode> p = std::make_shared<TrieNode>(*root_);
//   auto newRoot=std::make_shared<TrieNode>(*root_);
//   auto p=this->root_;
//   // auto value_ptr=std::make_shared<T>(std::move(value));

//   for(auto ch: key){    //遍历key中每一个字符
//     if(p->children_.find(ch)==p->children_.end()){
//       auto newnode = std::make_shared<const TrieNode>();
//       // TrieNode* newnode=new TrieNodeWithValue<T>();
//       p->children_.at(ch)=newnode;
//     }
//     p=p->children_.at(ch);
//   }
//   //[]不支持常量对象的访问
//   //这里自己写的有问题
//   auto res=dynamic_cast<const TrieNodeWithValue<T> *>(p.get());
//   res->value_=std::make_shared<T>(std::move(value));
//   res->is_value_node_=true;
//   return Trie(std::move(newRoot));

//   // You should walk through the trie and create new nodes if necessary. If the node corresponding to the key already
//   // exists, you should create a new `TrieNodeWithValue`.
// }

// auto Trie::Remove(std::string_view key) const -> Trie {
  // std::shared_ptr<TrieNode> p = std::make_shared<TrieNode>(*root_);
  // auto p=this->root_;
  // // TrieNode* p=std::move(root_);
  // int len=key.length();
  // for(int i=0;i<len;i++){
  //   char ch=key[i];
  //   if(p->children_.find(ch)==p->children_.end()){
  //     break;
  //   }
  //   if(i==len-1){
  //     delete p->children_[ch].get();
  //     p->children_.erase(ch);
  //     break;
  //   }
  //   p=p->children_[ch];
    
  // }
  // return *this;
//   std::shared_ptr<TrieNode> newRoot = std::make_shared<TrieNode>(*root_);

//     // Traverse the trie to find the node corresponding to the key
//     std::shared_ptr<TrieNode> current = newRoot;
//     std::vector<std::pair<std::shared_ptr<TrieNode>, char>> path;

//     for (char ch : key) {
//         if (current->children_.find(ch) == current->children_.end()) {
//             // Key does not exist in the trie, return the original trie
//             return Trie(root_);
//         }
//         path.emplace_back(current, ch);
//         current = current->children_[ch];
//     }

//     // Remove the value at the leaf node
//     current->value.reset();

//     // Remove empty nodes if necessary
//     for (auto it = path.rbegin(); it != path.rend(); ++it) {
//         auto node = it->first;
//         char ch = it->second;
//         if (node->children_[ch]->children_.empty() && !node->children_[ch]->value) {
//             node->children_.erase(ch);
//         } else {
//             break;
//         }
//     }

//     return Trie(std::move(newRoot));


//   // throw NotImplementedException("Trie::Remove is not implemented.");

//   // You should walk through the trie and remove nodes if necessary. If the node doesn't contain a value any more,
//   // you should convert it to `TrieNode`. If a node doesn't have children any more, you should remove it.
// }

// Below are explicit instantiation of template functions.
//
// Generally people would write the implementation of template classes and functions in the header file. However, we
// separate the implementation into a .cpp file to make things clearer. In order to make the compiler know the
// implementation of the template functions, we need to explicitly instantiate them here, so that they can be picked up
// by the linker.

template auto Trie::Put(std::string_view key, uint32_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint32_t *;

template auto Trie::Put(std::string_view key, uint64_t value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const uint64_t *;

template auto Trie::Put(std::string_view key, std::string value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const std::string *;

// If your solution cannot compile for non-copy tests, you can remove the below lines to get partial score.

using Integer = std::unique_ptr<uint32_t>;

template auto Trie::Put(std::string_view key, Integer value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const Integer *;

template auto Trie::Put(std::string_view key, MoveBlocked value) const -> Trie;
template auto Trie::Get(std::string_view key) const -> const MoveBlocked *;

}  // namespace bustub
