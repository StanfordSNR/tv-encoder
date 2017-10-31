#include <iostream>

#include "tfhd_box.hh"

using namespace std;
using namespace MP4;

TfhdBox::TfhdBox(const uint64_t size, const string & type)
  : FullBox(size, type), track_id_()
{}

TfhdBox::TfhdBox(const string & type,
                 const uint8_t version,
                 const uint32_t flags,
                 const uint32_t track_id,
                 const uint64_t base_data_offset,
                 const uint32_t sample_description_index,
                 const uint32_t default_sample_duration,
                 const uint32_t default_sample_size,
                 const uint32_t default_sample_flags)
  : FullBox(type, version, flags), track_id_(track_id)
{
  uint64_t size = 16; /* FullBox header + track_id */

  if (flags & base_data_offset_present) {
    base_data_offset_ = base_data_offset;
    size += 8;
  }
  if (flags & sample_description_index_present) {
    sample_description_index_ = sample_description_index;
    size += 4;
  }
  if (flags & default_sample_duration_present) {
    default_sample_duration_ = default_sample_duration;
    size += 4;
  }
  if (flags & default_sample_size_present) {
    default_sample_size_ = default_sample_size;
    size += 4;
  }
  if (flags & default_sample_flags_present) {
    default_sample_flags_ = default_sample_flags;
    size += 4;
  }

  set_size(size);
}


void TfhdBox::print_structure(const unsigned int indent)
{
  print_size_type(indent);

  string indent_str = string(indent + 2, ' ') + "| ";
  cout << indent_str << "track id " << track_id_ << endl;
  cout << indent_str << "sample default duration "
       << default_sample_duration_ << endl;
}

void TfhdBox::parse_data(MP4File & mp4, const uint64_t data_size)
{
  uint64_t init_offset = mp4.curr_offset();

  parse_version_flags(mp4);

  track_id_ = mp4.read_uint32();

  if (flags() & base_data_offset_present) {
    base_data_offset_ = mp4.read_uint64();
  }
  if (flags() & sample_description_index_present) {
    sample_description_index_ = mp4.read_uint32();
  }
  if (flags() & default_sample_duration_present) {
    default_sample_duration_ = mp4.read_uint32();
  }
  if (flags() & default_sample_size_present) {
    default_sample_size_ = mp4.read_uint32();
  }
  if (flags() & default_sample_flags_present) {
    default_sample_flags_ = mp4.read_uint32();
  }

  check_data_left(mp4, data_size, init_offset);
}

void TfhdBox::write_box(MP4File & mp4)
{
  write_size_type(mp4);
  write_version_flags(mp4);

  mp4.write_uint32(track_id_);

  if (flags() & base_data_offset_present) {
    mp4.write_uint64(base_data_offset_);
  }
  if (flags() & sample_description_index_present) {
    mp4.write_uint32(sample_description_index_);
  }
  if (flags() & default_sample_duration_present) {
    mp4.write_uint32(default_sample_duration_ );
  }
  if (flags() & default_sample_size_present) {
    mp4.write_uint32(default_sample_size_ );
  }
  if (flags() & default_sample_flags_present) {
    mp4.write_uint32(default_sample_flags_ );
  }
}
