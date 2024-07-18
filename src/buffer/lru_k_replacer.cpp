//===----------------------------------------------------------------------===//
//
//                         BusTub
//
// lru_k_replacer.cpp
//
// Identification: src/buffer/lru_k_replacer.cpp
//
// Copyright (c) 2015-2022, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#include "buffer/lru_k_replacer.h"
#include "common/exception.h"

namespace bustub {

LRUKReplacer::LRUKReplacer(size_t num_frames, size_t k) : replacer_size_(num_frames), k_(k) {}
//意思是说，优先删除   <k的这些
auto LRUKReplacer::Evict(frame_id_t *frame_id) -> bool { 
    std::scoped_lock scope_latch(latch_);
    if(curr_size_==0){
        return false;
    }
    size_t time_gap=0;
    size_t time_stamp=SIZE_MAX;
    *frame_id = INT32_MIN;
    for(auto &i:node_store_){
        if(i.second.is_evictable_==true){
            if(i.second.history_.size()<k_){
                time_gap=SIZE_MAX;
                if(i.second.history_.back()<time_stamp){
                    time_stamp=i.second.history_.back();
                    *frame_id=i.first;
                }
            }
            else{
            if(current_timestamp_-i.second.history_.back()>time_gap){
                time_gap=current_timestamp_-i.second.history_.back();
                *frame_id=i.second.fid_;
            }
            }
        }
    }
    if(*frame_id==INT32_MIN)
        return false;
    node_store_[*frame_id].history_.clear();
    node_store_.erase(*frame_id);
    curr_size_--;

    return true; }

void LRUKReplacer::RecordAccess(frame_id_t frame_id, [[maybe_unused]] AccessType access_type) {
    std::lock_guard locker(latch_);
    current_timestamp_++;
    if(node_store_.find(frame_id)!=node_store_.end()){
        node_store_[frame_id].history_.push_front(current_timestamp_);
        if(node_store_[frame_id].history_.size()>k_){
            node_store_[frame_id].history_.pop_back();
        }
    }else{
        // node_store_[frame_id]=LRUKNode({current_timestamp_},k_,frame_id,false);
        node_store_[frame_id] = LRUKNode{{current_timestamp_}, k_, frame_id, false};
    }
}

void LRUKReplacer::SetEvictable(frame_id_t frame_id, bool set_evictable) {
    std::scoped_lock scoped_latch(latch_);
    if(frame_id > static_cast<int32_t>(replacer_size_)){
        throw ExecutionException("frame id is not valid.");
    }
    if(node_store_.find(frame_id)!=node_store_.end()){
        if(node_store_[frame_id].is_evictable_ && !set_evictable){
            node_store_[frame_id].is_evictable_=set_evictable;
            curr_size_--;
        }else if(!node_store_[frame_id].is_evictable_ && set_evictable){
            node_store_[frame_id].is_evictable_=set_evictable;
            curr_size_++;
        }
    }else{
        latch_.unlock();
        throw ExecutionException("frame id is not exist.");
    }
    latch_.unlock();
}

void LRUKReplacer::Remove(frame_id_t frame_id) {
    std::scoped_lock scoped_latch(latch_);
    if(node_store_.find(frame_id)!=node_store_.end()){
        // set_evictable=false;
        if(!node_store_[frame_id].is_evictable_){
            latch_.unlock();
            throw ExecutionException("frame is not evictable.");
        }
        node_store_.erase(frame_id);
        curr_size_--;
    } 
}

auto LRUKReplacer::Size() -> size_t { 
    std::lock_guard locker(latch_);
    return curr_size_; }

}  // namespace bustub
