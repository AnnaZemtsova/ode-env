// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: common.proto

#ifndef PROTOBUF_common_2eproto__INCLUDED
#define PROTOBUF_common_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
#include <google/protobuf/unknown_field_set.h>
// @@protoc_insertion_point(includes)

namespace pb {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_common_2eproto();
void protobuf_AssignDesc_common_2eproto();
void protobuf_ShutdownFile_common_2eproto();

class EXPetscSolverConfig;

// ===================================================================

class EXPetscSolverConfig : public ::google::protobuf::Message {
 public:
  EXPetscSolverConfig();
  virtual ~EXPetscSolverConfig();

  EXPetscSolverConfig(const EXPetscSolverConfig& from);

  inline EXPetscSolverConfig& operator=(const EXPetscSolverConfig& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::google::protobuf::UnknownFieldSet& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::google::protobuf::UnknownFieldSet* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const ::google::protobuf::Descriptor* descriptor();
  static const EXPetscSolverConfig& default_instance();

  void Swap(EXPetscSolverConfig* other);

  // implements Message ----------------------------------------------

  EXPetscSolverConfig* New() const;
  void CopyFrom(const ::google::protobuf::Message& from);
  void MergeFrom(const ::google::protobuf::Message& from);
  void CopyFrom(const EXPetscSolverConfig& from);
  void MergeFrom(const EXPetscSolverConfig& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  ::google::protobuf::uint8* SerializeWithCachedSizesToArray(::google::protobuf::uint8* output) const;
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::google::protobuf::Metadata GetMetadata() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // optional double atol = 1;
  inline bool has_atol() const;
  inline void clear_atol();
  static const int kAtolFieldNumber = 1;
  inline double atol() const;
  inline void set_atol(double value);

  // optional double rtol = 2;
  inline bool has_rtol() const;
  inline void clear_rtol();
  static const int kRtolFieldNumber = 2;
  inline double rtol() const;
  inline void set_rtol(double value);

  // optional double init_step = 3;
  inline bool has_init_step() const;
  inline void clear_init_step();
  static const int kInitStepFieldNumber = 3;
  inline double init_step() const;
  inline void set_init_step(double value);

  // optional int32 n_cores = 4;
  inline bool has_n_cores() const;
  inline void clear_n_cores();
  static const int kNCoresFieldNumber = 4;
  inline ::google::protobuf::int32 n_cores() const;
  inline void set_n_cores(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:pb.EXPetscSolverConfig)
 private:
  inline void set_has_atol();
  inline void clear_has_atol();
  inline void set_has_rtol();
  inline void clear_has_rtol();
  inline void set_has_init_step();
  inline void clear_has_init_step();
  inline void set_has_n_cores();
  inline void clear_has_n_cores();

  ::google::protobuf::UnknownFieldSet _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  double atol_;
  double rtol_;
  double init_step_;
  ::google::protobuf::int32 n_cores_;
  friend void  protobuf_AddDesc_common_2eproto();
  friend void protobuf_AssignDesc_common_2eproto();
  friend void protobuf_ShutdownFile_common_2eproto();

  void InitAsDefaultInstance();
  static EXPetscSolverConfig* default_instance_;
};
// ===================================================================


// ===================================================================

// EXPetscSolverConfig

// optional double atol = 1;
inline bool EXPetscSolverConfig::has_atol() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void EXPetscSolverConfig::set_has_atol() {
  _has_bits_[0] |= 0x00000001u;
}
inline void EXPetscSolverConfig::clear_has_atol() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void EXPetscSolverConfig::clear_atol() {
  atol_ = 0;
  clear_has_atol();
}
inline double EXPetscSolverConfig::atol() const {
  // @@protoc_insertion_point(field_get:pb.EXPetscSolverConfig.atol)
  return atol_;
}
inline void EXPetscSolverConfig::set_atol(double value) {
  set_has_atol();
  atol_ = value;
  // @@protoc_insertion_point(field_set:pb.EXPetscSolverConfig.atol)
}

// optional double rtol = 2;
inline bool EXPetscSolverConfig::has_rtol() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void EXPetscSolverConfig::set_has_rtol() {
  _has_bits_[0] |= 0x00000002u;
}
inline void EXPetscSolverConfig::clear_has_rtol() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void EXPetscSolverConfig::clear_rtol() {
  rtol_ = 0;
  clear_has_rtol();
}
inline double EXPetscSolverConfig::rtol() const {
  // @@protoc_insertion_point(field_get:pb.EXPetscSolverConfig.rtol)
  return rtol_;
}
inline void EXPetscSolverConfig::set_rtol(double value) {
  set_has_rtol();
  rtol_ = value;
  // @@protoc_insertion_point(field_set:pb.EXPetscSolverConfig.rtol)
}

// optional double init_step = 3;
inline bool EXPetscSolverConfig::has_init_step() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void EXPetscSolverConfig::set_has_init_step() {
  _has_bits_[0] |= 0x00000004u;
}
inline void EXPetscSolverConfig::clear_has_init_step() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void EXPetscSolverConfig::clear_init_step() {
  init_step_ = 0;
  clear_has_init_step();
}
inline double EXPetscSolverConfig::init_step() const {
  // @@protoc_insertion_point(field_get:pb.EXPetscSolverConfig.init_step)
  return init_step_;
}
inline void EXPetscSolverConfig::set_init_step(double value) {
  set_has_init_step();
  init_step_ = value;
  // @@protoc_insertion_point(field_set:pb.EXPetscSolverConfig.init_step)
}

// optional int32 n_cores = 4;
inline bool EXPetscSolverConfig::has_n_cores() const {
  return (_has_bits_[0] & 0x00000008u) != 0;
}
inline void EXPetscSolverConfig::set_has_n_cores() {
  _has_bits_[0] |= 0x00000008u;
}
inline void EXPetscSolverConfig::clear_has_n_cores() {
  _has_bits_[0] &= ~0x00000008u;
}
inline void EXPetscSolverConfig::clear_n_cores() {
  n_cores_ = 0;
  clear_has_n_cores();
}
inline ::google::protobuf::int32 EXPetscSolverConfig::n_cores() const {
  // @@protoc_insertion_point(field_get:pb.EXPetscSolverConfig.n_cores)
  return n_cores_;
}
inline void EXPetscSolverConfig::set_n_cores(::google::protobuf::int32 value) {
  set_has_n_cores();
  n_cores_ = value;
  // @@protoc_insertion_point(field_set:pb.EXPetscSolverConfig.n_cores)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace pb

#ifndef SWIG
namespace google {
namespace protobuf {


}  // namespace google
}  // namespace protobuf
#endif  // SWIG

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_common_2eproto__INCLUDED