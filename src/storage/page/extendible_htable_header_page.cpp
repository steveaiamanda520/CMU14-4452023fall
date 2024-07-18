//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// extendible_htable_header_page.cpp
//
// Identification: src/storage/page/extendible_htable_header_page.cpp
//
// Copyright (c) 2015-2023, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "storage/page/extendible_htable_header_page.h"

#include "common/exception.h"

namespace bustub {

void ExtendibleHTableHeaderPage::Init(uint32_t max_depth) {
  this->max_depth_=max_depth;
  uint32_t size=MaxSize();
  for(uint32_t i=0;i<size;++i){
    directory_page_ids_[i]=INVALID_PAGE_ID;
  }


  // throw NotImplementedException("ExtendibleHTableHeaderPage is not implemented");
}

auto ExtendibleHTableHeaderPage::HashToDirectoryIndex(uint32_t hash) const -> uint32_t { 
  if(this->max_depth_==0){
    return 0;
  }
  return hash>>(sizeof(uint32_t)*8-this->max_depth_); }

auto ExtendibleHTableHeaderPage::GetDirectoryPageId(uint32_t directory_idx) const -> uint32_t { 
  if(directory_idx>=this->MaxSize()){
    return INVALID_PAGE_ID;
  }
  
  return this->directory_page_ids_[directory_idx]; }

void ExtendibleHTableHeaderPage::SetDirectoryPageId(uint32_t directory_idx, page_id_t directory_page_id) {
  if(directory_idx>=this->MaxSize()){
    return;
  }
  directory_page_ids_[directory_idx]=directory_page_id;
  // throw NotImplementedException("ExtendibleHTableHeaderPage is not implemented");
}

auto ExtendibleHTableHeaderPage::MaxSize() const -> uint32_t { 
  return 1<<this->max_depth_; }

}  // namespace bustub
