#ifndef __ARRAY_HPP__
#define __ARRAY_HPP__

#include <vector>
#include <memory>
#include "Partition.hpp"

/*
 * Array:
 *  buffer:   data
 *  PartitionPtr:   partition information
 *  BoxPtr:   Array can be a reference one 
 */
class Array;
typedef shared_ptr<Array> ArrayPtr;

class Array {
  private:
  void *m_buffer;
  bool m_is_field = false;
  int m_grid_pos = -1;
  int m_data_type = 2;
  PartitionPtr m_par_ptr;
  BoxPtr m_ref_box_ptr;
  Box m_corners;
  bool m_is_scalar = false;
  bool m_is_seq = false;
  size_t m_hash;

  public:
  Array(const PartitionPtr &ptr, int data_type = DATA_DOUBLE); 
  ~Array();
  int get_data_type() const;
  void* get_buffer();
  void set_buffer(void *buffer, int size);
  PartitionPtr get_partition() const;
  void display(const char *prefix = "");
  void set_corners();
  Box get_corners();
  Shape local_shape();
  int local_size();
  Shape shape();
  int size();
  int rank();
  bool is_scalar();
  void set_hash(const size_t &hash);
  size_t get_hash() const;
};

#endif
